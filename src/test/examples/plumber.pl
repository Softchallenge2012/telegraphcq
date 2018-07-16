#!/usr/bin/perl

################################################################################
#$RCSfile: plumber.pl,v $
#$Revision: 1.2 $
#$Date: 2005/06/13 02:59:29 $
################################################################################

use strict;
use warnings;

# Fred's modules
use pretty;
use tcqclient;
use tcqserver;

################################################################################
my $USAGE = <<USAGE_END;
plumber.pl

By Fred Reiss <phred\@cs.berkeley.edu>

Reads through TCQ memory debugging information to find leaks.  Creates a
Postgres database "plumber" with tables CONTEXT and OBJECT that describe the
live objects and memory contexts at the end of the trace.

After running this script, you can use queries like the following to track
down leaks:

select stack1, stack2, stack3, sum(size) as bytes, count(*) as num
from object
group by stack1, stack2, stack3
order by sum(size) desc;


Usage:    plumber.pl <infile> <pid>

Where:
        <infile> is logfile containing memory debugging info
        <pid> is PID of process whose memory you want to examine
USAGE_END
################################################################################


################################################################################
# ARGUMENTS

if (2 != (scalar @ARGV)) {
    die $USAGE;
}

my ($INFILE, $PID) = @ARGV;

################################################################################
# CONSTANTS

my $DBNAME = "plumber";

# DDL for our database schema.
my $DDL = <<END;

-- Active memory contexts
CREATE TABLE context(name varchar PRIMARY KEY,
                    size integer);


-- Active objects
CREATE TABLE raw_object(
    addr char(10) PRIMARY KEY,  
        -- e.g. '0x12345678'
        -- Primary key because an address shouldn't be allocated within 2 
        -- contexts at once.
    cxt varchar REFERENCES context (name),
    size integer
);

-- Stack traces
CREATE TABLE stacktrace(
    obj_addr char(10) REFERENCES raw_object (addr),
    level integer,
        -- How deep into the stack trace; starts with 0
    stack_addr char(10),
    PRIMARY KEY (obj_addr, level)
);

-- View that reconstructs the original format of the OBJECT table
-- Postgres won't know how to decorrelate this, but the primary key index on 
-- STACKTRACE should make query runtimes tolerable.
CREATE VIEW object as
SELECT R.addr as addr, R.cxt as cxt, R.size as size,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 0) AS stack1,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 1) AS stack2,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 2) AS stack3,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 3) AS stack4,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 4) AS stack5,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 5) AS stack6,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 6) AS stack7,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 7) AS stack8,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 8) AS stack9,
    (SELECT S.stack_addr FROM stacktrace S 
        WHERE S.obj_addr = R.addr AND S.level = 9) AS stack10
FROM raw_object R;
END

# Temp files for loading database tables.
my $CXT_TMP = "/tmp/plumber_cxt.csv";
my $OBJ_TMP = "/tmp/plumber_obj.csv";
my $STK_TMP = "/tmp/plumber_stk.csv";

# How many levels of stack trace do we load into the database?
my $MAX_STACK_LEVEL = 9;

################################################################################
# GLOBALS

# An hash of hash references, holding information about active memory 
# contexts.  The key to the first hash is the memory context name.  The keys
# of the second hash (representing facets of the state of the memory context) 
# are in constants below.
# Access to this hash is only through accessor functions!
my %CONTEXT_STATES;

# "Fields" of a memory context state.
my $ALLOC = "alloc";
    # Hash containing information about objects allocated within the context.
    # Maps starting address (in hex) to a list containing num bytes allocated,
    # followed by a stack trace, stored bottom-up.
my $SIZE = "size";
    # Size of the context in bytes

################################################################################
# SUBROUTINES

# Subroutine that returns the context state for the indicated memory context,
# creating a new empty state if necessary.
sub get_context_state($) {
    my ($context_name) = @_;

    if (not defined $CONTEXT_STATES{$context_name}) {

        # If we get here, we need to create a new state.
        my %new_state; 

        # Create the allocated objects hash.
        my %new_alloc;
        $new_state{$ALLOC} = \%new_alloc;
        $new_state{$SIZE} = -1;

        $CONTEXT_STATES{$context_name} = \%new_state;
    }

    return $CONTEXT_STATES{$context_name};
}

sub have_context_state($) {
    my ($context_name) = @_;
    return (defined $CONTEXT_STATES{$context_name});
}

sub remove_context_state($) {
    my ($context_name) = @_;
    delete $CONTEXT_STATES{$context_name};
}

# Subroutine that returns the allocation information for an object within a
# memory context, given the memory context's state.
sub get_alloc_info($$) {
    my ($state, $addr) = @_;

    my $alloc_ref = $state->{$ALLOC};

    if (not defined $alloc_ref->{$addr}) {
        # Create a new empty allocation info object
        my @new_info = (0,[]);

        $alloc_ref->{$addr} = \@new_info;
    }

    return $alloc_ref->{$addr};
}

sub have_alloc_info($$) {
    my ($state, $addr) = @_;

    my $alloc_ref = $state->{$ALLOC};

    return (defined $alloc_ref->{$addr});
}

sub remove_alloc_info($$) {
    my ($state, $addr) = @_;

    my $alloc_ref = $state->{$ALLOC};

    delete $alloc_ref->{$addr};
}

sub remove_all_alloc_info($) {
    my ($state) = @_;

    my %new_hash;
    $state->{$ALLOC} = \%new_hash;
        # Let the garbage collector take care of the rest.
}


# Handler functions
sub handle_create_cxt($$) {
    my ($cxt, $size) = ($1, $2);
        
    if (have_context_state($cxt)) {
        # This one seems to happen because a context was blown away when its
        # parent is reset; see TODO item at top of the script.
        #warn "Context '$cxt' allocated twice";
    }

    my $state = get_context_state($cxt);
    $state->{$SIZE} = $size;
}

sub handle_alloc_object($$$$) {
    my ($size, $addr, $cxt, $stack_ref) = @_;

    if (! have_context_state($cxt)) {
        # Longer-lived contexts may have been allocated before we started
        # tracing, so we silently create an internal state for contexts we
        # haven't heard about.
        #warn "Allocation into nonexistent context '$cxt'";
    }

    my $state = get_context_state($cxt);

    if (have_alloc_info($state, $addr)) {
        # For now, we assume that this error comes from an incomplete trace.
        #warn "Allocated object at address $addr in context '$cxt' twice";
    }

    my $info_ref = get_alloc_info($state, $addr);
    $info_ref->[0] = $size;
    $info_ref->[1] = $stack_ref;
}

sub handle_free_object($$$) {
    my ($addr, $size, $cxt) = @_;

    if (! have_context_state($cxt)) {
        # Longer-lived contexts may have been allocated before we started
        # tracing, so we ignore this error.
        #warn "Freeing from nonexistent context '$cxt'";
        return;
    }

    my $state = get_context_state($cxt);

    if (! have_alloc_info($state, $addr)) {
        # This error seems to happen because of an incomplete trace; at any
        # rate, it's not our job to track down deletion of freed pointers!
        #warn "Freeing unknown object at $addr in context '$cxt'";
        return;
    }

    remove_alloc_info($state, $addr);
}

sub handle_reset_context($) {
    my ($cxt) = @_;

    if (! have_context_state($cxt)) {
        # We'll assume that the context was created before TCQ could start
        # tracing.
        #warn "Resetting nonexistent context '$cxt'";
        return;
    }

    my $state = get_context_state($cxt);

    remove_all_alloc_info($state);
}

sub handle_delete_context($) {
    my ($cxt) = @_;

    if (! have_context_state($cxt)) {
        # This generally happens because the context was allocated before TCQ
        # could start tracing.
        #warn "Deleting nonexistent context '$cxt'";
        return;
    }

    remove_context_state($cxt);
}

################################################################################
############################################################
########################################
####################
# BEGIN SCRIPT

################################################################################
# READ THE TRACE
################################################################################
open INFILE, $INFILE or die "Couldn't open $INFILE for reading";
my $line;
my $numproc = 0;
my $numevt = 0;
while ($line = <INFILE>) {

    $numproc++;

    $numevt++;
        # Decremented below if we don't find an event on the current line.
    
    # Try to parse the line.
    # N.B. The regular expressions in the following IF statements need to be
    # kept in sync with src/backend/utils/mmgr/mcxt.c!
    if ($line =~ /$PID: Memory context '(.*)' with size (\d+) created/) {
        # Memory context creation.
        my ($cxt, $size) = ($1, $2);
        handle_create_cxt($cxt, $size);

    } elsif ($line 
        =~ /$PID: allocating (\d+) bytes at (0x[0-9a-f]*) in context '(.*)' from ((0x[0-9a-f]*,)*0x[0-9a-f]*)/)
    {
        # Object allocation 
        my ($size, $addr, $cxt, $stack) = ($1,$2,$3,$4);
        my @stack = split ',', $stack;
        
        # Truncate the stack to keep loading times down.
        splice @stack, $MAX_STACK_LEVEL;

        handle_alloc_object($size, $addr, $cxt, \@stack);

    } elsif ($line 
        =~ /$PID: freeing (0x[0-9a-f]*) of size (\d+) in context '(.*)'/)
    {
        # Freeing an object
        my ($addr, $size, $cxt) = ($1, $2, $3);
        handle_free_object($addr, $size, $cxt);

    } elsif ($line 
        =~ /$PID: Memory context reset called on context '(.*)'/)
    {
        # Resetting a context
        my ($cxt) = ($1);
        handle_reset_context($cxt);

    } elsif ($line 
        =~ /$PID: Memory context delete called on context '(.*)'/)
    {
        # Deleting a context
        my ($cxt) = ($1);
        handle_delete_context($cxt);
    } else {
        # Didn't find an event.
        $numevt--;
    }

    status_inflight("Read $numproc lines and found $numevt events");

}

status_done("Processed $numproc lines.");

################################################################################
# CREATE THE DATABASE
################################################################################

status_inflight("Creating database...");

my $srvr = new tcqserver($DBNAME);
$srvr->logfile("/tmp/plumber_logfile");
$srvr->stop_nicely();
$srvr->remove_logfile();
$srvr->start_pg();


system "dropdb --quiet $DBNAME";
system "createdb --quiet $DBNAME";

my $client = new tcqclient($DBNAME);
$client->dosql($DDL);
status_inflight("Done creating database.");

################################################################################
# LOAD THE DATABASE
################################################################################


open CXT, ">$CXT_TMP" or die "Couldn't open $CXT_TMP for writing";
open OBJ, ">$OBJ_TMP" or die "Couldn't open $OBJ_TMP for writing";
open STK, ">$STK_TMP" or die "Couldn't open $STK_TMP for writing";

foreach my $cxt (keys %CONTEXT_STATES) {
    status_inflight("Processing context '$cxt'");
    my $state = get_context_state($cxt);
    my $size = $state->{$SIZE};

    print CXT "$cxt,$size\n";

    my $alloc_ref = $state->{$ALLOC};

    foreach my $addr (keys %$alloc_ref) {
        my $info_ref = get_alloc_info($state, $addr);

        my ($size, $stack_ref) = @$info_ref;

        print OBJ "$addr,$cxt,$size\n";
        
        for (my $lev = 0; $lev < (scalar @$stack_ref); $lev++) {
            print STK "$addr,$lev,$stack_ref->[$lev]\n";
        }
    }
}

close STK;
close OBJ;
close CXT;

status_inflight("Loading tables: context");

$client->dosql("copy context from '$CXT_TMP' delimiter ','");

status_inflight("Loading tables: raw_object");

$client->dosql("copy raw_object from '$OBJ_TMP' delimiter ','");

status_inflight("Loading tables: stacktrace (may take a while)");

$client->dosql("copy stacktrace from '$STK_TMP' delimiter ','");
$client->dosql("vacuum analyze");

status_inflight("Done.");

print STDERR <<END;
Information about live objects can be found in the DB instance
"$DBNAME". Please restart postgres after this message.
END

$srvr->stop_nicely();

close INFILE;

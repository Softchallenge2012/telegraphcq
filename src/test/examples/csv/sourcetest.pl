#!/usr/bin/perl

################################################################################
# sourcetest.pl
#
# Test of the Perl module source.pm.
#
# TODO: Make this run for people other than phred.
################################################################################

use strict;
use warnings;

use Term::ANSIColor;

# My CSV wrapper library (source.pm).
use source;

################################################################################
# SCRIPT SETTINGS

my $WAIT_FOR_DEBUGGER = 1;

# Should we create the database and streams, or are they already created for
# us?
my $CREATE_STREAMS = 1;

# Are we running with the TCQ beta?
my $USING_TCQ_BETA = 0;


my $HOST = "localhost";
my $PORT = 5533;
my $WRAPPERTYPE = "csvwrapper";
my $SCHEMA = "bug";
my $STREAM = "S";

my $DBNAME = "demo";

my $PGRSHOME = $ENV{"PGRSHOME"};

#my $PSQL_COMMAND = "$PGRSHOME/bin/psql -d $DBNAME -h $HOST";
my $PSQL= "$PGRSHOME/bin/psql";
my $PSQL_COMMAND = "$PSQL -d $DBNAME";

################################################################################


# Extra DDL code that we need to add for the beta version.
my $EXTRA_BETA_DDL = <<END;
create schema wrappers;
create or replace function wrappers.csv_init(integer) returns boolean
as '$PGRSHOME/lib/libcsv.so','csv_init' language 'C';
create or replace function wrappers.csv_next(integer) returns boolean
as '$PGRSHOME/lib/libcsv.so','csv_next' language 'C';
create or replace function wrappers.csv_done(integer) returns boolean
as '$PGRSHOME/lib/libcsv.so','csv_done' language 'C';

-- We need to create wrappers explicitly with the beta.
-- In fact, we need to explicitly drop the wrapper, too, since the wrapper 
-- doesn't go away when we drop the database instance.
drop wrapper CSVWRAPPER;
create wrapper CSVWRAPPER (init=wrappers.csv_init, next=wrappers.csv_next, done
=wrappers.csv_done);

END

# The common code for creating the test stream.
my $DDL = <<END;
create schema bug;

create stream bug.S (a integer, b integer,
                    tcqtime timestamp TIMESTAMPCOLUMN) 
type unarchived on overload keep sample;

alter stream bug.S add wrapper CSVWRAPPER;
END

################################################################################
# Subroutine that creates a temp file with the indicated text.
#
#sub maketmp($filename, $contents) {
sub maketmp($$) {
    my ($filename, $contents) = @_;

    open TMP, "> $filename" or die "Couldn't open temp file '$filename'";
    print TMP $contents;
    close TMP;

}

################################################################################
# Subroutine that runs some SQL through psql.
#
# Arguments:    <sqlstr> is the SQL to run.
#sub dosql($sqlstr, $silent) {
sub dosql($$) {
    my ($sqlstr) = @_;


    # Create a temporary file to feed through psql.
    my $tmpfile = "/tmp/srctesttmp.sql";
    maketmp($tmpfile, "$sqlstr\n\\q\n");
        # The trailing "\q" tells psql to quit nicely.
        # TODO: Is this necessary?

    # Run the script through psql.
    system "$PSQL_COMMAND -f $tmpfile";
}

################################################################################
# SUBROUTINE run_query_async 
# Spawns a child process to run a query.
#
# Arguments:
#       <dbname> is the name of the database instance.
#       <query> is a SQL query to run.
#       <query_name> is a human-readable nickname for the query.
#       <num_rows> is the number of rows to fetch of the query results.
#       <show_results> is 1 if you want to display the results for the query
#               in an xterm.
#       <use_cq> is 1 if you want to use a cursor for the query.
#
# Returns the PID of the child.
#
#sub run_query_async($dbname, $query, $query_name, $num_rows, $show_results
#                   $use_cq) {
sub run_query_async($$$$$$) {
    my ($dbname, $query, $query_name, $num_rows, $show_results, $use_cq) = @_;

    print STDERR "Starting query '$query_name'\n";

    my $tmpfile = "/tmp/${query_name}_tmp.sql";

    my $child_pid = fork;
    if (0 == $child_pid) {
        # Child for this query.
        maketmp($tmpfile, "$query\n");

        my $commandline;
        if (1 == $use_cq) {
            $commandline = "$PSQL -C -I $num_rows -d $dbname -f $tmpfile";
        } else {
            $commandline = "$PSQL -d $dbname -f $tmpfile";
        }


        if (1 == $show_results) {
            # Sleep after running the query, so we can see the last error 
            # message.
#            $commandline .= "; sleep $TERMINAL_CLOSE_DELAY";
            my $xterm_cmd = "xterm -T '$query_name query' -e $commandline";
            print STDERR "Running $xterm_cmd\n";
            system $xterm_cmd;
        } else {
            system "$commandline -o /dev/null";
        }

        if (1 == $show_results) {
            print colored "$query_name query done.\n", 'bold';
        }
        exit;
    }

    return $child_pid;
}



################################################################################
# BEGIN SCRIPT

# First, create the database.
if ($CREATE_STREAMS) {
    system "pg_ctl stop";
    sleep 1;
    system "pg_ctl start -l logfile";
    sleep 5;
    system "dropdb $DBNAME";
    system "createdb $DBNAME";

    if ($USING_TCQ_BETA) {
        dosql($EXTRA_BETA_DDL, 0);
    }
    dosql($DDL, 0);
}

# Make sure we're in streaming mode.
system "pg_ctl stop";
sleep 1;
system "rm -f logfile; pg_ctl start -l logfile -o \"-t $DBNAME -u phred -G\"";
sleep 3;

# Connect to the wrapper clearinghouse.
print STDERR "Connecting to wrapch...\n";
my $s = source->new($HOST, $PORT, $WRAPPERTYPE, $SCHEMA, $STREAM);
$s->open();

# Start a query.
print STDERR "Starting query...\n";
run_query_async($DBNAME, "SELECT * FROM bug.S;\n", "My_Query", 100, 1, 1);
sleep 5;

if ($WAIT_FOR_DEBUGGER) {
    print STDERR "Press return to continue.\n";
    <>;
}

# Send a tuple.
print STDERR "Sending tuple\n";
$s->sendLine("1, 42");

# Wait 2 seconds.
sleep 5;

# Send 10 tuples, all at once.
print STDERR "Sending 10 tuples\n";
for (my $i = 2; $i < 12; $i++) {
    $s->sendLine("$i, 42");
}

# Wait 5 seconds.
sleep 5;

# Send another tuple.
print STDERR "Sending tuple\n";
$s->sendLine("12, 42");

# Wait another 5 seconds.
sleep 5;
print STDERR "Closing connection\n";
$s->close();



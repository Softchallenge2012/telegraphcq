#!/usr/bin/perl

# sanitylite.pl
#
# Perl script for replicating race conditions between multiple queries.
# Should be entirely self-contained.  Modify for your own needs...
#
# Usage: ./sanitylite.pl [-debug]
#
# Where:
#       -debug means to wait for the user to attach a debugger.

use FileHandle;
use Socket;
use strict;
use warnings;
use Term::ANSIColor;

die "Usage: ./sanitylite.pl [-debug]\n" unless 
(
    (1 == (scalar @ARGV) and ("-debug" eq $ARGV[0]))
    or 0 == (scalar @ARGV)
);

################################################################################
# EXTERNAL CONSTANTS
################################################################################

# Set the following to 1 if you want to attach a debugger before sending the
# streams.
my $ATTACH_DEBUGGER = (1 == (scalar @ARGV)) ? 1 : 0;

# Set the following to 1 to run the queries in new terminal windows.
my $USE_SEPARATE_WINDOWS = 1;

# Set the following to 1 to use the "quik" wrapper code.  If set to 0, we use
# the CSV wrapper.
my $USE_QUIK = 1;

# Set the following to 1 if you want to run the queries in CQ mode on the
# backend.
my $USE_CQ_BE = 1;

# Set the following to 1 if you want to use cursors to submit queries.
my $USE_CQ_FE = 1;

# How many rows to fetch for each query.
my $ROWS_PER_QUERY = 20;

my $USERNAME = $ENV{"USER"};

################################################################################
# INTERNAL CONSTANTS
################################################################################

# Wrapper clearinghouse port
my $PORTNO = 5533;

my $HOST = "localhost";
my $SOURCENAME;

if (1 == $USE_QUIK) {
    $SOURCENAME = "quikwrapper,sanitylite.r";
        # NOTE: Must be all lower-case!!!
} else {
    $SOURCENAME = "csvwrapper,sanitylite.r";
}

my $DBNAME = "bug";

my $MSG_COLOR = "red";
    # The color of status messages.

my $CHILD_MSG_COLOR = "bold";
    # The color of status messages from child processes.

# My shell aliases...
my $stp = "pg_ctl stop";
my $sne = "pg_ctl start -l logfile";
my $stqe = "pg_ctl start -l logfile -o \"-t $DBNAME -u $USERNAME -G\"";

# (Start TelegraphCQ - without eddies):
my $stq  = "pg_ctl start -l logfile -o \"-t $DBNAME -u $USERNAME\"";

# Temporary files...
my $DDL_TMP = "/tmp/ddl_tmp.sql";
my $R_QUIK_TMP = "/tmp/r_quik.csv";

my $R_SRC_DUMP = "/tmp/r_source.dat";

# Database creation code.
my $DDL = <<'END_DDL';
create schema sanitylite;

create stream sanitylite.R ( 
    a integer,
    tcqtime timestamp TIMESTAMPCOLUMN
) type unarchived;

create table sanitylite.S ( b integer, c integer );

-- Create the table that will be dumped to disk, then read back as R.
create table sanitylite.R_source (ts integer, a integer);

END_DDL

# SQL to load the source of R into Postgres and make it into a binary dump.
my $R_QUIK_DDL = 
    "copy sanitylite.R_source from '$R_QUIK_TMP' with delimiter as ',';\n"
.   "select quik_dump_table('sanitylite.R_source','$R_SRC_DUMP');\n";

# Choose the appropriate wrapper.
if (1 == $USE_QUIK) {
    $DDL .= "alter stream sanitylite.R add wrapper quikwrapper;\n";
} else {
    $DDL .= "alter stream sanitylite.R add wrapper csvwrapper;\n";
}

# Queries.
my $MAIN_Q = <<'END_MAIN_Q';
--select R_stream.a, S_table.c
--from sanitylite.R R_stream, sanitylite.S S_table
--where  R_stream.a = S_table.b;
select * from sanitylite.R;
END_MAIN_Q

my $KEPT_Q = <<'END_SECOND_Q';
select * from sanitylite.__r_kept;
END_SECOND_Q

my $KEPT_COUNT_Q = <<'END_KEPT_COUNT_Q';
select * from sanitylite.__r_kept_count;
END_KEPT_COUNT_Q

my @QUERIES = ($MAIN_Q);
my @QUERY_NAMES = ("Main");
#my @QUERIES = ($MAIN_Q, $KEPT_Q, $KEPT_COUNT_Q);
#my @QUERY_NAMES = ("Main", "Kept", "Kept_Count");

# The "R" stream, for when we're using CSV wrappers.
my $R_STREAM = <<'END_R_STREAM';
1,2003-06-06 20:50:01 CEST
2,2003-06-06 20:50:02 CEST
3,2003-06-06 20:50:03 CEST
4,2003-06-06 20:50:04 CEST
5,2003-06-06 20:50:05 CEST
6,2003-06-06 20:50:06 CEST
7,2003-06-06 20:50:07 CEST
8,2003-06-06 20:50:08 CEST
9,2003-06-06 20:50:09 CEST
10,2003-06-06 20:50:10 CEST
END_R_STREAM

# The contents of the "R" stream, when it's loaded into a table and sent
# through the binary wrapper.
my $R_QUIK_STREAM = <<'END_R_QUIK';
0,0
1000000,1
2000000,2
3000000,3
4000000,4
5000000,5
6000000,6
7000000,7
8000000,8
9000000,9
10000000,10
END_R_QUIK

################################################################################
# SUBROUTINES
################################################################################

# Subroutine that pretty-prints a "please wait" message.
# First argument is the message, second is the color, and third is the number
# of seconds to delay.
sub please_wait($$$) {
    my ($msg, $color, $seconds) = @_;

    # Turn off buffering. 
    select STDERR; 
    my $orig_buf = $|;
    $| = 1; 

    print STDERR colored $msg, $color;

    for (my $i = 0; $i < $seconds; $i++) {
        print STDERR colored ".", $color;
        sleep 1;
    }
    print STDERR "\n";

    # Restore buffering settings.
    $| = $orig_buf; 
    select STDOUT; 
}


################################################################################
# Subroutine that spawns a child to run a query.
#
# Arguments:
#       <dbname> is the name of the database instance.
#       <query> is a SQL query to run.
#       <query_name> is a human-readable nickname for the query.
#       <num_rows> is the number of rows to fetch of the query results.
#       <show_results> is 1 if you want to display the results for the query
#               in an xterm.
#
# Returns the PID of the child.
#
#sub run_query_async($dbname, $query, $query_name, $num_rows, $show_results) {
sub run_query_async($$$$$) {
    my ($dbname, $query, $query_name, $num_rows, $show_results) = @_;

    my $tmpfile = "/tmp/${query_name}_tmp.sql";

    my $child_pid = fork;
    if (0 == $child_pid) {
        # Child for this query.
        open TMP, "> $tmpfile" or die "Couldn't open temp file $tmpfile.\n";
        print TMP $query;
        close TMP;

        my $commandline;
        if (1 == $USE_CQ_FE) {
            $commandline = "psql -C -I $num_rows -d $dbname -f $tmpfile";
        } else {
            $commandline = "psql -d $dbname -f $tmpfile";
        }


        if (1 == $show_results) {
            # Sleep after running the query, so we can see the last error 
            # message.
#            $commandline .= "; sleep 5";
            system "xterm -T '$query_name query' -e $commandline";
        } else {
            system "$commandline >& /dev/null";
        }

        print colored "$query_name query done.\n", 'bold';
        exit;
    }

    return $child_pid;
}



################################################################################
# Begin script.
################################################################################

# Generate temp files.
open TMP, "> $DDL_TMP" or die "Couldn't open temp file $DDL_TMP.\n";
print TMP $DDL;
close TMP;


system $stp;
system "rm logfile";
system "echo 'Log of database creation for sanitylite.pl' > logfile";
system $sne;
please_wait("Waiting for database to start", $MSG_COLOR, 5);
system "dropdb --quiet $DBNAME";
system "createdb --quiet $DBNAME";
system "psql -d $DBNAME -f $DDL_TMP";

# If necessary, load the table we'll use as an indirect source of R.
if (1 == $USE_QUIK) {
    open TMP, "> $R_QUIK_TMP";
    print TMP $R_QUIK_STREAM;
    close TMP;

    open TMP, "> $DDL_TMP";
    print TMP $R_QUIK_DDL;
    close TMP;

    system "psql -d $DBNAME -f $DDL_TMP";
}

system $stp;
system "mv logfile sl_create.log";

system "echo 'Log of query execution for sanitylite.pl' > logfile";

if (1 == $USE_CQ_BE) {
    system $stqe;
} else {
    system $stq;
}
please_wait("Waiting for TelegraphCQ to start", $MSG_COLOR, 5);

# Spawn children to run the queries.
my @children = ();
for (my $i = 0; $i < scalar @QUERIES; $i++) {
    my $query = $QUERIES[$i];
    my $query_name = $QUERY_NAMES[$i];

    my $tmpfile = "/tmp/${query_name}_tmp.sql";

    my $child_pid = run_query_async($DBNAME, $query, $query_name, 
                                     $ROWS_PER_QUERY, 1);

    # Parent
    push @children, $child_pid;

    # Sleep some time to make sure that the queries start in the same order
    # all the time.
    sleep 1;
}


# Wait for the child to start up.
please_wait("Waiting for queries to start up", $MSG_COLOR, 3 + scalar @QUERIES);

# Give yourself time to attach a debugger to a postgres process.
if (1 == $ATTACH_DEBUGGER) {
    select STDERR; $| = 1; select STDOUT;
    print STDERR colored "Attach a debugger, then press return. ", 'bold';
    my $line = <STDIN>;
}




# Parent process runs the stream.
# 
# Code copied from source.pl.
my $iaddr = inet_aton($HOST);
my $paddr = sockaddr_in($PORTNO,$iaddr);
my $proto = getprotobyname('tcp');

print STDERR colored "Connecting to wrapper clearinghouse...\n", $MSG_COLOR;

socket(SOCK, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK, $paddr) || die "connect: $!";
autoflush SOCK,1;
my $namestr = sprintf("%4d%s\x0",length($SOURCENAME)+1,$SOURCENAME);
print SOCK $namestr;

# Give things a chance to settle, so that the log file entries come out in the
# right order.
please_wait("Waiting for wrapper clearinghouse to settle down", $MSG_COLOR, 3);
print STDERR colored "Sending stream...\n", $MSG_COLOR;

if (1 == $USE_QUIK) {
    # Stream the binary file into the socket.
    open BINFILE, $R_SRC_DUMP or die "Couldn't open $R_SRC_DUMP.\n";
    my $scalar_var;
    my $bytes_read;

    do {
        $bytes_read = sysread BINFILE, $scalar_var, 4096;
        syswrite SOCK, $scalar_var, $bytes_read;
    } while ($bytes_read > 0);
    
} else {
    print SOCK $R_STREAM;
    print SOCK "\n";
}
close SOCK;

print STDERR colored "Done sending stream.\n", $MSG_COLOR;

# Should we be interactive about killing off the queries?
if (1 == $USE_SEPARATE_WINDOWS or 1 == $ATTACH_DEBUGGER) {
    select STDERR; $| = 1; select STDOUT;
    print STDERR "    Press return to continue. ";
    my $line = <STDIN>;
} else {
    please_wait("Waiting for queries to complete", $MSG_COLOR, 3);
}

# Force the queries to disconnect, assuming we're not stopping the system with
# a debugger.
system $stp;

# Wait for the queries to finish.
print STDERR colored "Waiting for queries to finish.\n", $MSG_COLOR;

foreach my $pid (@children) {
    waitpid $pid, 0x0;
}


print STDERR colored "Done.\n", $MSG_COLOR;




#! /usr/bin/perl

use strict;
use warnings;
use Term::ANSIColor;
	# Oooo!  Pretty colors!
use POSIX qw(asctime);
	# For generating timestamps.
use Carp;
use Time::HiRes qw( gettimeofday tv_interval usleep );

use source2;
use tcqclient;

# Turn off STDERR buffering.
select(STDERR); $| = 0; select(STDOUT);

#
# CONSTANTS
#

my $USAGE = <<END;
################################################################################
# usage: streamsanity.pl <dbname> <username> <mode>
#
# Where:
#   -- <dbname> is an already-created database in which to run the test
#   -- <username> is your PostgreSQL username
#   -- <mode> is "rt" (real-time sequential tests) 
#       or "batch" (non-real time, all tests run at once, predictable results)
#
# A script that tests whether TelegraphCQ streams are doing what they're
# supposed to.
#
# This script requires that your PATH contain the build of Postgres that you
# want to test, and that the database and user in the CONSTANTS section
# already exist.
################################################################################
END
die $USAGE unless 3 == scalar @ARGV; 

# Read arguments
my ($TEST_DB, $USERNAME, $MODE) = @ARGV;


# Postgres logfile for the processes we run.
my $PG_LOGFILE = "./logfile";

# How to run psql
my $ROWS_PER_ITERATION = 1;
my $ROWS_PER_QUERY = 20;
my $PSQL_COMMAND = "psql -d $TEST_DB";

my $STREAMING_PSQL_COMMAND = 
    "$PSQL_COMMAND -C -i $ROWS_PER_ITERATION -I $ROWS_PER_QUERY";


# The UNIX time at which the streams start in batch mode:
# midnight, jan 1, 2000
my $BATCH_START_SEC = POSIX::mktime(0,0,0,1,0,100);

# PostgreSQL version of the starting time
my $BATCH_START_STR = source2::gen_pg_timestamp($BATCH_START_SEC);

# Should we show query results in batch mode?
my $SHOW_BATCH_QUERY_RESULTS = 0;

# Where should the batch mode logfiles go?
my $BATCH_LOGFILE_DIR = "/tmp/streamsanity";

# Queries that we run over the test tables.
my @QUERIES = 
(
<<END,
-- Hello world.  Should return current time.
SELECT NOW();
END
<<END,
-- Table scan.  Should return the numbers 5-14
SELECT * FROM R;
END
<<END,
-- Stream scan
SELECT * FROM ss.TESTSTREAM3;
END
<<END,
-- Stream-table join.
SELECT J FROM ss.TESTSTREAM1, R where R.A = ss.TESTSTREAM1.I;
END
<<END,
-- Hopping window with selection
SELECT * 
FROM ss.TESTSTREAM1 [range by '1 second' slide by '1 second'];
END
<<END,
-- Sliding window with aggregate and group by.  
-- Each window should contain aprox. 10 tuples total across all groups.
SELECT K, count(*), wtime(*)
FROM ss.TESTSTREAM3 [range by '1 second' slide by '0.5 seconds']
GROUP BY k;
END
<<END,
-- Hopping with aggregate and funky window size.  
-- Each window should contain aprox. 12 tuples.
SELECT count(*), wtime(*)
FROM ss.TESTSTREAM3 [range by '1.234 second' slide by '0.4321 seconds'];
END
<<END,
-- Sliding window with "holes".  Each window should contain aprox. 5 tuples.
SELECT count(*), wtime(*)
FROM ss.TESTSTREAM3 [range by '0.5 seconds' slide by '1 second'];
END
<<END,
-- Windowed stream-stream join.  Should return big tuples from within 1 
-- second of each other
SELECT * 
FROM ss.TESTSTREAM1 [range by '1 second' slide by '0.5 seconds'],
    ss.TESTSTREAM2 [range by '1 second' slide by '0.5 seconds']
WHERE ss.TESTSTREAM1.I = ss.TESTSTREAM2.K;
END
<<END,
-- Check for basic window jitter.  Each window's min and max timestamps should
-- be between (wtime(*) - RANGE BY) and wtime(*)
SELECT 
    count(*), 
    min(tcqtime), 
    max(tcqtime),
    wtime(*)
FROM ss.TESTSTREAM1 [range by '0.5 seconds' slide by '1 second'];
END
<<END,
-- Check for inter-stream window jitter.  
-- This query should return 2 results 1 second apart, followed by 3 
-- results 0.5 seconds apart.
-- rangeby1 and rangeby2 should be close to 1.5 but always less than 1.5
-- start1 and start2 should advance as the windows on TESTSTREAM1 and 
-- TESTSTREAM2 advance, e.g. every 1.5 seconds and 1 seconds, respectively.
SELECT count(*), 
    min(s1.tcqtime) as start1,
    min(s2.tcqtime) as start2,
    wtime(*),
    max(s1.tcqtime) - min(s1.tcqtime) as rangeby1,
    max(s2.tcqtime) - min(s2.tcqtime) as rangeby2
FROM ss.TESTSTREAM1 s1 [range by '1.5 seconds' slide by '1.5 seconds'],
    ss.TESTSTREAM2 s2 [range by '1.5 seconds' slide by '1 seconds']
WHERE s1.one = s2.one;
END
<<END,
-- WITH construct, and a hack to do range predicates.
-- This set of queries computes the average value of K from the previous 
-- time window (threshold column), then returns all tuples with a value of 
-- K greater than that average (value column).  value should always be
-- greater than threshold, and all threshold values for a particular 
-- thresholdtime should be the same.
WITH 
    ss.thresh as (
        select avg(K) as a, 1 as one, wtime(*) as ts
        from ss.TESTSTREAM2 [range by '1 sec' slide by '1 sec']) 
    ss.hack as (
        select K, 1 as one, tcqtime 
            from ss.TESTSTREAM3) 
    (select T.tcqtime as thresholdtime, H.tcqtime as tupletime,
            T.A as threshold, H.K as value
        from ss.hack H [range by '1 sec' slide by '1 sec'], 
            ss.thresh T [range by '1 sec' slide by '1 sec'] 
        where H.one = T.one and H.K > T.A);
END
<<END,
-- Punctuation and joins.
-- This query joins the threshold subquery with the static table R.
-- If punctuation is visible on your query output, you should see a 
-- punctuation tuple at the end of each join result.
WITH 
    ss.thresh as (
        select avg(K) as a, 1 as one, wtime(*) as ts
        from ss.TESTSTREAM2 [range by '1 sec' slide by '1 sec']) 
    (select * from ss.thresh, r where ss.thresh.a = r.a);
END
);

# How many seconds do we run the stream for?
my $TEST_STREAM_TIME_SEC = $ROWS_PER_QUERY * 20;

# How many microseconds between tuples?  Values below 10000 are likely to
# cause load-shedding.
# One value for each stream.
my %TEST_STREAM_TUPLE_INTERVAL_USEC = 
(
    "teststream1" => 177000,
    "teststream2" => 234000,
    "teststream3" => 100000
);

# The streams we send tuples to.
my @TEST_STREAM_NAMES = 
(
    "teststream1",
    "teststream2",
    "teststream3",
);

# Tick interval of the system's real time clock.
my $TICK_INTERVAL_USEC = 10000;

# Should we display counts of kept and dropped tuples in separate windows?
my $MONITOR_DROPS = 0;

# Should we pause for the debugger after TCQ startup?
my $WAIT_FOR_DEBUGGER = 0;

# Temporary file we use for transitory SQL scripts.
my $SQL_TMP_FILE = "/tmp/dosql_tmp.sql";

# Temporary file we use for table contents.
my $TABLE_DUMP_TMP_FILE = "/tmp/table_tmp.txt";

# Our test data stream.
my @TEST_STREAM = (
    [1,1,-12553],
    [1,2,-7812],
    [1,3,286],
    [1,4,-11069],
    [1,5,-18263],
    [1,6,-25415],
    [1,7,-18190],
    [1,8,-28055],
    [1,9,20992],
    [1,10,-26447],
    [1,11,4049],
    [1,12,22055],
    [1,13,8425],
    [1,14,26855],
    [1,15,-13099],
    [1,16,-13055],
    [1,17,10830],
    [1,18,26799],
    [1,19,-7651],
    [1,20,-32294],
#    [1,21,702],
#    [1,22,-18241],
#    [1,23,12706],
#    [1,24,-19682],
#    [1,25,-3094],
#    [1,26,-18162],
#    [1,27,22018],
#    [1,28,-30057],
#    [1,29,-9329],
#    [1,30,-17977],
#    [1,31,15748],
#    [1,32,16234],
#    [1,33,-749],
#    [1,34,-11855],
#    [1,35,30767],
#    [1,36,-12918],
#    [1,37,29765],
#    [1,38,-23606],
#    [1,39,-13147],
#    [1,40,22835],
#    [1,41,-24538],
#    [1,42,32713],
#    [1,43,31030],
#    [1,44,10722],
#    [1,45,4712],
#    [1,46,-7446],
#    [1,47,-17147],
#    [1,48,-5803],
#    [1,49,-19096],
#    [1,50,2673]
);


#
# PSQL (Sub)SCRIPTS
#

# Script to drop existing tables created by previous runs of streamsanity.pl.
my $DROP_SCRIPT = <<'END_DROP_SCRIPT';
DROP TABLE R;
drop stream ss.hack;
drop stream ss.thresh;

drop stream ss.TESTSTREAM1;
drop stream ss.TESTSTREAM2;
drop stream ss.TESTSTREAM3;
drop schema ss;
END_DROP_SCRIPT

# Script to create new tables.
my $CREATE_SCRIPT = <<'END_CREATE_SCRIPT';

-- Tables
create table R(A integer);

create schema ss;

-- Streams
create stream ss.TESTSTREAM1(ONE integer, I integer, J integer, 
    TCQTIME timestamp) 
	type unarchived;
create stream ss.TESTSTREAM2(ONE integer, K integer, L integer, 
    TCQTIME timestamp) 
	type unarchived;
create stream ss.TESTSTREAM3(ONE integer, K integer, L integer, 
    TCQTIME timestamp) 
	type unarchived;

alter stream ss.TESTSTREAM1 add wrapper CSVWRAPPER;
alter stream ss.TESTSTREAM2 add wrapper CSVWRAPPER;
alter stream ss.TESTSTREAM3 add wrapper CSVWRAPPER;

-- Intermediate streams for subqueries.
create stream ss.thresh(A integer, one integer, TCQTIME timestamp)
        type unarchived;

create stream ss.hack(K integer, one integer, TCQTIME timestamp)
        type unarchived;

END_CREATE_SCRIPT


# 
# SUBROUTINES
#


################################################################################
# Subroutine that waits for the user to press <return>
################################################################################
sub wait_for_return() {
    print STDERR "Press <return> to continue.\n";
    <>;
}

# Compute the qualified name of a stream.
sub get_qual_stream_name($$) {
    my ($schema, $stream) = @_;
    if (defined $schema and $schema ne "") {
        return "$schema.$stream";
    } else {
        return $stream;
    }
}
 
################################################################################
# Subroutine that generates the input file for the indicated stream.
# Argument: 
#       <schema> is the schema in which the stream resides
#       <stream> is the unqualified name of the stream.
#
# Returns the filename.
################################################################################
sub gen_strm_dump($$) {
    my ($schema, $stream) = @_;

    my $qual_stream_name = get_qual_stream_name($schema, $stream);

    my $filename = "/tmp/${qual_stream_name}.csv";

    open DUMP, ">$filename" or die "Couldn't open $filename for writing";

    # Generate the timestamped tuples.
    my $tuple_interval_sec = 
        $TEST_STREAM_TUPLE_INTERVAL_USEC{$stream} / 1000000.0;

    my $end_sec = $BATCH_START_SEC + $TEST_STREAM_TIME_SEC;

    my $pos = 0;
    for (my $now_sec = $BATCH_START_SEC; $now_sec < $end_sec;
        $now_sec += $tuple_interval_sec)
    {
        my $ts_str = source2::gen_pg_timestamp($now_sec);

        my $line_ref = $TEST_STREAM[$pos];

        my $line_str = join ',', @$line_ref;
        $line_str .= ",$ts_str\n";

        print DUMP $line_str;

        $pos = ($pos + 1) % (scalar @TEST_STREAM);
    }

    close DUMP;

    return $filename;
}


################################################################################
# Subroutine that starts the stream source in the background.
#
# Argument: 
#       <schema> is the schema in which the stream resides
#       <stream> is the unqualified name of the stream.
#
# Returns the PID of the source for easy killing later on.
################################################################################
sub start_stream_source($$) {
    my ($schema, $stream) = @_;

    # Compute the qualified name of the stream.
    my $qual_stream_name = get_qual_stream_name($schema, $stream);
    
    my $tuple_interval = $TEST_STREAM_TUPLE_INTERVAL_USEC{$stream};
    die "Don't have an inter-tuple arrival interval for stream $stream"
        unless defined $tuple_interval;

    my $pid;

    # Code copied from page 146 of the camel book.
    FORK:
    if ($pid = fork) {
        # Parent does nothing.
        return $pid;
            
    } elsif (defined $pid) {
        # Initialize the data source library.
        my $s = source2->new("localhost", $qual_stream_name);
        $s->open();

        # Send 10 tuples a second.
        my $divisor = 1;
        if ($tuple_interval > 0) {
            $divisor = $tuple_interval; 
        }
        my $num_reps = $TEST_STREAM_TIME_SEC 
                            * 1000000 * (scalar @TEST_STREAM) / $divisor;
        for (my $time_through_list = 0; $time_through_list < $num_reps;
                $time_through_list++) {

            my $start_time = [gettimeofday];

            for (my $i = 0; $i < scalar @TEST_STREAM; $i++) {
                my $line_ref = $TEST_STREAM[$i];
                $s->sendLine(join ',', @$line_ref);
#                my $first = $TEST_STREAM[$i][0];
#                my $second = $TEST_STREAM[$i][1];
#                $s->sendLine("$first, $second");

                # Sleep intervals of less than 2 clock ticks don't work.
                if ($tuple_interval > 
                        2 * $TICK_INTERVAL_USEC) {
                    usleep $tuple_interval;
                }
            }

            # How long should each run through the list take?
            my $time_per_list_sec = (scalar @TEST_STREAM) *
                $tuple_interval / 1000000;
            my $elapsed = tv_interval ($start_time, [gettimeofday]);
            my $remaining = $time_per_list_sec - $elapsed;
            if ($remaining > 0) {
                usleep($remaining * 1000000);
            }
        }

        print STDERR "Done sending tuples for stream '$stream'.\n";

        sleep 100; # TEMPORARY

        print STDERR "Closing connection for stream '$stream'.\n";

        $s->close();
        

        exit;

        # Shouldn't get here.
        die "Unreachable.";

    } elsif ($! =~ /No more process/) {
        # EAGAIN, supposedly recoverable fork error.
        sleep 5;
        redo FORK;
    } else {
        croak "Error forking stream source process.\n";
    }

}


################################################################################
# Subroutine that runs some SQL through psql.
#
# Arguments:	<sqlstr> is the SQL to run.
#sub dosql($sqlstr) {
sub dosql($) {
	my ($sqlstr,$cq) = @_;


	# Create a temporary file to feel through psql.
	open TEMPFILE, "> $SQL_TMP_FILE";
	print TEMPFILE $sqlstr;
	
	# Tell psql to quit nicely.
	print TEMPFILE "\n\\q\n";

	close TEMPFILE;

	# Run the script through psql.
	system "$PSQL_COMMAND -f $SQL_TMP_FILE";
}

################################################################################
# Subroutine that runs some SQL through psql in CQ mode.
#
# Arguments:	<sqlstr> is the SQL to run.
#sub dosql_cq($sqlstr) {
sub dosql_cq($) {
	my ($sqlstr,$cq) = @_;


	# Create a temporary file to feel through psql.
	open TEMPFILE, "> $SQL_TMP_FILE";
	print TEMPFILE $sqlstr;
	
	# Tell psql to quit nicely.
	print TEMPFILE "\n\\q\n";

	close TEMPFILE;

	# Run the script through psql.
	system "$STREAMING_PSQL_COMMAND -f $SQL_TMP_FILE";
}
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
# Subroutine that populates a table.  Specifically, the table is populated
# with integer tuples.
#
# Arguments:	<tabname> is the name of the table.
#		<numcols> is the number of columns of integers in the table.
#		<numrows> is the number of rows of data to generate.
#		<seed>	  is the integer we use to start the data generation.
################################################################################
#sub poptable($tabname, $numcols, $numrows, $seed) {
sub poptable($$$$) {
	my ($tabname, $numcols, $numrows, $seed) = @_;

	my $now = time;

	# Generate a comma-delimited text file containing the tuples.
	open DUMP, "> $TABLE_DUMP_TMP_FILE";
	
	for (my $i = 0; $i < $numrows; $i++) {
	
		# Generate the integer fields.
		for (my $j = 0; $j < $numcols; $j++) {
			my $num = ($seed + $i) * ($numcols - $j);
			print DUMP "$num";

			if ($j != $numcols - 1) {
			    print DUMP ",";
			} else {
			    print DUMP "\n";
			}
		}

		# Generate the timestamp field.
#		my $ts_int = $now - ($numrows/2) + $i;
#		my $ts_str = asctime(localtime($ts_int));
#		chomp $ts_str;
#		print DUMP "$ts_str\n";

	}
		

	close DUMP;

	# Load this file into the database.
	my $ddl = 
		"COPY $tabname FROM '$TABLE_DUMP_TMP_FILE' USING DELIMITERS ',';";
	dosql($ddl);
}

# Run the tests in real-time mode
sub run_real_time() {
    # Start up the data sources.
    print STDERR colored("\nStarting streams...\n", 'red');
    my @source_pids;
    foreach my $streamname (@TEST_STREAM_NAMES) {
        my $source_pid = start_stream_source("ss", $streamname);
        push @source_pids, $source_pid;
    }

    my $cli = new tcqclient($TEST_DB);

    # Start some queries to monitor dropped tuples, if requested.
    my @monitor_q_pids;
    if ($MONITOR_DROPS) {
        print STDERR colored("\nRunning monitoring queries...\n", 'red');
        my $KEPT_Q = "SELECT * FROM ss.__teststream1_kept_count",
        my $DROPPED_Q = "SELECT * FROM ss.__teststream1_dropped_count;",
#    my $DROPPED_Q = 
#	"SELECT * FROM ss.TESTSTREAM2;";

        my $MONITOR_NUM_ROWS = 500;

        my $kept_pid = 
            $cli->run_query_async(
                $KEPT_Q, "kept", $MONITOR_NUM_ROWS, 1, "/tmp/kept.log"); 
        my $dropped_pid = 
            $cli->run_query_async(
                $DROPPED_Q, "dropped", $MONITOR_NUM_ROWS, 1, "/tmp/drop.log");
            # Last two arguments are $show_results and $use_cq, respectively.

#    @monitor_q_pids = ($kept_pid, $dropped_pid);

        sleep 5;
    }

    # Run some queries.
    print STDERR colored("\nRunning queries...\n", 'red');
    foreach my $query (@QUERIES) {

        print STDERR colored("\n$query\n", 'yellow');
        dosql_cq($query);

        # Wait for things to settle down.
        sleep 2;

    }

    # Kill off the sources.
    foreach my $source_pid (@source_pids) {
        kill $source_pid;
    }
}

# Run the queries and streams in predictable batch mode.
sub run_batch() {
    # Clear out output directory.
    system "mkdir -p $BATCH_LOGFILE_DIR";
    system "rm -f $BATCH_LOGFILE_DIR/*";

    # Start all the queries
    my $cli = new tcqclient($TEST_DB);

    print STDERR colored "Starting queries...\n", 'red';
    my @query_pids;
    for (my $qid = 0; $qid < (scalar @QUERIES); $qid++) {

        # Our streams will start at Jan 1, 2000, so we need to add a 
        # START AT to the window clause
        my $query = $QUERIES[$qid];
        $query =~ s/\]/ start at '$BATCH_START_STR'\]/g;

        my $pid =     
            $cli->run_query_async($query, "Query $qid", $ROWS_PER_QUERY, 
                $SHOW_BATCH_QUERY_RESULTS, "$BATCH_LOGFILE_DIR/q${qid}.log"); 
        push @query_pids, $pid;
    }

    # Queries 0 and 1 will go to completion without any streams.  Wait until
    # at least one of them has completed before continuing.
    wait;
    sleep 2;
    print STDERR colored "Done starting queries...\n", 'red';

    # Generate the dumps, and send them to TCQ.
    print STDERR colored "Sending streams...\n", 'red';
    foreach my $streamname (@TEST_STREAM_NAMES) {
        my $dumpfile = gen_strm_dump("ss", $streamname);

        print STDERR "    $streamname\n";

        my $s = source2->new("localhost", 
            get_qual_stream_name("ss",$streamname));
        $s->open();
        $s->sendFile($dumpfile);
        $s->close();

#        sleep 2;
    }

    print STDERR colored "Waiting for queries to finish...\n", 'red';
    foreach my $pid (@query_pids) {
        POSIX::waitpid $pid, 0;
    }
}

###############################################################################
# BEGIN SCRIPT
###############################################################################

# Stop the postmaster, forcefully if necessary.
system "pg_ctl stop -m fast";
system "ipcclean";
unlink $PG_LOGFILE;

system "pg_ctl start -l $PG_LOGFILE";
sleep 2;

# Give the database server some time to start up.
print STDERR colored("Waiting a few seconds for postmaster to start",
	'red');
for (my $i = 0; $i < 3; $i++) {
	print STDERR colored(".", 'red');
	sleep 1;
}
print STDERR colored("Done.\n", 'red');

# Create some test tables.
print STDERR colored("\nCreating tables and streams...\n", 'red');
dosql($DROP_SCRIPT);
dosql($CREATE_SCRIPT);
print STDERR colored("Done creating tables and streams.\n", 'red');

# Put data in those tables.
print STDERR colored("\nPopulating tables...\n", 'red');
poptable("R", 1, 10, 5);
print STDERR colored("Done populating tables.\n", 'red');

# Restart the postmaster with all the TelegraphCQ doodads enabled.
unlink $PG_LOGFILE;
system "pg_ctl stop";
system "pg_ctl start -l $PG_LOGFILE -o \"-t $TEST_DB -u $USERNAME -G\"";
sleep 2;

if ($WAIT_FOR_DEBUGGER) {
    wait_for_return();
}

if ($MODE eq "rt") {
    run_real_time();
} elsif ($MODE eq "batch") {
    run_batch();
}

print STDERR colored("Done running queries.\n", 'red');
sleep 2;

# Stop the server.
system "pg_ctl stop";





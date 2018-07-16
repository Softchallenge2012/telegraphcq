#! /usr/bin/perl

################################################################################
# streamsanity.pl
#
# A script that tests whether the demo queries over TelegraphCQ
# streams are doing what they're supposed to.
#
# This script assumes the following:
#
#   The PATH variable contains the build of Postgres being tested
#   The database and user in the CONSTANTS section already exist.
#   The demo schema has been setup (run ./setup.sh)
#   The csv wrappers have been built and installed
# 
################################################################################

use strict;
use warnings;
use Term::ANSIColor;
	# Oooo!  Pretty colors!
use POSIX qw(asctime);
	# For generating timestamps.
use Carp;

# Turn off STDERR buffering.
select(STDERR); $| = 0; select(STDOUT);

#
# CONSTANTS
#

#die "usage: demosanity.pl <dbname> <username> <batch> <time>\n"
#	 unless 4 == scalar @ARGV;

die "usage: demosanity.pl <dbname> <username> <time>\n"
	unless 3 == scalar @ARGV;

# Name of our test database.
my $TEST_DB = shift;

# Username to use.
my $USERNAME = shift;

# Number of records for each query
#my $BATCH = shift;

# Max time for each query
my $TRUN = shift;

# How to run psql
#my $PSQL_COMMAND = "psql -C -i 1 -I $BATCH -d $TEST_DB ";
my $PSQL_COMMAND = "psql -C -i 1 -d $TEST_DB ";

# Demo queries
my @QUERIES =
  (
   "SELECT NOW() ;",
   
   "select stationid, speed, tcqtime  ".
   " from traffic.measurements        ;",
   
   "select tm.stationid, tm.speed, tm.tcqtime, ts.latitude, ts.longitude   " .
   " from   traffic.measurements as tm, traffic.stations as ts              " .
   " where  tm.stationid = ts.stationid                                    ;",

   "select ts.* ".
   " from traffic.incidents ;",

   "select ts.incidentID,ts.location,ti.highway,ti.street,ti.latitude,ti.longitude ".
   " from  traffic.intersections as ti, traffic.incidents as ts ".
   " where (ti.highway||ti.street) = (tint.highway||tint.street);",
   
  );

# Temporary file we use for transitory SQL scripts.
my $SQL_TMP_FILE = "scratch/dosql_tmp.sql";

# 
# SUBROUTINES
#

################################################################################
# Subroutine that starts the streamers 
#
# Returns the PID of the source for easy killing later on.
################################################################################
sub start_streamers() {

  my $pid;

  # Code copied from page 146 of the camel book.
 FORK:
  if ($pid = fork) {
	# Parent does nothing.
	return $pid;
	
  }
  elsif (defined $pid) {
	# Child starts streamers without NOHUP
	system("./streamers.sh");
	exit;
	
  } elsif ($! =~ /No more process/) {
	# EAGAIN, supposedly recoverable fork error.
	sleep 5;
	redo FORK;
  } else {
	croak "Error forking stream source process.\n";
  }
}

################################################################################
# Subroutine that runs some SQL through psql in the background.
#
# Arguments:	<sqlstr> is the SQL to run.
#               <uniq> is a uniq identifier to generate unique temp files
################################################################################
#sub dosql($sqlstr, $uniq) {
sub dosql($$) {
    my ($sqlstr,$uniq) = @_;

    my $pid;

    # Code copied from page 146 of the camel book.
  FORK:
    if ($pid = fork) {
		# Parent does nothing.

		print STDERR colored("\nStarted query in process $pid \n",'yellow');
		return $pid;
    }
    elsif (defined $pid) {

		# Open a pipe to psql.
		open PSQL, "| $PSQL_COMMAND -o $SQL_TMP_FILE.out.$uniq ";

		print PSQL "$sqlstr\n\\q\n";

		close PSQL;
		# perl's close() command does an implicit waitpid(2) on the child
		# process.  Note that the above line isn't technically necessary, as
		# the exit() below would implicitly close all open filehandles,
		# implicitly doing a waitpid...
		
		print STDERR colored("psql finished query $uniq.\n",'green');

		exit;
    } elsif ($! =~ /No more process/) {
		# EAGAIN, supposedly recoverable fork error.
		sleep 5;
		redo FORK;
    } else {
		croak "Error forking stream query process.\n";
    }
}

###############################################################################
# BEGIN SCRIPT
###############################################################################

# Stop the postmaster, forcefully if necessary.
system "pg_ctl stop";

# Restart the postmaster with all the TelegraphCQ doodads enabled.
system "pg_ctl start -l scratch/logfile -o \"-t $TEST_DB -u $USERNAME -G\"";

# Give the database server some time to start up.
print STDERR colored("Waiting a few seconds for postmaster to start",
	 'red');
for (my $i = 0; $i < 3; $i++) {
	 print STDERR colored(".", 'red');
	 sleep 1;
}
print STDERR colored("Done.\n", 'red');

# Start up the data sources.
my $source_pid = start_streamers();

# Wait for things to settle down.
print STDERR colored("Waiting a few seconds for streamers to start ",'blue');
for (my $i = 0; $i < 3; $i++) {
  print STDERR colored(".",'blue');
  sleep 1;
}
print STDERR "\n";
my $i = 0;

my @qpid;

foreach my $query (@QUERIES) {
  print STDERR colored("\n$query\n", 'yellow');
  $qpid[$i] = dosql($query,$i);
  $i++;
}
my $nqueries = $i;

# Wait for $TRUN seconds.
print STDERR colored("Let the queries run for $TRUN seconds \n",'blue');
for (my $i = 1; $i < $TRUN; $i++) {
  if (($i % 60) == 0) {
	 print STDERR colored(".\n",'red');
  }
  else {
	 print STDERR colored(".",'blue');
  }	
  sleep 1;
}

# Time to kill the queries
print STDERR colored("Time to kill the queries \n",'blue');

for (my $j = 0; $j < $nqueries; $j++) {
  print STDERR colored("Sending SIGKILL to kill process $qpid[$j]\n",'red');
  kill 'KILL', $qpid[$j];
}

print STDERR colored("Done running queries \n",'green');

print STDERR colored("Shutting down the experiment \n",'green');
system "./stop.sh";

print STDERR colored("All done ... time to inspect the damage \n",'green');

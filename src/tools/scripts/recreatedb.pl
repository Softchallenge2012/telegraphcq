#!/usr/bin/perl
#
# Please run this in the postgres init directory for mashah
# /home/mashah/research/install/bin otherwise unpredictable results may occur.
#
# ensure the PGDATA variable is set to /home/mashah/research/pgdata/data.

#usage recreatedb.pl <node_id>

use FileHandle;
use IPC::Open2;

$node_id = $ARGV[0];

$command = "rm -r /home/mashah/research/pgdb/";
print "Running: ", $command, "\n";
system( $command );

$command = "mkdir /home/mashah/research/pgdb/";
print "Running: ", $command, "\n";
system( $command );

$command = "./initdb";
print "Running: ", $command, "\n";
system( $command );

$command = "./postmaster";
print "Running: ", $command, "\n";
$pid = open2(*rhandle, *wrhandle, "$command");

print "Sleeping ... \n";
sleep(2);
print "Awake! \n";

$command = "./createdb test";
print "Running: ", $command, "\n";
system( $command );

$command = "./psql -t test -f create.qry";
print "Running: ", $command, "\n";
system( $command );

$replace = "s/telegraph_nodeid.*/telegraph_nodeid = $node_id/g";
$pgconfig = "/home/mashah/research/pgdb/data/postgresql.conf";

#print "Setting telegraph_nodeid to $node_id, using: $replace, on file: $pgconfig \n";
print "Setting telegraph_nodeid to $node_id \n";

system("changenodeid.pl", $node_id); # file defaults to $pgconfig
system("tail", $pgconfig);

$hbaconf = "/home/mashah/research/pgdb/data/pg_hba.conf";

open(HBAFILE, ">> $hbaconf");
print HBAFILE "host \tall \tall \t128.32.37.177 \t255.255.255.255 \ttrust \n";
close(HBAFILE);

print "Sleeping ... \n";
sleep(2);
print "Awake! \n";

# Fast shutdown
print "Killing the server: $pid \n";
system("pg_ctl", "stop", "-m", "f"); 

print "Done. Hope it worked. \n";

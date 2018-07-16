#!/usr/bin/perl
# changes the telegraph_nodeid field in the given postgres config file.
# usage: changenodeid <node_id> <config_file>

$node_id = shift @ARGV;
$pgconfig = shift @ARGV;

if ($pgconfig == "") {
    $pgconfig = "/home/mashah/research/pgdb/data/postgresql.conf";
}

system( "perl", 
	"-i.orig", 
	"-pe", 
	"s/telegraph_nodeid.*/telegraph_nodeid = $node_id/g",
	$pgconfig );

#!/usr/bin/perl
# copies a specified file from specified place to all machines in the cluster.

$orig = shift @ARGV;

print "Blah", $orig, "\n";

if ($orig eq "") {
    $orig = "/home/mashah/research/pgdb/data/pg_hba.conf";
}

for ($i = 2; $i <= 8;$ i++) {

    $dest = "telegraph$i:$orig";

    print "Copying from: $orig to $dest \n";
    system( "scp", $orig, $dest );

}

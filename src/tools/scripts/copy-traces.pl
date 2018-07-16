#!/usr/bin/perl

# Copies traces from machines and installs them here.

$file = shift @ARGV;

print "Copying files with prefix $file from telegraphM [2:7] with extension (M) \n";

for ($i = 2; $i <= 7; $i++) {

	$dest = "$file.$i";
	$orig = "telegraph$i:research/pgdb/$file";

    print "Copying from: $orig to $dest \n";
    system( "scp", $orig, $dest );

}

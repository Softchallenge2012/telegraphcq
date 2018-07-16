#!/usr/bin/perl

# Cleans the output from a run to print KTuple

# Gets time and throughput from the files for an FCons

while (<>) {
	s/ .* P-KTup\/sec: (.*), .* dur: (.*) ms /\2 \1/;
	print int($2/1000), " ", $1, "\n";
}

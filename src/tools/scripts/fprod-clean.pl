#!/usr/bin/perl

# Cleans the output from a run to print KTuple

# Gets time and throughput from the files for an FProd

while (<>) {
	s/ .* T-KTup\/sec: (.*), .* dur: (.*) ms /\2 \1/;
	print int($2/100)/10, " ", $1, "\n";
}

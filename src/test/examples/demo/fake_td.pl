#! /usr/bin/perl

# fake_td.pl
#
# By Fred Reiss
#
# A script that pretends to be tcpdump.  Reads tcpdump output from a text file
# called "tcpdump.out" and echos it to stdout at a constant rate.

use strict;
use warnings;
use Time::HiRes qw(usleep);

# CONSTANTS

my $PACKET_INTERVAL_USEC = 200000;
	# How many microseconds we wait between printing out lines of tcpdump
	# data.

# Loop forever.
for (;;) {
	open DUMP, "./tcpdump.out" or die "Can't open tcpdump dump file.\n";

	my $line;
	while ($line = <DUMP>) {
		print $line;
		usleep($PACKET_INTERVAL_USEC);
	}

	close DUMP;
}


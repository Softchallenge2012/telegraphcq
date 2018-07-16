#!/usr/bin/perl

################################################################################
#$RCSfile: newregress.pl,v $
#$Revision: 1.1 $
#$Date: 2006/02/14 22:34:44 $
################################################################################

use strict;
use warnings;

################################################################################
my $USAGE = <<USAGE_END;
newregress.pl

New TCQ regression suite

Usage:    newregress.pl <mode>

Where:
    <mode> is one of:
        -- "refresh": regenerate the canonical results.
        -- "run": run the regression against the current canonical results.

USAGE_END
################################################################################

use Term::ANSIColor;


################################################################################
# ARGUMENTS

if (1 != (scalar @ARGV)) {
    die $USAGE;
}

my ($MODE) = @ARGV;

if ($MODE ne "refresh" and $MODE ne "run") {
    die $USAGE;
}

################################################################################
# CONSTANTS

my $DBNAME = "tcqregress"; 

my $USERNAME = $ENV{"USER"};

my $RUNNING_DIR = $ENV{"PWD"};

# Where we get our "canonical" results from.
my $CANONICAL_IN = "$RUNNING_DIR/regress.tgz";

# Where we put the canonical results
my $CANONICAL_DIR = "/tmp/newregress";

# Where streamsanity.pl puts its query logfiles.
my $SS_RESULTS_DIR = "/tmp/streamsanity";

# Hash of queries that don't give repeatable output.
my %IGNORED_QUERIES = (
    0 => 1
);

################################################################################
# GLOBALS

################################################################################
# SUBROUTINES


################################################################################
############################################################
########################################
####################
# BEGIN SCRIPT
system "pg_ctl stop";
system "pg_ctl start -l regress.log";
sleep 5;
system "createdb $DBNAME";

# Run streamsanity
system "$RUNNING_DIR/streamsanity.pl $DBNAME $USERNAME batch";

if ($MODE eq "refresh") {
    chdir $SS_RESULTS_DIR;
    system "tar -czf $CANONICAL_IN *";

    print STDERR colored "Regenerated canonical results.\n", 'red';

} elsif ($MODE eq "run") {
    # Unpack the regression results.
    system "mkdir -p $CANONICAL_DIR";
    system "rm -f $CANONICAL_DIR/*";
    system "tar --directory $CANONICAL_DIR -xzf $CANONICAL_IN";

    print STDERR colored "Checking results...\n", 'red';

    # Compare the regression results against the canonical ones.
    opendir DIR, $SS_RESULTS_DIR 
        or die "Couldn't open directory $SS_RESULTS_DIR";

    # Skip over . and ..
    readdir(DIR); readdir(DIR);

    my @failed_queries;
    while (my $logfile = readdir(DIR)) {

        $logfile =~ /q(\d*).log/ or die "Don't understand filename '$logfile'";
        my $query_num = int $1;

        if (not defined $IGNORED_QUERIES{$query_num}) {        
#           print STDERR "Diffing $logfile...\n";

            system 
                "diff --brief $CANONICAL_DIR/$logfile $SS_RESULTS_DIR/$logfile";

            my $exit_value  = $? >> 8;

            if (0 != $exit_value) {
                push @failed_queries, $query_num;
            }
        }
    }

    closedir DIR;


    if (0 != (scalar @failed_queries)) {
        print STDERR "Failed regression test on the following queries:\n";
        foreach my $qnum (sort {$a <=> $b} @failed_queries) {
            print STDERR "    Query $qnum\n";
        }

        print STDERR <<END;
Try comparing logfiles in $SS_RESULTS_DIR with canonical results
located in $CANONICAL_DIR.  
END
        exit 1;
    }

    print STDERR colored "Success!\n", 'red';
} 

system "pg_ctl start -l regress.log";
sleep 5;
system "dropdb $DBNAME";
system "pg_ctl stop";

unlink "regress.log";



#!/usr/bin/perl

# A script that calls my cluster_run script to run things on a bunch of machines.

$usage = "mashah-queries.pl <query-name> <num-machines> \n";

if (scalar(@ARGV) != 2)
{
	print $usage;
	exit 1;
}

$query = $ARGV[0];
$nmach = $ARGV[1];

if ($query eq "one-level-src")
{
	$command = "cluster_run.pl all clustq.qry $nmach FP_PAIR_SCONS " .
		" FP_PAIR_SCONS_STDBY sourceq.qry FP_PAIR_SRC ";
} 
elsif($query eq "two-level-src")
{
	$command = "cluster_run.pl all clustq.qry $nmach FP_TWO_LEVEL " .
		" FP_TWO_LEVEL_STDBY sourceq.qry FP_PAIR_SRC ";
}
elsif($query eq "one-level-flux")
{
	$command = "cluster_run.pl all clustq.qry $nmach FP_PAIR_FLUX " .
		" FP_PAIR_FLUX_STDBY ";
}
elsif($query eq "two-level-grp")
{
	$command = "cluster_run.pl all twolvlgrp.qry $nmach FP_TWO_GROUPS " .
		" FP_TWO_GROUPS_STDBY clicksrc.qry FP_PAIR_SRC_CLICK ";
}
elsif($query eq "two-level-grp-push")
{
	$command = "cluster_run.pl all-push twolvlgrp.qry $nmach FP_TWO_GROUPS " .
		" FP_TWO_GROUPS_STDBY clicksrc.qry FP_PAIR_SRC_CLICK ";
}
elsif($query eq "three-level-grp")
{
	$command = "cluster_run.pl all threelvlgrp-full.qry $nmach FP_THREE_GROUPS " .
		" FP_THREE_GROUPS_STDBY tgroupsrc.qry FP_PAIR_SRC_TGROUPS ";
}
elsif($query eq "three-level-grp-push")
{
	$command = "cluster_run.pl all-push threelvlgrp-full.qry $nmach FP_THREE_GROUPS " .
		" FP_THREE_GROUPS_STDBY tgroupsrc.qry FP_PAIR_SRC_TGROUPS ";
}
elsif($query eq "four-level-grp")
{
	$command = "cluster_run.pl all fourlvlgrp.qry $nmach FP_THREE_GROUPS " .
		" FP_THREE_GROUPS_STDBY fgroupsrc.qry FP_PAIR_SRC_FGROUPS ";
}
elsif($query eq "four-level-grp-push")
{
	$command = "cluster_run.pl all-push fourlvlgrp.qry $nmach FP_THREE_GROUPS " .
		" FP_THREE_GROUPS_STDBY fgroupsrc.qry FP_PAIR_SRC_FGROUPS ";
}
elsif($query eq "web-log-qry-push")
{
	$command = "cluster_run.pl all-push web-log.qry $nmach FP_THREE_GROUPS " .
		" FP_THREE_GROUPS_STDBY web-log-src.qry FP_PAIR_SRC_WEBLOG ";
}
elsif($query eq "two-level-grp-egress-push")
{
	$command = "cluster_run.pl all-push twolvlgrpe.qry $nmach FP_TWO_GROUPS_EGRESS " .
		" FP_TWO_GROUPS_EGRESS_STDBY twolvlgrpe.qry FP_PAIR_SRC_AND_EGRESS_CLICK ";
}
elsif($query eq "two-level-grp-egress-push-lat")
{
	$command = "cluster_run.pl all-push twolvlgrpl.qry $nmach FP_TWO_GROUPS_EGRESS_LAT " .
		" FP_TWO_GROUPS_EGRESS_LAT_STDBY twolvlgrpl.qry FP_PAIR_SRC_EGRESS_LAT_CLICK ";
}
elsif($query eq "two-level-grp-egress-push-lat-double")
{
	$command = "cluster_run.pl all-push twolvlgrpl.qry $nmach DOUBLE_TWO_GROUPS_EGRESS " .
		" DOUBLE_TWO_GROUPS_EGRESS twolvlgrpl.qry DOUBLE_SRC_EGRESS_LAT_CLICK ";
}
elsif($query eq "two-level-grp-egress-push-lat-single")
{
	$command = "cluster_run.pl all-push twolvlgrpl.qry $nmach SINGLE_TWO_GROUPS_EGRESS " .
		" SINGLE_TWO_GROUPS_EGRESS twolvlgrpl.qry SINGLE_SRC_EGRESS_LAT_CLICK ";
}

print "Running: $command \n";

# We are done.
exec $command;

#!/usr/bin/perl

# Cleans the output from a run to print KTuple

# Gets time and throughput from the files for an FProd

use FileHandle;
use IPC::Open2;

$usage = "cluster_run.pl <nodeid> <query_file> \n" . 
    "cluster_run.pl all <query-file> <num_machines> " . 
	"<plan_q> <standby_q> [source-query-file <plan_code>] [PUSH] \n" .
    "<plan_q> and <standby_q> are plans in coverttofsched_dup \n" .
	" standby is on num_machines + 1 and source is on + 2 \n ".
    " Use all-push instead of all to create push based plan. ";

# Make sure we have the right number of arguements.
if ((scalar(@ARGV) != 2) && 
    (scalar(@ARGV) != 4) &&
    (scalar(@ARGV) != 5) &&
    (scalar(@ARGV) != 7))
{
    print "Only ", scalar(@ARGV), " arguments \n";
    print $usage;
    exit 1;
}

$node_id = $ARGV[0];
$q_file = $ARGV[1];

$qry = "";

#@machine_map = {1, 2, 3, 4, 5, 6, 7, 8};
@machine_map = (2, 3, 4, 5, 6, 7, 8, 1);
#@machine_map = (3, 4, 5, 6, 7, 8, 1, 2);

open(QFILE, "< $q_file");
while (<QFILE>)
{
	$qry = $qry . $_;
}
chomp($qry);
#remove the semicolon;
$qry =~ s/;//g;
close(QFILE);

print "Query file: $q_file \n";
print "Query base: $qry \n";

if ($node_id =~ /^[0-9]$/) {

    $psql_handle[$node_id] = open_psql( $node_id, $q_file );

#    $psql_handle[$node_id] = $fhandle;

    $rhandle = $psql_handle[$node_id];

    while(<$rhandle>) {
		print $hostname, ": ", $_;
    }

    close($rhandle);
    
    print "Done.\n ";

} elsif (($node_id eq "all") || ($node_id eq "all-push")) {

    $num_machines = $ARGV[2];
    $fplan = $ARGV[3];

	if ($node_id eq "all")
	{
		$myquery = "$qry declust[$num_machines, '$fplan'];";
	}
	else
	{
		$myquery = "$qry declust[$num_machines, '$fplan', 'PUSH'];";
	}

#    for ($i = 1; $i <= $num_machines; $i++) {
    for ($i = 0; $i < $num_machines; $i++) {

# Side effect of subroutine
		print "Iteration: $i \n";

		($psql_r_handle[$i], $psql_w_handle[$i]) = 
			open_psql( $machine_map[$i], $myquery );

# Send the child our query
		$psql_alive[$i] = 1;
		$psql_hostname[$i] = $hostname;

    }

    # Create a standby machine if told to do so.
    if (scalar(@ARGV) >= 5) {

		$stand_by = $ARGV[4];

		if ($stand_by ne "none") {

			if ($node_id eq "all")
			{
				$myquery = "$qry declust[$num_machines, '$stand_by'];";
			}
			else
			{
				$myquery = "$qry declust[$num_machines, '$stand_by', 'PUSH'];";
			}

			print "Iteration: $i \n";

			($psql_r_handle[$i], 
			 $psql_w_handle[$i]) = open_psql( $machine_map[$i], $myquery );

			$psql_alive[$i] = 1;
			$psql_hostname[$i] = $hostname;
			$i++; # Must increment.
		}
    }

    if (scalar(@ARGV) >= 7) {

# fire off the source query
		$sq_file = $ARGV[5];
		$splan   = $ARGV[6];

		open(SQFILE, "< $sq_file");
		$sqry = "";
		while(<SQFILE>)
		{
			$sqry = $sqry . $_;
		}
		chomp($sqry);
#remove the semicolon;
		$sqry =~ s/;//g;
		close(SQFILE);

		$squery = "$sqry declust[$num_machines, '$splan'];";
	
		($psql_r_handle[$i],
		 $psql_w_handle[$i]) = open_psql( $machine_map[$i], $squery );

		$psql_alive[$i] = 1;
		$psql_hostname[$i] = $hostname;
		$i++; # Must increment.
    }

# Rotate through each file descriptor and print the lines
    print "prompt> ";

    while($line = <STDIN>) {
		print $line "\n";
    }
#    $num_done = 0;
#   $i = 1;

#    while ($num_done != $num_machines) {

#	if ($psql_alive[$i] == 1) {

#	    $rhandle = $psql_handle[$i];

#	    print "Getting line from $i \n";

#	    if ($line = <$rhandle>) {
#		print $psql_hostname[$i], ": ", $line;
#	    } else {
#		$psql_alive[$i] = 0;
#	    }
#	}

#	$i = ($i + 1);

#	if ($i == $num_machines) {
#	    $i = 1;
#	}
#    }
#    print "Can't handle all at the moment \n";

    print "Done. \n";

} else {

    print $usage;

}

sub open_psql {

    $node_id = shift @_;
    $aqry = shift @_;

    $hostname = "telegraph$node_id";
    
    $command = "psql -C -t test -h $hostname";
    
#    $command = "cat ~/.psql_history";
    print "Running command: $command \n";

#    open($handle, "$command |")
#    open(BLAH, "$command |")
#	or die "Cannot run program: $command \n";
#    open($handle, "-|") || exec $command;

    open2(*RHANDLE, *WHANDLE, "$command");

    print "Running on node: \t $node_id \t query: $aqry \n";

    print WHANDLE $aqry, "\n";

    return RHANDLE, WHANDLE;
}

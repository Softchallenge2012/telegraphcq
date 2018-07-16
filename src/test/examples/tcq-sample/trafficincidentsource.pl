#!/usr/bin/perl
use FileHandle;
use Socket;

###
# by emach 
# Combines cidrgen.pl and sensorsource.pl.
#  - cidrgen.pl puts input in a file called "incidents"
#  - sensorsource.pl actually sends it to TCQ.
#   - combine these steps so file is written, but keep track of latest ID. Only send those tuples after the latestID. The reason for this is the TCQ is buggy and cannot accept duplicate tuples at this time.
###

$usage = "trafficincidentsource.pl host port sourcename duration [-fs sensor_freq]
 where
    duration = time (in minutes) the script should run for (< 0 = run forever, 0 = run once)
    host = The name or IP address of the TelegraphCQ host to connect to
    port = The port number on which the TCQ host is listening
    sourcename = The name of the TCQ stream to push data into
    -fs = time (in minutes) between updates of the sensor-counts file

";

if ($#ARGV<=2) {
	print "Missing one or more parameters.\n Usage: $usage";
	exit;
}
$host = $ARGV[0];
$port = $ARGV[1];
$sourcename = $ARGV[2];
$dur = $ARGV[3] * 60;
$sens = 60;



if ($dur < 0) { $forever = 1; }
else {$forever = 0; }

$i=3;
for $f (@ARGV) {
$i++;
if ($f eq "-h" || $f eq "--help") { print "Usage: $usage"; exit; }
if ($f eq "-fs") { $sens = $ARGV[$i] * 60; }
}
if ($sens <= 0) { $sens=60; }

if ($forever) {
	print "Duration=forever\n"
} else {
	print "Duration=$dur seconds\n"
}

print "Traffic incidents update freq=$sens seconds\nIncident update frequency=$inc seconds\n";
$rem_sens = 0;
$rem_inc = 0;
#open the socket
$iaddr = inet_aton($host);
$paddr = sockaddr_in($port,$iaddr);
$proto = getprotobyname('tcp');

print "sourcename = $sourcename, length = ".length($sourcename)."\n";


socket(SOCK, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK, $paddr) || die "connect: $!";
autoflush SOCK,1;
$namestr = sprintf("%4d%s\x0",length($sourcename)+1,$sourcename);

printf "sending: $namestr";

print SOCK $namestr;

$latestincidentnum = 0;

while ($dur >= 0 || $forever == 1) {
	    $rem_inc -= $rem_sens;
	    $dur -= $rem_sens;
	    sleep $rem_sens;
	    print("Fetching incidents-counts");
	    if ($forever) {
		  print ("(looping forever)\n");
	    } else {
		    print("(time remaining = ".($dur/60)." minutes)\n");
            }
	    
	    # emach. use screen scraper to put information in file "incidents"
	    system("java -classpath \".:tess.jar:gnu-regexp-1.0.8.jar\" doTess TrafficUpdate.jsc \"&\" > incidents");
	    #system("gawk -f add_time.awk incidents > incidents-counts");
	    print ("Done.\n");
	    print ("Sending data to TelegraphCQ.\n");
	    open(INCIDENTS, "incidents") || die "Can't open incidents data file: $!\n";
	    $count=0;
	    while (<INCIDENTS>) {
		#parse the line
		($incidentnum,$date,$location,$city,$description) = split(/,/);
		#output it again (Why did I parse it in the first place? I don't know)
		$datelength = strlength($date);
		$incIDlength = "0004";
		$loclength = strlength($location);
		$citylength = strlength($city);
		$desclength = strlength($description);
		$str = "$date" . "," . 
		    "$incidentnum" . "," . 
		    "$loclength" . "," . 
		    "$location" . "," .
		    "$citylength" . "," . 
		    "$city" . "," .
		    "$desclength" . "," .
		    "$description" .
		    "\n"; 
		## emach. NEED TO FIX HERE.
		# Send only the LATEST TUPLES.
                # but keep track of latest ID. 
		# Only send those tuples after the latestID. 
		# The reason for this is the TCQ is buggy and cannot accept duplicate tuples at this time.
		#print "incidentnum = $incidentnum";
		$resultsent = alreadysent($incidentnum);
		#print "resultsent = $resultsent\n";
		if ($resultsent == 0){
		    print ("sending : $str lENGTH:" . length($str) . "\n");
		    print(SOCK  $str);
		    $latestincidentnum = $incidentnum;
		    
		    $count++;
		}
		#print "done checking already sent";
	    }
	    close(INCIDENTS);
	    print ("Done sending to TelegraphCQ.  $count tuples sent\n");
	    $rem_sens = $sens;
	}


print ("Closing connection to TelegraphCQ.\n");
close(SOCK);

exec 


###
# emach
# One line of code copied from the cidrgen.pl file.
# EFFECTS: 
#   - Screen scrapes some government site and puts it in a file "incidents". 
#   - 
#
###


###
# emach
# alreadysent checks the hash $sentincidentnums to see if $ARG
# has already been sent to TCQ.
##

    $sentincidentnums = {};

sub alreadysent {

    my $arg = shift;
    
    #print "in alreadysent $arg\n";

    if ($sentincidentnums->{$arg}) {
	#print "in here";
	return 1;
    } else {
	#print " in has not been sent";
	# has not been sent
	$sentincidentnums->{$arg} = true;
	return 0;
    }

}


sub strlength {

    my $arg = shift;

    $arglength = length($arg);
    
    #print "THIS THE THE LENGTH: $arglength";

    if ($arglength > 999) {
	return $arglength;
    } elsif ($arglength > 99) {
	return "0" . $arglength;
    } elsif ($arglength > 9) {
	return "00" . $arglength;
    } else {
	return "000" . $arglength;
    }
}

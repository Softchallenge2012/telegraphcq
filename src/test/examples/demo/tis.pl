#!/usr/bin/perl
use FileHandle;
use Socket;
use strict;
use parseCHP;

###
# by emach 
#
# Combines cidrgen.pl and sensorsource.pl.
#  - cidrgen.pl puts input in a file called "incidents"
#  - sensorsource.pl actually sends it to TCQ.
#  - combine these steps so file is written, but keep track of latest
#    ID.  Only send those tuples after the latestID. The reason for
#    this is the TCQ is buggy and cannot accept duplicate tuples at this
#    time.
#
# scratch/incidents is a temporary file used by the doTess code.
# scratch/incidents-log are all the incidents logged from the website. 
#   - Each line should be unique.
#
my $usage = "tis.pl host port sourcename duration [-fs sensor_freq]
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
my $host = $ARGV[0];
my $port = $ARGV[1];
my $sourcename = $ARGV[2];
my $dur = $ARGV[3] * 60;
my $sens = 60;
my $forever;
my $sendincnum = {};


if ($dur < 0) { $forever = 1; }
else {$forever = 0; }

my $i=3;
for my $f (@ARGV) {
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

print "Traffic incidents update freq=$sens seconds\nIncident update frequency=$dur seconds\n";
my $rem_sens = 0;
my $rem_inc = 0;
#open the socket
my $iaddr = inet_aton($host);
my $paddr = sockaddr_in($port,$iaddr);
my $proto = getprotobyname('tcp');

print "sourcename = $sourcename, length = ".length($sourcename)."\n";


socket(SOCK, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK, $paddr) || die "connect: $!";
autoflush SOCK,1;
my $namestr = sprintf("%4d%s\x0",length($sourcename)+1,$sourcename);

printf "sending: $namestr";

print SOCK $namestr;

my $latestincidentnum = 0;

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
    system("java -classpath \".:tess.jar:gnu-regexp-1.0.8.jar\" doTess TrafficUpdate.jsc \"&\" > scratch/incidents");

    print ("Done.\n");
    print ("Sending data to TelegraphCQ.\n");
    open(INCIDENTS, "scratch/incidents") || die "Can't open incidents data file: $!\n";
    my $count=0;

    while (<INCIDENTS>) {
	#parse the line
	chop;
	my ($incidentnum,$date,$location,$city,$description) = split(/,/);
	
	# store in data struct to be sent later.
	$sendincnum->{$incidentnum}->{location} = $location;
	$sendincnum->{$incidentnum}->{city}= $city;
	$sendincnum->{$incidentnum}->{descr}= $description;
	$sendincnum->{$incidentnum}->{date}= $date;
	#$sendincnum->{$incidentnum}->{sent} = 0;
	
    }
    close(INCIDENTS);

    ## Send in order...
    foreach my $incident (sort keys %{$sendincnum}) {

	    
	my $resultsent = $sendincnum->{$incident}->{sent}; #alreadysent($incident);
	if ($resultsent == 0){
	    #print "RESULTSENT: $resultsent \n\n";
	    $sendincnum->{$incident}->{sent} = 1; # true.
	    print "------------------------------------------------------------\n";
	    
	    printf "processing incident $incident\n";
	    my $location = $sendincnum->{$incident}->{location};
	    my $city = $sendincnum->{$incident}->{city};
	    my $description = $sendincnum->{$incident}->{descr};
	    my $date = $sendincnum->{$incident}->{date};
	    
	    printf "calling parselocation $location\n";
	    
	    my ($st1,$st2) = parseLocation($location);
	    
	    printf "called parselocation ($st1,$st2) \n";
	    
	    next if (!defined($st1) || !defined($st2));
	    
	    # The following was added because the new method of geocoding
	    # uses Fwy###, the old one uses Fwy ### with a space.  
	    
	    my $fwy_num;
	    my $fwy_pre;
	    if( $st1 =~ /fwy[0-9]/i ) {
		($fwy_num = $st1) =~ s/fwy//ig;
		($fwy_pre = $st1) =~ s/[0-9]//g;
		$st1 = "$fwy_pre $fwy_num";
	    }
	    
	    if( $st2 =~ /fwy[0-9]/i ) {
		($fwy_num = $st2) =~ s/fwy//ig;
		($fwy_pre = $st2) =~ s/[0-9]//g;
		$st2 = "$fwy_pre $fwy_num";
	    }
	    printf "Hwy: ($st1), Str: ($st2) \n";
	    
	    my $str =
		"$incident" . "," .
		"$location" . "," .
		"$city" . "," .
		"$description" . "," .
		"$st1" . "," .
		"$st2" . "," .
		"$date" .
		"\n";
	    print ("sending : $str lENGTH:" . length($str) . "\n");
	    print(SOCK  $str);
	    
	    open(INCIDENTSLOG, ">>scratch/incidents-log") || die "Can't open incidents-log file";
	    print INCIDENTSLOG $str;
	    close (INCIDENTSLOG);

	    $count++;
	}
    }
    
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




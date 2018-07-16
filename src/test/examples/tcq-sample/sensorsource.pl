#!/usr/bin/perl
use FileHandle;
use Socket;

$usage = "sensorsource.pl host port sourcename duration [-fs sensor_freq]
 where
    duration = time (in minutes) the script should run for (< 0 = run forever, 0 = run once)
    host = The name or IP address of the TelegraphCQ host to connect to
    port = The port number on which the TCQ host is listening
    wrappername = The name of the TCQ wrapper to push data into
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

print "Sensors update freq=$sens seconds\nIncident update frequency=$inc seconds\n";
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



while ($dur >= 0 || $forever == 1) {
	    $rem_inc -= $rem_sens;
	    $dur -= $rem_sens;
	    sleep $rem_sens;
	    print("Fetching sensor-counts");
	    if ($forever) {
		  print ("(looping forever)\n");
	    } else {
		    print("(time remaining = ".($dur/60)." minutes)\n");
            }
	    system("wget -q http://www.dot.ca.gov/traffic/d4/update.txt -O update.txt");
	    system("gawk -f add_time.awk update.txt > sensor-counts");
            system("cat sensor-counts >> sensor-counts-log");
	    print ("Done.\n");
	    print ("Sending data to TelegraphCQ.\n");
		open(SENSOR, "sensor-counts") || die "Can't open sensor data file: $!\n";
	    $count=0;
		while (<SENSOR>) {
		  #parse the line
		  ($date, $stationid, $speed) = split(/,/);
		  #output it again (Why did I parse it in the first place? I don't know)
		  $str = "$date,$stationid,$speed\n";
		  print ("sending : $str \n");
		  print(SOCK  $str);
		  $count++;
		}
		close(SENSOR);
		print ("Done sending to TelegraphCQ.  $count tuples sent\n");
		$rem_sens = $sens;
	}


print ("Closing connection to TelegraphCQ.\n");
close(SOCK);

exec 

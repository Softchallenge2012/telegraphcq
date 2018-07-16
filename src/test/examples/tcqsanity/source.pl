#!/usr/bin/perl
use FileHandle;
use Socket;

$usage = "source.pl host port sourcename 
 where
    host = The name or IP address of the TelegraphCQ host to connect to
    port = The port number on which the TCQ host is listening
    wrappername = The name of the TCQ wrapper to push data into

";

if ($#ARGV<2) {
	print "Missing one or more parameters.\n Usage: $usage";
	exit;
}
$host = $ARGV[0];
$port = $ARGV[1];
$sourcename = $ARGV[2];




for $f (@ARGV) {
$i++;
if ($f eq "-h" || $f eq "--help") { print "Usage: $usage"; exit; }
}

#open the socket
$iaddr = inet_aton($host);
$paddr = sockaddr_in($port,$iaddr);
$proto = getprotobyname('tcp');



socket(SOCK, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK, $paddr) || die "connect: $!";
autoflush SOCK,1;
$namestr = sprintf("%4d%s\x0",length($sourcename)+1,$sourcename);
print SOCK $namestr;

while (<STDIN>) {
    $str = $_;
    print(SOCK  $str);
    $count++;
}
close(SOCK);


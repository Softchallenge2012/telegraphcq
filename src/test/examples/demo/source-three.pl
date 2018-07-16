#!/usr/bin/perl
use FileHandle;
use Socket;

# Signal handler for broken pipes (e.g. the wretch doesn't like us)
sub sigpipe_handler {
    die <<'END_MSG';
Error: Wrapper clearinghouse has closed the connection.
       Did you spell the stream name correctly?
END_MSG
}

$SIG{'PIPE'} = 'sigpipe_handler';

$usage = "source.pl host port sourcename 
 where
    host = The name or IP address of the TelegraphCQ host to connect to
    port = The port number on which the TCQ host is listening
    wrappername = The name of the TCQ wrapper to push data into

";

if ($#ARGV != 4) {
    print $#ARGV;
	print "Missing one or more parameters.\n Usage: $usage";
	exit;
}
$host = $ARGV[0];
$port = $ARGV[1];

$sourcename1 = $ARGV[2];
$sourcename2 = $ARGV[3];
$sourcename3 = $ARGV[4];


for $f (@ARGV) {
$i++;
if ($f eq "-h" || $f eq "--help") { print "Usage: $usage"; exit; }
}

#open the socket
$iaddr = inet_aton($host);
$paddr = sockaddr_in($port,$iaddr);
$proto = getprotobyname('tcp');

socket(SOCK1, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK1, $paddr) || die "connect: $!";
autoflush SOCK1,1;
$namestr = sprintf("%4d%s\x0",length($sourcename1)+1,$sourcename1);
print SOCK1 $namestr;

socket(SOCK2, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK2, $paddr) || die "connect: $!";
autoflush SOCK2,1;
$namestr = sprintf("%4d%s\x0",length($sourcename2)+1,$sourcename2);
print SOCK2 $namestr;

socket(SOCK3, PF_INET, SOCK_STREAM, $proto) || die "Couldn't open socket: $!";
connect(SOCK3, $paddr) || die "connect: $!";
autoflush SOCK3,1;
$namestr = sprintf("%4d%s\x0",length($sourcename3)+1,$sourcename3);
print SOCK3 $namestr;

my $biasmiddle = 1;

while (<STDIN>) {
    $str = $_;
    print($str);
    if($count%3 == 0) {
	  print(SOCK1  $str);
    }
	else {
	  if($count%3 == 1) {
		for (my $i = 0; $i < $biasmiddle; $i++) {
		  print(SOCK2  $str);
		}
	  } else {
		print(SOCK3  $str);
	  }
	}
    $count++;
  }
close(SOCK1);
close(SOCK2);
close(SOCK3);


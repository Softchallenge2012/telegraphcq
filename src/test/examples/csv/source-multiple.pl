#!/usr/bin/perl
use FileHandle;
use IO::Socket::INET;

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

$host = $ARGV[0];
$port = $ARGV[1];

$numsources = $ARGV[2];

for($i = 0; $i < $numsources; $i++)
{
    $sourcename[$i] = $ARGV[$i+3];
}
print "$numsources \n";

#open the sockets
$iaddr = inet_aton($host);
$paddr = sockaddr_in($port,$iaddr);
$proto = getprotobyname('tcp');

for($i = 0; $i < $numsources; $i++)
{
    $sockets[$i] = new IO::Socket::INET->new(PeerPort=>$port, Proto=>'tcp', PeerAddr=>$host);
    $namestr = sprintf("%4d%s\x0",length($ARGV[$i+3])+1,$ARGV[$i+3]);
    $sockets[$i]->send($namestr);
}


while (<STDIN>) {
    $str = $_;
    print($str);
    $sockets[$count%$numsources]->send($str);
    $count++;
}

for($i = 0; $i < $numsources; $i++)
{
    close($sockets[$i]);
}


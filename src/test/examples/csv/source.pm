#!/usr/bin/perl

################################################################################
# perl module version of source.pl.  Provides a simple way of making a
# wrapper, based on the CSV wrapper.
#
# Usage:
#
# my $s = source->new(<host>, <port>, <wrappertype>, <schema>, <stream>)
#
# $s->open();
#
# $s->sendLine($line);
# $s->sendList(@list);
# $s->sendListRef(\@list);
#
# $s->close();
################################################################################

use strict;
use warnings;

use FileHandle;
use Socket;
use IO::Socket::INET;
use Time::HiRes qw( usleep gettimeofday tv_interval );
use POSIX;
use Carp;
use pretty;

package source;

################################################################################
# CONSTANTS
################################################################################

# Possible states for a wrapper clearinghouse connection
my $STATE_CLOSED = 0;
my $STATE_OPEN = 1;


################################################################################
# CONSTRUCTOR
#
# Arguments:
#       <class> is passed by the Perl interpreter.
#       <host> and <port> are where the TCQ server is running.
#       <wrappertype> is the name of the wrapper instance.
#       <schema> is the schema in which the stream was created
#       <stream> is the name of the stream to attach to.
################################################################################
#sub new($class, $host, $port, $wrappertype, $schema, $stream)
sub new($$$$) {
    my ($class, @param) = @_;

    # Perl voodoo.  Apparently, this creates the class instance, while
    # allowing for inheritance.
    my $self = {};
    bless $self, ref($class) || $class;

    $self->_initialize(@param);

    return $self;
}

# The subroutine that actually does the initialization.
#sub _initialize($self, $host, $port, $wrappertype, $schema, $stream) {
sub _initialize($$$$$$) {
    my ($self, $host, $port, $wrappertype, $schema, $stream) = @_;

    $self->host($host);
    $self->port($port);
    $self->wrappertype($wrappertype);
    $self->schema($schema);
    $self->stream($stream);

    $self->state($STATE_CLOSED);
}

################################################################################
# ACCESSORS
#
# Note that most of these are "private".
################################################################################

# Generic accessor function.
sub _genacc  { # ($self, $val, $name)
    my $self = shift @_;
    my $name = pop @_;

    if (@_) {
	$self->{$name} = shift @_;
    }
    return $self->{$name};
}

sub host {  _genacc(@_, 'host'); }
sub port {  _genacc(@_, 'port'); }
sub wrappertype {  _genacc(@_, 'wrappertype'); }
sub schema {  _genacc(@_, 'schema'); }
sub stream {  _genacc(@_, 'stream'); }
sub state {  _genacc(@_, 'state'); }

sub sock {  _genacc(@_, 'sock'); }

################################################################################
# METHODS
################################################################################

# Subroutine that opens a socket to the wrapper clearinghouse.  Dies if it is
# not successful.
sub open($) {
    my ($self) = @_;

    die "Tried to open a WCH connection twice" 
        unless ($STATE_CLOSED == $self->state);

    $self->sock(IO::Socket::INET->new(
			Proto    => "tcp",
			PeerAddr => $self->host,
			PeerPort => $self->port))
		  or die "Cannot connect to port " . $self->port 
                            . " on " . $self->host;


    $self->sock->autoflush(1);

    # Note that we convert the stream name to lowercase, to get around a bug
    # in the beta.
    my $wrapperid_str = $self->wrappertype . "," . $self->schema . "." .
                        lc($self->stream);
    my $namestr = sprintf("%4d%s\x0",length($wrapperid_str)+1, $wrapperid_str);

#    print "Namestr is '$namestr'\n";

    $self->sock->print($namestr);

    $self->state($STATE_OPEN);
}


# Subroutine that sends a formatted line of CSV.  Argument may or may not have
# a carriage return.
sub sendLine($$) {
    my ($self, $line) = @_;

    die "Socket not open" unless defined $self->sock;

    chomp $line;

    $self->sock->print("$line\n");

}

sub sendListRef($$) {
    my ($self, $list_ref) = @_;

    my $line = join ',', @$list_ref;

    $self->sendLine($line);
}

sub sendList($@) {
    my ($self, @list) = @_;

    my $line = join ',', @list;

    $self->sendLine($line);
}


# Send a hashtable as two-element tuples.
sub sendHash($$) {
    my ($self, $hash_ref) = @_;

    foreach my $key (keys %$hash_ref) {
        my $val = $hash_ref->{$key};
        my $line = "$key,$val";

        $self->sendLine($line);
    }
}



sub close($) {
    my ($self) = @_;

    Carp::confess "Tried to close a WCH connection twice" 
        unless ($STATE_OPEN == $self->state);

    my $sock = $self->sock;

    $sock->close();

    $self->state($STATE_CLOSED);
}

# Play back a file of timestamped tuples at a specified multiple of
# real-time.  Opens a connection if necessary but does not close the
# connection.
#
# Returns the average amount by which we were late in delivering a tuple.
#
# Arguments:
#       <filename> is the location of the trace fle.
#       <speedup> is a multiplicative factor by which to speed up playback
#       <timelimit> is the maximum number of real-time seconds to play 
#               back for, or -1 to go until the end of the file.
sub playbackFile($$$$) {
    my ($self, $filename, $speedup, $timelimit) = @_;

    if (not defined $timelimit) {
        $timelimit = -1;
    }

    # We verify the file before trying to open a connection.
    CORE::open TRACE, "$filename" or die "Couldn't open $filename";

    my @filename_parts = split '/', $filename;
    my $short_filename = pop @filename_parts;

    if ($STATE_CLOSED == $self->state) {
        $self->open();
    }

    # Read the timestamp off the first line to set our starting point.
    my $first_line = <TRACE>;
    chomp $first_line;
    my @first_line = split ',', $first_line;
    my $first_ts_str = $first_line[$#first_line];
    my $start_sec = parse_pg_timestamp($first_ts_str);

    my $start_time = [Time::HiRes::gettimeofday];
    my $elapsed_sec = 0.0;

    my $target_sec;
    my $line = $first_line;
    my $numtups = 0;
    my $tot_extra_delay_sec = 0.0;
    do {
        my @line = split ',', $line;
        my $ts_str = pop @line;
        my $ts = parse_pg_timestamp($ts_str);
        my $offset_sec = $ts - $start_sec;
        
        # Apply the speedup multiplier
        $target_sec = $offset_sec / $speedup;
#        print STDERR "Target elapsed time is $target_sec\n";


        # Generate some feedback for the user.
        my $status_str = sprintf("Playing back %s: %6.2f/%6.2f sec", 
            $short_filename, $target_sec, $timelimit);
        if (-1 != $timelimit) {
            pretty::status_graph($status_str, $target_sec, $timelimit);
        } else {
            pretty::status_inflight($status_str);
        }



        $elapsed_sec = Time::HiRes::tv_interval($start_time, 
                                [Time::HiRes::gettimeofday]);

        if ($elapsed_sec > $target_sec) {
            $tot_extra_delay_sec += $elapsed_sec - $target_sec;
        } else {
            # Sleep until the correct offset has happened.
            my $sleep_usec = 1e6 * ($target_sec - $elapsed_sec);
#            print STDERR "Sleeping $sleep_usec usec.\n";
            Time::HiRes::usleep($sleep_usec);
        }

        # Whew!  Now we can send the line to the wrapper clearinghouse.
        $self->sendList(@line);
        $numtups++;

    } while (
        defined ($line = <TRACE>)
        and (-1 == $timelimit
            or $target_sec < $timelimit));

    pretty::status_done("Done playing back file '$filename'");

    CORE::close TRACE;

    return ($tot_extra_delay_sec / $numtups);
}

################################################################################
# PRIVATE FUNCTIONS

# Parse a Postgres timestamp into the (# of seconds) format that Perl
# understands.
#
# NOTE: Appends fractional seconds.
sub parse_pg_timestamp($) {
    my ($ts_str) = @_; 
    my ($year, $mon, $mday, $hour, $min, $sec, $sec_frac);

    # Timestamp format:  2005-05-11 13:02:36.15137-07
    #                    or 2005-05-11 13:02:36
    if ($ts_str =~ /(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d):(\d\d)\.(\d+)/)
    {
        ($year, $mon, $mday, $hour, $min, $sec, $sec_frac) = 
           ($1,   $2,   $3,    $4,    $5,   $6,   $7);
    } elsif ($ts_str =~ /(\d\d\d\d)-(\d\d)-(\d\d) (\d\d):(\d\d):(\d\d)/) {
        ($year, $mon, $mday, $hour, $min, $sec, $sec_frac) = 
           ($1,   $2,   $3,    $4,    $5,   $6,   0.0);
    } else {
        die "Can't parse timestamp '$ts_str'";
    }

    $sec_frac = "0.$sec_frac";    
    $year -= 1900;

    # Convert 
    my $time_t = POSIX::mktime($sec, $min, $hour, $mday, $mon, $year);

    $time_t += $sec_frac;

    return $time_t;
}


# Perl modules need to return 1...
1;



################################################################################
# tcqserver.pm 
#
# A perl module for starting and stopping the TCQ server.
#
# Assumes that the PGDATA environment variable is set properly.
################################################################################

use strict;
use warnings;

package tcqserver;

use Term::ANSIColor;
use Time::HiRes qw( usleep );
use Cwd;


################################################################################
# CONSTANTS
################################################################################

die "PGDATA not set" unless defined $ENV{"PGDATA"};
my $PGDATA = $ENV{"PGDATA"};


################################################################################
# CONSTRUCTOR
#
# Arguments:
#       <class> is passed by the Perl interpreter.
#       <dbname> is the name of the database instance.
################################################################################
#sub new($class, $dbname) {
sub new {
    my ($class, @param) = @_;

    # Perl voodoo.  Apparently, this creates the class instance, while
    # allowing for inheritance.
    my $self = {};
    bless $self, ref($class) || $class;

    $self->_initialize(@param);

    return $self;
}

# The subroutine that actually does the initialization.
#sub _initialize($self, $dbname) {
sub _initialize($$) {
    my ($self, $dbname) = @_;

    $self->dbname($dbname);

    $self->silent(0);
    $self->logfile("./logfile");

    my %hash;
    $self->pg_conf_params(\%hash);
}

################################################################################
# ACCESSORS
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

# The database instance
sub dbname { _genacc(@_, 'dbname'); }

# Should we suppress error messages?
sub silent { _genacc(@_, 'silent'); }

# Location of the logfile.
sub logfile { _genacc(@_, 'logfile'); }

# Parameters that are passed to Telegraph via postgresql.conf
sub pg_conf_params { _genacc(@_, 'pg_conf_params'); }

################################################################################
# METHODS
################################################################################

################################################################################
# Subroutine that sets the value we will use for a given postgresql.conf 
# parameter the next time we start TCQ.
sub set_param_val($$$) {
    my ($self, $param, $val) = @_;

    $self->pg_conf_params->{$param} = $val;
}

sub unset_param_val($$) {
    my ($self, $param) = @_;
    delete $self->pg_conf_params->{$param};
}

################################################################################
# Subroutine that stops the server nicely.  Does not return until the server
# is stopped.
sub stop_nicely($) {
    my ($self) = @_;

    $self->run_cmd("pg_ctl stop");
    $self->restore_postgresql_conf();
}

################################################################################
# Subroutine that stops the server less nicely and tries to clean up the mess.  
# Does not return until the server is stopped.
sub stop_forcefully($) {
    my ($self) = @_;

    $self->run_cmd("pg_ctl stop -m fast");
    my $orig_silent = $self->silent;
    $self->silent(0);
    $self->run_cmd("ipcclean");
    $self->silent($orig_silent);
    $self->restore_postgresql_conf();
}

################################################################################
# Subroutine that starts the server in Postgres mode.
sub start_pg($) {
    my ($self) = @_;

    my $logfile = $self->logfile;

    $self->edit_postgresql_conf();

    $self->run_cmd("pg_ctl start -l $logfile");
    $self->wait_for_dbstart(0);
}

################################################################################
# Subroutine that starts the server in streaming mode with eddies.
sub start_normal($) {
    my ($self) = @_;
    my $logfile = $self->logfile;
    my $dbname = $self->dbname;
    my $username = $ENV{"USER"};
    $self->edit_postgresql_conf();
    $self->run_cmd(
        "pg_ctl start -l $logfile -o \"-t $dbname -u $username -G\"");
    $self->wait_for_dbstart(1);
}

################################################################################
# Subroutine that starts the server in streaming mode without eddies.
sub start_noeddies($) {
    my ($self) = @_;
    my $logfile = $self->logfile;
    my $dbname = $self->dbname;
    my $username = $ENV{"USER"};
    $self->edit_postgresql_conf();
    $self->run_cmd(
        "pg_ctl start -l $logfile -o \"-t $dbname -u $username\"");
    $self->wait_for_dbstart(1);
}

################################################################################
# Subroutine that gets rid of the logfile.
sub remove_logfile($) {
    my ($self) = @_;

    my $logfile = $self->logfile;
    unlink $logfile;
}

################################################################################
# PRIVATE METHODS
#
# Used internally.
################################################################################

################################################################################
# Run a command, squelching output if $self->silent is set.
sub run_cmd($$) {
    my ($self, $cmd) = @_;

    if ($self->silent) {
        system "$cmd 1>&2 2>/dev/null";
    } else {
        system "$cmd 1>&2";
    }
}


################################################################################
# Subroutine that waits for TelegraphCQ to start up.
#
# Assumes that you're using a fresh logfile.
#
# Argument:
#       <tcq_mode> is 1 if we're in TCQ mode, and 0 for postgres mode.
#sub wait_for_dbstart($tcq_mode, $logfilename) {
sub wait_for_dbstart($$$) {
    my ($self, $tcq_mode) = @_;

    my $logfilename = $self->logfile;

    my $orig_buf;
    if (not $self->silent) {
        # Turn off buffering. 
        select STDERR; 
        $orig_buf = $|;
        $| = 1; 
        select STDOUT;

        print STDERR "Waiting for database to start.";
    }

    # Wait until the logfile contains an "okay" message.
    for (my $i = 0; $i < 1000000; $i++) {
        if (-e $logfilename) {
            open LOGFILE, "$logfilename";

            my $line;
            while ($line = <LOGFILE>) {

                # Check for failures.
                if ($line =~ /signal 6/ || $line =~ /signal 11/) {
                    die "Server died";
                }

                # Check for success.
                if ((0 == $tcq_mode && $line =~ /database system is ready/)
                    || $line =~ /TelegraphCQ processes successfully started/) {
                    if (not $self->silent) {
                        print STDERR "\n";
                        $| = $orig_buf;
                    }
                    return;
                }
            }
            close LOGFILE;
        }

        if (not $self->silent) {
            print STDERR ".";
        }

        usleep(100000); 
    }
  
}



################################################################################
# Subroutine that checks to see whether the current postgres process has gone
# away.  Returns the PID of the process if it is active, 0 otherwise.
sub server_pid($) {
    my ($self) = @_;

    my $pgdata = $ENV{"PGDATA"};

    my $pglockfile = "$pgdata/postmaster.pid";

    # Look for the previous process's lockfile
    if (-e $pglockfile) {
        open LOCKFILE, $pglockfile or die "Couldn't open $pglockfile";
        my $pid = <LOCKFILE>;
        close LOCKFILE;

        chomp $pid;

        return $pid;

    } else {
        return 0;
    }
}



################################################################################
# Subroutine that undoes edit_postgresql_conf()
################################################################################
sub restore_postgresql_conf($) {
    my ($self) = @_;

    if (-e "$PGDATA/orig_postgresql.conf") {
        system "mv $PGDATA/orig_postgresql.conf $PGDATA/postgresql.conf";
    }

}

################################################################################
# Subroutine to edit postgresql.conf.  Puts in place all the substitions in
# $self->pg_conf_params{}.
#sub edit_postgresql_conf($self) {
sub edit_postgresql_conf($) {
    my ($self) = @_;

    # SPECIAL CASE: We were previously interrupted before we could restore
    # postgresql.conf.
    if (-e "$PGDATA/orig_postgresql.conf") {
        system "mv $PGDATA/orig_postgresql.conf $PGDATA/postgresql.conf";
    }
    # END SPECIAL CASE
    
    system "mv $PGDATA/postgresql.conf $PGDATA/orig_postgresql.conf";

    open CONFIN, "$PGDATA/orig_postgresql.conf" 
        or die "Couldn't open postgresql.conf for reading";
    open CONFOUT, "> $PGDATA/postgresql.conf" 
        or die "Couldn't open postgresql.conf for writing";
    
    my $params_href = $self->pg_conf_params;

    my $line;
    while ($line = <CONFIN>) {
        chomp $line;

        foreach my $param (keys %$params_href) {
            if ($line =~ /$param/) {
                my $val = $params_href->{$param};
                print CONFOUT "$param = $val\n";
            } else {
                print CONFOUT "$line\n";
            }
        }
    }

    close CONFOUT;
    close CONFIN;

}


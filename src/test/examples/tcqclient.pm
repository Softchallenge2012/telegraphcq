
################################################################################
# tcqclient.pm 
#
# A perl module for interacting with TCQ/postgres as a client.
#
# Assumes that the PGRSHOME environment variable is set correctly.
################################################################################

use strict;
use warnings;

use POSIX ":sys_wait_h";

package tcqclient;

use DBI;

################################################################################
# CONSTANTS
################################################################################

# How to run psql
die "PGRSHOME environment variable not set" unless defined $ENV{"PGRSHOME"};
my $PGRSHOME = $ENV{"PGRSHOME"};

my $PSQL = "$PGRSHOME/bin/psql";

my $USER = $ENV{"USER"};
my $SQL_TMP_FILE = "/tmp/tcqclient_${USER}_tmp.sql";



################################################################################
# CONSTRUCTOR
#
# Arguments:
#       <class> is passed by the Perl interpreter.
#       <dbname> is the name of the database instance.
################################################################################
#sub new($class, $timestamp, $sub, $arg) {
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

# Should we suppress error messages and query output?
sub silent { _genacc(@_, 'silent'); }

################################################################################
# METHODS
################################################################################

################################################################################
# Subroutine that runs a file of SQL commands through psql.
#
# Arguments:	<filename> is the file.
#sub dosql_file($self,$filename)
sub dosql_file($$$) {
    my ($self, $filename) = @_;
    my $dbname = $self->dbname;

    my $psql_command = "$PSQL -d $dbname --quiet -f $filename";
    if ($self->silent) {
        system "$psql_command 2>&1 1>/dev/null";
    } else {
        system "$psql_command";
    }
}



################################################################################
# Subroutine that runs some SQL through psql.
#
# Arguments:	<sqlstr> is the SQL to run.
#sub dosql($self,$sqlstr) {
sub dosql($$) {
    my ($self, $sqlstr) = @_;


    # Create a temporary file to feel through psql.
    maketmp($SQL_TMP_FILE, "$sqlstr\n\\q\n");
        # The trailing "\q" tells psql to quit nicely.
        # TODO: Is this necessary?

    # Run the script through psql.
    $self->dosql_file($SQL_TMP_FILE);
}


################################################################################
# Subroutine that runs some SQL through postgres via DBI, then returns the 
# result as an arrayref.
sub dosql_dbi($$) {
    my ($self, $query) = @_;

    my $dbname = $self->dbname;

    my $dbh = DBI->connect("dbi:Pg:dbname=$dbname")
        or die "Error connecting to database: $DBI::errstr\n";
  
    my $arrayref = $dbh->selectall_arrayref($query);

    $dbh->disconnect();

    return $arrayref;
}

################################################################################
# Subroutine that spawns a child to run a query.
#
# Arguments:
#       <query> is a SQL query to run.
#       <query_name> is a human-readable nickname for the query.
#       <num_rows> is the number of rows to fetch of the query results.
#       <show_results> is 1 if you want to display the results for the query
#               in an xterm.
#       <logfile> is the name of the file into which we should tee results, or
#               undef if we shouldn't send results anywhere.
#
# Returns the PID of the child.
#
#sub run_query_async($self, $query, $query_name, $num_rows, $show_results
#                   $logfile) {
sub run_query_async($$$$$$) {
    my ($self, $query, $query_name, $num_rows, $show_results, $logfile) = @_;

    my $dbname = $self->dbname;

    my $query_name_no_spaces = $query_name;
    $query_name_no_spaces =~ s/ /_/g;
    my $tmpfile = "/tmp/${query_name_no_spaces}_tmp.sql";

    my $logfile_str = "";
    if (defined $logfile) {
        if (1 == $show_results) {
            $logfile_str = "| tee $logfile";
        } else {
            $logfile_str = "> $logfile";
        }
    } 

    my $child_pid = fork;
    if (0 == $child_pid) {
        # Child for this query.
        maketmp($tmpfile, "$query\n");
        my $commandline;
#        if (1 == $use_cq) {
            $commandline = 
                "$PSQL -C -I $num_rows -d $dbname -f $tmpfile $logfile_str";
#        } else {
#            $commandline = "$PSQL -d $dbname -f $tmpfile";
#        }


        if (1 == $show_results) {
            # Sleep after running the query, so we can see the last error 
            # message.
#            $commandline .= "; sleep $TERMINAL_CLOSE_DELAY";
            my $xterm_cmd = "xterm -T '$query_name query' -e '$commandline'; "
                . "/bin/sleep 5";
            print STDERR "Running $xterm_cmd\n";
            system $xterm_cmd;
        } else {
            print STDERR "Running '$commandline'\n";
            system $commandline;
        }

        if (1 == $show_results) {
            print "$query_name query done.\n";
        }

        # 
#        print "$query_name query waiting for children.\n";
        wait;
#        print "$query_name query completely done.\n";
#        unlink $tmpfile;
        exit;
    }

    return $child_pid;
}


################################################################################
# PRIVATE METHODS
#
# Used internally.
################################################################################


################################################################################
# Subroutine that creates a temp file with the indicated text.
#
#sub maketmp($filename, $contents) {
sub maketmp($$) {
    my ($filename, $contents) = @_;

    open TMP, "> $filename" or die "Couldn't open temp file '$filename'";
    print TMP $contents;
    close TMP;

}


# Voodoo to make the module compile.  
return 1;



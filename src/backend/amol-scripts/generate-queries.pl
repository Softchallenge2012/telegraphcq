@tablenames = ('hundredk', 'fiftyk', 'tenk', 'onek');
@sizes = (100000, 50000, 10000, 1000);

# two arguments to choose the size (0 to sizeof(tablenames)), two arguments to choose the number (1 to 4)
sub getforeignkeypredicate 
{
    die "size mismatch" if $_[0] > $_[1];
    $retval = "$tablenames[$_[0]]$_[2].unique2";
    $retval .= " % $sizes[$_[1]]" unless $_[0] == $_[1];
    $retval .= " = $tablenames[$_[1]]$_[3].unique2";
    return $retval;
}

# two arguments to choose the size (0 to sizeof(tablenames)), two arguments to choose the number (1 to 4)
sub getjoinpredicate 
{
    $retval = "$tablenames[$_[0]]$_[2].unique2";
    if($sizes[$_[0]] != $sizes[$_[1]] / $_[4])
    {
        $retval .= "%" . $sizes[$_[1]] / $_[4];
    }
    $retval .= "=$tablenames[$_[1]]$_[3].unique2";
    if($_[4] != 1)
    {
        $retval .= "%" . $sizes[$_[1]] / $_[4];
    }

    return $retval;
}

sub getrandomlydistributedselection 
{
    return "$tablenames[$_[0]]$_[1].unique1<" . int(rand($sizes[$_[0]]));
}

sub gettimecorrelatedselection 
{
    $start = $end = int(rand($sizes[$_[0]]));
    $dummy_end = int(rand($sizes[$_[0]]));
    if($start > $dummy_end) {
        $start = $dummy_end;
    } else {
        $end = $dummy_end;
    }
    return "$start<$tablenames[$_[0]]$_[1].unique2\n$tablenames[$_[0]]$_[1].unique2<$end";
}

# generate a query
# first argument is the number of relations in the query
# the second and third arguments specify the kinds of joins and the shape in the query somehow
sub generatequery 
{
    my @randsizes, @randnums;
    my @selectionpredicates, @joinpredicates;
    my $ref1, $ref2; # we are going to use pass-by-reference

    my $numrelations = $_[0];
    $ref1 = $_[1]; # first relation in a join; this is an array
    $ref2 = $_[2]; # second relation in a join; this is also an array

    for($i = 0; $i < $numrelations; $i++) {
        @randsizes = (@randsizes, int(rand(4)));
        @randnums = (@randnums, 1 + int(rand(4)));
    }


    my $numselectionpredicates = 0;
    for($i = 0; $i < $numrelations; $i++)
    {
        if(rand(1) < .5) {
            if(rand(1) < .5) {
                @selectionpredicates = (@selectionpredicates, gettimecorrelatedselection($randsizes[$i], $randnums[$i]));
                $numselectionpredicates += 2;
            } else {
                @selectionpredicates = (@selectionpredicates, getrandomlydistributedselection($randsizes[$i], $randnums[$i]));
                $numselectionpredicates += 1;
            }
        }
    }

    # $@$ref1 ===> $ = sizeof, @ = array, $ = the variable ref1 is a reference. @$ will convert it to an array.
    my $numjoinpredicates = 0;
    for($i = 0; $i < @$ref1; $i++) {
        @joinpredicates = (@joinpredicates, 
                getjoinpredicate($randsizes[@$ref1[$i]], $randsizes[@$ref2[$i]], $randnums[@$ref1[$i]], $randnums[@$ref2[$i]], 1)
                );
        $numjoinpredicates++;
    }

    die "ERROR: The number of join predicates does not correspond to the number of relations" if $numjoinpredicates != $numrelations - 1;

    print "$numjoinpredicates\n"; # join predicates
    print "$numselectionpredicates\n"; # selection predicates

    print join("\n", @joinpredicates) . "\n";

    for($i = 0; $i < $numrelations; $i++) {
        print "$tablenames[$randsizes[$i]]$randnums[$i]";
        $num = 0;
        for($j = 0; $j < @$ref1; $j++) {
            $num++ if ((@$ref1[$j] == $i) || (@$ref2[$j] == $i));
        }
        print " $num ";
        for($j = 0; $j < @$ref1; $j++) {
            print " $j " if ((@$ref1[$j] == $i) || (@$ref2[$j] == $i));
        }
        print "\n";
    }

    print join("\n", @selectionpredicates) . "\n";
}

srand($ARGV[0]);

@joinrelations1 = (0, 0, 0);
@joinrelations2 = (1, 2, 3);

generatequery(4, \@joinrelations1, \@joinrelations2);

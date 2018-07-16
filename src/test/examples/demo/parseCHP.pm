package parseCHP;

#
# parseCHP.pm
#
# Routines to parse address strings from CHP into 
# two streets, and back.
#

require Exporter;

@ISA = qw(Exporter);

@EXPORT = qw(parseLocation abbr parseStreets parseDir);

use String::Scanf;



				# 
				# Define abbreviations
				# 
%abbr = qw(COUNTY 0 FROM 0 TO 0 NEAR 0 AT 0 JEO 0 JNO 0 JSO 0 JWO 0 EB 0 NB 0 WB 0 SB 0 ON 0 OFR 0 OC 0 ONR 0 O/C 0 O/ 0 FWY 0 X 0 TO 0 E 0 W 0 N 0 S 0 HWY 0 BYPASS 0);

#
# parseLocation
#
# Parse location string into two street names
#
sub parseLocation {

  my @entries = split(/\s+/,$_[0]);
  my $word;
  my ($fwyName,$pref,$crsName);

  while ( $#entries > -1 ) {
	$word = shift @entries;
	if ( (defined $abbr{$word}) ) {
	  # This is a known abbreviation, ignore
	  next;
	}
	elsif ( $word !~ /[A-Z,a-z]/ ) {
	  # This is a number, ignore because we
	  # don't understand numbered addresses
	  next;
	}
	elsif ( $word =~ /^SR|^I|^FWY|^HWY|^US|^RT/ && $word=~/[0-9]$/ ) {
	  # This is a main street, add it.
	  ($pref,$rtNum) = sscanf('%[^0-9]%d',$word);
	  $rtNum = abs($rtNum);		# Sometimes it's I8, sometimes I-8.
	  if ( ! (defined $fwyName) ) {
		$fwyName = addSuffix($rtNum);
	  }
	  else {
		$crsName = addSuffix($rtNum);
		return( $fwyName, $crsName );
	  }
	}
	elsif ( defined $crsName ) {
	  $crsName .= " $word";
	}
	elsif (  defined $fwyName ) {
	  # Freeway id has to come first, otherwise go to next
	  $crsName = $word;
	}
  }								# while
    
  if ( defined $fwyName && defined $crsName ) {
	if ( $crsName =~ /^[0-9]/ && $crsName =~ /[0-9]$/ ) {
	  $crsName = addSuffix($crsName);
	}
	return( $fwyName,$crsName );
  }
    
  # 
  # No match was found at this
  # point.  Try the "# AT ____" format
  # 
  undef $fwyId;
  undef $crsName;
  @entries = split(/\s+/,$_[0]);
  if ( $entries[0] =~ /^[0-9]/ && $entries[0] =~ /[0-9]$/ &&
	   ($entries[1] =~ /^AT$|^X$|^AND$/ || 
		($entries[1] =~ /^J/ && $entries[1] =~ /O$/) ) ) {
    
	$fwyName = addSuffix($entries[0]);
	# Now get the second street
	if ( $entries[2] =~ /^SR|^I|^RT|^FWY|^US/ &&
		 $entries[2] =~ /[0-9]/ ) {
	  # Convert to a number
	  ($pref,$crsName) = sscanf('%s%d',$entries[2]);
	  $crsName = addSuffix($crsName);
	}
	elsif ($entries[2] =~ /^[0-9]/ && $entries[2] =~ /[0-9]$/ ) {
	  $crsName = addSuffix($entries[2]);
	}
	else {
	  $crsName = join(" ",@entries[2..$#entries]);
	}
	return ($fwyName,$crsName);
  }
  else {
	return;
  }
}

    
    
#
# addSuffix
#
# Addes "ST," "TH," "RD," or "ND" to numbered freeways.
# This is a hack for ArcView, because it will try to match numbers to 
# them.
#
sub addSuffix {
    return "FWY" . $_[0];
}

#
# parseStreets
#
# Parse the cross streets from the ArcView format file.
#
sub parseStreets {
    my @entries  = split(/\&/,$_[0]);
    if( $#entries >= 1 ) {
	return @entries[0..1];
    } else {
	return;
    }
}

#
# parseDir
#
# Parse a LOCATION string to get the direction and freeway.
#
sub parseDir {
    my @entries = split(/\s+/,$_[0]);
    my $i;
    my $fwy=-1;
    my $word = '';
    my $cand = '';
    $word = shift @entries;
    if( $word =~ /NB|SB|WB|EB/i  ) {
				# The first word must be the direction.
				# Otherwise, it's not on the freeway.
	$cand = shift @entries;
	($rtPrefix = $cand) =~ s/[0-9]+$//ig;
	($fwy=$cand) =~ s/^[a-z]+//ig;
	if( uc($rtPrefix) =~ /SR|I|US/)  {
	    return ($word,$fwy);
	}
    }
    return;
}

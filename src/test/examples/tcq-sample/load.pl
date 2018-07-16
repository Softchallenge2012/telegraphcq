#!/usr/bin/perl

# Little script to load up the database.

my $DBNAME = "test";

my $PSQL = "psql -C -d $DBNAME";

system "$PSQL -f measurements.sql";
system "$PSQL -f stations.sql";


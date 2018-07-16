#!/bin/sh
#                        -*-mode:ksh-*-
# Assumes that PGDATA is setup correctly

SEC=4
case $# in
  0) 
	 PGDIR=$PGDATA;
	 DB=sample;;
  1)
	 PGDIR=$1
	 DB=sample;;
  2) 
	 PGDIR=$1
	 DB=$2;;
  *) echo 'Usage: $0 [PGDIR] [database]';;
esac

echo "Setting up demo script in database $DB";

if [ -z "$GEOSERVERPORT" ]
then
    GEOSERVERPORT=6754
fi
echo "GEOSERVERPORT is $GEOSERVERPORT";

echo "Current status of TelegraphCQ"
pg_ctl status -D $PGDIR 
echo "Shutting down TelegraphCQ";
pg_ctl stop -D $PGDIR
echo "Sleeping $SEC seconds";
sleep $SEC;
echo "Starting up TelegraphCQ";
pg_ctl start -l $PGDIR/logfile -D $PGDIR
echo "Sleeping $SEC seconds";
sleep $SEC;

echo "Running the setup scripts";

echo "Setting up the schemas"
psql -f schema.sql $DB
echo "Setting up the wrappers"
cat wrap.sql | sed s/TCQGEOSERVERPORT/$GEOSERVERPORT/g >wrap.sql.out
psql  -f wrap.sql.out $DB
echo "Setting up the streams"
psql -f streams.sql $DB
echo "Setting up the tables"
psql -f tables.sql $DB
echo "Loading data in tables"
./load.sh $DB
echo "Perform mopping up operations"
#psql -f mop.sql $DB
echo "Shutting down TelegraphCQ";
pg_ctl stop -D $PGDIR 
echo "Current status of TelegraphCQ"
pg_ctl status -D $PGDIR

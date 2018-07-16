#!/bin/sh
#
# Assumes that PGDATA is setup correctly

export PATH=${PGRSHOME}/bin:${PATH}
SEC=4
PGDIR=$PGDATA
case $# in
    0) DB=sample;;
    1) DB=$1;;
    *) echo 'Usage: $0 [database]';;
esac


echo "Setting up demo script in database $DB";

echo "Current status of TelegraphCQ"
pg_ctl status
echo "Shutting down TelegraphCQ";
pg_ctl stop
echo "Sleeping $SEC seconds";
sleep $SEC;
echo "Starting up TelegraphCQ";
pg_ctl start -l $PGDIR/logfile
echo "Sleeping $SEC seconds";
sleep $SEC;

echo "Running the cleanup scripts";

psql -f clean.sql $DB

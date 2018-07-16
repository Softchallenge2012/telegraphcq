#!/bin/sh
#
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

echo "Current status of TelegraphCQ"
pg_ctl status -D $PGDIR
echo "Shutting down TelegraphCQ";
pg_ctl stop -D $PGDIR
echo "Sleeping $SEC seconds";
sleep $SEC;
echo "Starting up TelegraphCQ";
pg_ctl start -l $PGDIR/logfile -D $PGDIR -o "-i -t $DB -u $USER -G "
#pg_ctl start -l $PGDIR/logfile -o "-i -t $DB -u $USER -G -y -x -z"

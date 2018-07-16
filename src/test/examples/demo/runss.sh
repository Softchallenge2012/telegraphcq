#!/bin/sh

if [ -z "$TCQHOST" ]
then 
    HOST=localhost
else 
    HOST="$TCQHOST"
fi

if [ -z "$TCQWCHPORT" ]
then 
	PORT=5533
else 
	PORT="$TCQWCHPORT"
fi

WRAP=csvwrapper
SSCHEMA=traffic
STREAM=measurements
NAME="$WRAP,$SSCHEMA.$STREAM"

if [ $# -eq 0 ]
then
    echo "Running command:"
#    echo "ss.pl $HOST $PORT $NAME -1 > scratch/nohup.ss.out 2> scratch/nohup.ss.err "
#    ./ss.pl $HOST $PORT $NAME -1 > scratch/nohup.ss.out 2> scratch/nohup.ss.err 
    echo "ss.pl $HOST $PORT $NAME -1 2> scratch/nohup.ss.err "
    ./ss.pl $HOST $PORT $NAME -1  2> scratch/nohup.ss.err 
elif [ $1 = "nohup" ]
then  
    echo "Running command:"
    echo "nohup ss.pl $HOST $PORT $NAME -1 > scratch/nohup.ss.out 2> scratch/nohup.ss.err &"
    nohup ./ss.pl $HOST $PORT $NAME -1 > scratch/nohup.ss.out 2> scratch/nohup.ss.err &
else
    echo "Don't understand arg"
    echo "Usage: runss.sh [nohup]"
fi


#./ss.pl $HOST $PORT $NAME -1

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
STREAM=incidents
NAME="$WRAP,$SSCHEMA.$STREAM"

if [ $# -eq 0 ]
then
    echo "Running command:"
    echo "tis.pl $HOST $PORT $NAME -1 > scratch/nohup.is.out 2> scratch/nohup.is.err &"
    ./tis.pl $HOST $PORT $NAME -1 > scratch/nohup.is.out 2> scratch/nohup.is.err &
elif [ $1 = "nohup" ]
then  
    echo "Running command:"
    echo "nohup tis.pl $HOST $PORT $NAME -1 > scratch/nohup.is.out 2> scratch/nohup.is.err &"
    nohup ./tis.pl $HOST $PORT $NAME -1 > scratch/nohup.is.out 2> scratch/nohup.is.err &
else
    echo "Don't understand arg"
    echo "Usage: runis.sh [nohup]"
fi
#./tis.pl $HOST $PORT $NAME -1


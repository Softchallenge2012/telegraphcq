#!/bin/bash
#
# Send a signal to processes matching specified string
#
# Usage
#
#   ssig.sh pattern [signal]
#


case $# in
    1) PATTERN=$1;SIGNAL=9;;
    2) PATTERN=$1;SIGNAL=$2;;
    *) echo "Usage: $0 pattern signal";exit 1;;
esac

if [ `uname` ==  "Darwin" ]
then
   PSOPT="-ex -o pid,command"
   PIDCOL=1
else
   PSOPT="-fu "
   PIDCOL=2 
fi
echo "Finding processes with pattern $PATTERN to send signal $SIGNAL"

for p in `ps $PSOPT $USER | grep $PATTERN | grep -v $0 | grep -v grep  | awk "{print \\$$PIDCOL}"`
do
   echo "Sending signal $SIGNAL to process matching ($PATTERN) with pid ($p)"
   kill -s $SIGNAL $p
done


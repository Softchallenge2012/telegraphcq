#!/bin/sh

if [ -z "$TCQHOST" ]
then 
    HOST=localhost
else 
    HOST="$TCQHOST"
fi

echo $INTERFACE

if [ -z "$INTERFACE" ]
then
   INTERFACE=eth1
fi
if [ -z "$TCQWCHPORT" ]
then 
    PORT=5533
else 
    PORT="$TCQWCHPORT"
fi

WRAP=csvwrapper
SSCHEMA=network
STREAM=tcpdump
NAME="$WRAP,$SSCHEMA.$STREAM"

if [ $# -eq 0 ]
then
    echo "Running command:"
    echo "td $INTERFACE | ./sep | grep -v tcpdump | tee -a scratch/tcpdump-log | ./source.pl $HOST $PORT $NAME > scratch/nohup.td.out 2> scratch/nohup.td.err "
    ./td $INTERFACE | ./sep | grep -v tcpdump | tee -a scratch/tcpdump-log | ./source.pl $HOST $PORT $NAME > scratch/nohup.td.out 2> scratch/nohup.td.err 
elif [ $1 = "nohup" ]
then  
    echo "Running command:"
    echo "nohup td $INTERFACE | ./sep | grep -v tcpdump | tee -a scratch/tcpdump-log | ./source.pl $HOST $PORT $NAME > scratch/nohup.td.out 2> scratch/nohup.td.err &"
    nohup ./td $INTERFACE | ./sep | grep -v tcpdump | tee -a scratch/tcpdump-log | ./source.pl $HOST $PORT $NAME > scratch/nohup.td.out 2> scratch/nohup.td.err &
else
    echo "Don't understand arg"
    echo "Usage: runtcp.sh [nohup]"
fi


#td | sep | tee -a scratch/tcpdump-log | source.pl $HOST $PORT $NAME 

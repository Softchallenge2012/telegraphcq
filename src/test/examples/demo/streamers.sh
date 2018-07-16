#!/bin/sh

if [ $# -eq 0 ]
then
    NHP=" "
elif [ $1 = "nohup" ]
then  
    NHP="nohup"
else
    echo "Don't understand arg"
    echo "Usage: streamers.sh [nohup]"
fi


echo "Starting streaming source for traffic.incidents"
./runis.sh $NHP
echo "Starting streaming source for traffic.measurements"
./runss.sh $NHP
echo "Starting streaming source for network.tcpdump"
./runtcp.sh $NHP
#echo "Starting the geoserver"
#./rungeoserver.sh $NHP
echo "Starting the WebQueryServer"
./runwebqueryserver.sh $NHP


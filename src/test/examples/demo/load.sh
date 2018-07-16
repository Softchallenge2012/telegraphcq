#!/bin/bash

case $# in
    0) DB=sample;;
    1) DB=$1;;
    *) echo 'Usage: $0 [database]';;
esac

PWD=`pwd`
TSNAME="$PWD/init.txt"
psql -c "COPY traffic.stations from '$TSNAME' using delimiters ','" $DB
TINAME="$PWD/04-inter-latlong.csv"
psql -c "COPY traffic.intersections from '$TINAME' using delimiters ','" $DB


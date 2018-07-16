#!/bin/bash

PGRSHOME=`pg_config --bindir`
PGRSHOME=`dirname ${PGRSHOME}`
if [ -z "$GEOSERVERPORT" ]
then 
    GEOSERVERPORT=6754
fi

if [ -z "$GEOSERVERPATH" ]
then 
    GEOSERVERPATH="${PGRSHOME}/bin"
else 
    GEOSERVERPATH="$GEOSERVERPATH"
fi

if [ ! -x $GEOSERVERPATH/geoserver ] 
then 
    echo "the geoserver path $GEOSERVERPATH is invalid"
    exit 1
fi

OUTDIR=`pwd`/scratch
if [ $# -eq 1 ] 
then 
    COMMAND="nohup" 
else
    COMMAND=" "
fi

echo "Running command:"
COMMAND1="pushd $GEOSERVERPATH"
COMMAND2="$COMMAND ./geoserver -p $GEOSERVERPORT  > ${OUTDIR}/nohup.geoserver.out 2> ${OUTDIR}/nohup.geoserver.err &"
echo $COMMAND1
echo $COMMAND2
eval $COMMAND1
eval $COMMAND2

#!/bin/bash

PGRSHOME=`pg_config --bindir`
PGRSHOME=`dirname ${PGRSHOME}`
OUTDIR=`pwd`/scratch
if [ $# -eq 1 ] 
then 
    COMMAND="nohup" 
else
    COMMAND=" "
fi

echo "Running command:"
COMMAND1="pushd ${PGRSHOME}/bin"
COMMAND2="$COMMAND ./runwqs.sh   > ${OUTDIR}/nohup.webqueryserver.out 2> ${OUTDIR}/nohup.webqueryserver.err &"
echo $COMMAND1
echo $COMMAND2
eval $COMMAND1
eval $COMMAND2

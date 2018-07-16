#!/bin/sh
#         -*- mode:ksh -*-
#
if [ $# == 0 ] ; then
  echo "Usage: gencpfromdir.sh dirs"
  exit 1;
fi
CP=' '
for j in $*
do
  JARS=`ls $j/*.jar`
  CP=`echo ${JARS} | sed 's/[ ]/:/g'`:$CP
done

echo $CP


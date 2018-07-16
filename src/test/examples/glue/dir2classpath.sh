#!/bin/sh
#         -*- mode:ksh -*-
#
if [ $# == 0 ] ; then
echo dir2classpath  dir
exit
fi
JARS=`ls $1/*.jar`
JARP=`echo ${JARS} | sed 's/[ ]/:/g'`
export CLASSPATH=${CLASSPATH}:${JARP}
echo JARS added to classpath: ${JARP}


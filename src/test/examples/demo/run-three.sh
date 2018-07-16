#!/bin/bash

numiterations=1
sleeptime=0.2
switchfile=/tmp/.switch
/bin/rm -f $switchfile

for i in `seq $numiterations`
do
#   echo "************************************************************" 
#   echo "Join results better be produced" 
#   echo "************************************************************" 
   seq 500 | (
   while read i
   do
	 echo $i,$i
	 echo $i,$i
#	 sleep $sleeptime
	 echo $i,$i
     if test -f $switchfile
     then
        /bin/rm -f $switchfile
        break
     fi
   done
   )

#   echo "************************************************************" 
#   echo "No join results " 
#   echo "************************************************************" 
   seq 500 | (
   while read i
   do
	 echo $i,$i
     echo $i,$i
#	 sleep $sleeptime
     echo $i,$i
     if test -f $switchfile
     then
        /bin/rm -f $switchfile
        break
     fi
   done
   )

#   echo "************************************************************" 
#   echo "Join results better be produced" 
#   echo "************************************************************" 
   seq 500 | (
   while read i
   do
	 echo $i,$i
	 echo $i,$i
#	 sleep $sleeptime
	 echo $i,$i
     if test -f $switchfile
     then
        /bin/rm -f $switchfile
        break
     fi
   done
   )

#   echo "************************************************************" 
#   echo "No join results " 
#   echo "************************************************************" 
   seq 500 | (
   while read i
   do
     echo $i,$i
     echo $i,$i
#	 sleep $sleeptime
     echo $i,$i
     if test -f $switchfile
     then
        /bin/rm $switchfile
        break
     fi
   done
   )
done | ./source-three.pl 127.0.0.1 5533 csvwrapper,test.r csvwrapper,test.t csvwrapper,test.s

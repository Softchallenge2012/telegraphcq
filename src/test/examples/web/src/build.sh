#!/bin/bash

if [ "$1" == "-c" ]
then
echo cleaning distribution...
find . -name \*.class -exec rm {} \; > /dev/null
rm -f WebQueryServer.jar
fi


MANIFESTPATH=$PWD/Manifest
rm $PWD/../jars/WebQueryServer.jar
jars=`find $PWD/../jars -name \*.jar`
javaclasses=`echo $jars | sed 's/[ ]/:/g'`
export CLASSPATH=`pwd`:`pwd`/import:${javaclasses}
echo CLASSPATH is $CLASSPATH

pushd TeSS
make
popd
pushd org/w3c
echo building W3C HTTP libraries...
make
popd
pushd HtmlGet
echo building HtmlGet...
javac -g HtmlGet.java PrintResults.java 
popd
javac WebQueryServer.java
find . -name \*.class | xargs jar cvf WebQueryServer.jar
cp WebQueryServer.jar $PWD/../jars

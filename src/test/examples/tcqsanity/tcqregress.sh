#!/bin/sh
#          -*-mode:ksh-*-
#
echo "**********************************************************************"
echo "Welcome to the TelegraphCQ Regression Test Suite                      "
echo "Creating a data directory at src/test/examples/tcqsanity/data         "
rm -rf data
PGDIR=`pwd`/data
initdb -D $PGDIR
pg_ctl start -l logfile -D $PGDIR
sleep 3
pwd
which createdb
echo "Creating a sample database                                            "
createdb sample
pg_ctl stop -D $PGDIR
cd ../demo && ./createdb.sh $PGDIR
echo "Setting up streams                                                    "
./setup.sh $PGDIR
cd ../tcqsanity
echo "Compiling the sanity tests, if required                               "
make tcqsanity
echo "Creating the output dir, if necessary                                 "
if ! test -d "output"; then
	mkdir output;
else
	echo "Output dir exists."
fi;
echo "Running the sanity tests                                              "
./tcqsanity $PGDIR
echo "Ending the TelegraphCQ Regression Test Suite                          "
echo "**********************************************************************"

#!/bin/sh

case $# in
    0) PGDIR=$PGDATA;;
    1) PGDIR=$1;;
    *) echo 'Usage: $1 [database]';;
esac

# 1. Kill streamers and related processes
echo "1. Killing streamers and related processes"
for i in WebQueryServer runtcp runis runss sed awk td tee tcpdump ss\.pl tis\.pl geoserver
do
    ./ssig.sh $i 9
done

### # 2. Send SIGINT to all psql client processes
### echo "2. Send SIGINT (2) to all psql client processes"
### ./ssig.sh psql 2
### 
### # 3. Send SIGINT to all postgres back end (tcq front end) processes
### echo "3. Send SIGINT to all postgres back end (tcq front end) processes"
### ./ssig.sh postgres 2

# 4. Finally it's safe to call pg_ctl stop to bring down the system
echo "4. Run pg_ctl stop"
pg_ctl stop -D $PGDIR

### ## 5. For good measure run "ipcs" and "ipcclean"
### #echo "5. For good measure run "ipcs" and "ipcclean""
### ipcs
### ipcclean
### 
### # 6. Display the processes
### echo "6. Display the processes"
### ps -fu $USER

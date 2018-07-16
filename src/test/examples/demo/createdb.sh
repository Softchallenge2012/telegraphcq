PATH=${PGRSHOME}/bin:${PATH}
export PATH

case $# in
  0) 
	 PGDIR=$PGDATA;;
  1)
	 PGDIR=$1;
	 if [ -z "$PGDATA" ]
	 then
        echo "set the PGDATA variable and rerun this script"
        exit 1
	 fi;;
  *) echo 'Usage: $0 [PGDIR]';;
esac

if [ !  -d $PGDIR ]
then
    initdb --no-locale -D $PGDIR
    pg_ctl start -l /tmp/log -D $PGDIR
    sleep 5
    createdb sample 
    pg_ctl stop -D $PGDIR
else
    echo $PGDIR is already setup... continuing
fi


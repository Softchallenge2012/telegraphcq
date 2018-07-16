echo 'making sure postgres is not already running'
pg_ctl stop
sleep 5

echo 'turning on TCQ'
./tcq_start
sleep 5

echo 'loading in the schema/tables/streams...'
psql -C -f schema.sql sample
psql -C -f wrap.sql sample
psql -C -f tables.sql sample
psql -C -f streams.sql sample

echo 'loading data into the tables'
PWD=`pwd`
TSNAME="$PWD/sensors.csv"
psql -c "COPY traffic.stations from '$TSNAME' using delimiters ','" sample
TINAME="$PWD/intersections.csv"
psql -c "COPY traffic.intersections from '$TINAME' using delimiters ','" sample
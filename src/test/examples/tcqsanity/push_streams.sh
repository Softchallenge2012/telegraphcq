HOST=localhost
PORT=5533

CSV_WRAP=csvwrapper
TRAFFIC_SCHEMA=traffic
INCIDENT_STREAM=incidents
SENSOR_STREAM=measurements
NATURAL_STREAM=nat_src # 12/03/2004 anil WITH syntax
PRIME_STREAM=prime_src # 12/03/2004 anil WITH syntax
STORE_STREAM=rfid # 5/28/2005 Shariq... first event query
LIBRARY_STREAM=books # 5/28/2005 Shariq... second event query


NETWORK_SCHEMA=network
TCPD_STREAM=tcpdump

NUMBERS_SCHEMA=numbers # 12/03/2004 anil WITH syntax
STORE_SCHEMA=store # 5/28/2005 Shariq... first event query
LIBRARY_SCHEMA=library # 5/28/2005 Shariq... second event query


INCIDENT_NAME="$CSV_WRAP,$TRAFFIC_SCHEMA.$INCIDENT_STREAM"
SENSOR_NAME="$CSV_WRAP,$TRAFFIC_SCHEMA.$SENSOR_STREAM"
TCPD_NAME="$CSV_WRAP,$NETWORK_SCHEMA.$TCPD_STREAM"
NATURAL_NAME="$CSV_WRAP,$NUMBERS_SCHEMA.$NATURAL_STREAM"
PRIME_NAME="$CSV_WRAP,$NUMBERS_SCHEMA.$PRIME_STREAM"
STORE_NAME="$CSV_WRAP,$STORE_SCHEMA.$STORE_STREAM"
LIBRARY_NAME="$CSV_WRAP,$LIBRARY_SCHEMA.$LIBRARY_STREAM"

echo 'sending incidents stream...'
cat incidents.log | ./source.pl $HOST $PORT $INCIDENT_NAME

echo 'sending sensor readings stream...'
cat sensors.log | ./source.pl $HOST $PORT $SENSOR_NAME

echo 'sending tcpdump stream...'
cat tcpdump.log | ./source.pl $HOST $PORT $TCPD_NAME

echo 'sending natural stream...'
cat natural.log | ./source.pl $HOST $PORT $NATURAL_NAME

echo 'sending prime stream...'
cat prime.log | ./source.pl $HOST $PORT $PRIME_NAME

echo 'sending store stream...'
cat store.log | ./source.pl $HOST $PORT $STORE_NAME

echo 'sending library stream...'
cat library.log | ./source.pl $HOST $PORT $LIBRARY_NAME



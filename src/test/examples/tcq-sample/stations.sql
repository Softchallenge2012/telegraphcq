drop table station;
create table station(region integer, stationid integer, name varchar(100), unknown integer, highway integer, mile float, direction char, longitude float, latitude float);
copy station from '/home/tcq/work/postgres/src/test/examples/tcq-sample/init.txt' using delimiters ',';

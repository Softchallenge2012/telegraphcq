create table traffic.stations (
	region     integer, 
	stationid  integer, 
	name       varchar(100), 
	unknown    integer, 
	highway    integer, 
	mile       float, 
	direction  char, 
	latitude   float,
	longitude  float 
);

--copy traffic.stations from 
-- '/home/sailesh/work/postgres/src/test/examples/demo/init.txt' 
-- using delimiters ',';

create table traffic.intersections (
  highway    varchar(100),
  street     varchar(100),
  latitude   float,
  longitude  float
);

--copy traffic.intersections
--	from  '/home/sailesh/work/postgres/src/test/examples/demo/04-inter-latlong.csv'
--	with delimiter ',';




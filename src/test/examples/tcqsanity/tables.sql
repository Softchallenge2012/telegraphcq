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

create table traffic.intersections (
  highway    varchar(100),
  street     varchar(100),
  latitude   float,
  longitude  float
);




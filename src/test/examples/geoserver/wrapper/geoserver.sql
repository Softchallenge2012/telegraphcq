drop function geocode_rescan(integer);
drop wrapper geoserver;
drop STREAM geoserver_data;
create STREAM geoserver_data(
	addr cidr,
	city varchar(90),
	state varchar(2),
	country varchar(90),
	latitude float,
	longitude float,
	tcqtime timestamp TIMESTAMPCOLUMN) type unarchived;

create function geocode_rescan(integer) returns boolean 
as 'libgeocode.so','geocode_rescan' language 'C';

create wrapper geoserver (init=csv_init,
			  next=csv_next,
			  done=csv_done,
			  rescan=geocode_rescan)	
 with wrapperinfo 'BATCHSIZE=10,WRAPPERTYPE=PULL,HOST=localhost,PORT=6754,ERROR_INDICATOR=ERROR:';

alter stream geoserver_data add wrapper geoserver with bindings(addr);	

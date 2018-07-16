
-- drop existing objects
drop function wrappers.tcpdump_init(int);
drop function wrappers.tcpdump_next(int);
drop function wrappers.tcpdump_done(int);
drop wrapper tcpdump;
drop stream streams.tcpdump;

create schema wrappers;
create schema streams;
create function wrappers.tcpdump_init(integer) returns boolean 
as '/Users/owenc/postgres/lib/libcsv.so','csv_init' language 'C';
create function wrappers.tcpdump_next(integer) returns boolean 
as '/Users/owenc/postgres/lib/libcsv.so','csv_next' language 'C';
create function wrappers.tcpdump_done(integer) returns boolean 
as '/Users/owenc/postgres/lib/libcsv.so','csv_done' language 'C';

create wrapper tcpdump (init=wrappers.tcpdump_init, next=wrappers.tcpdump_next, done=wrappers.tcpdump_done) with wrapperinfo 'DEFAULT_DELIMITERS= ,DELIMITERS0=>,DELIMITERS1=:';

create stream streams.tcpdump(col1 varchar(200), 
			col2 varchar(200), 
			col3 varchar(200),
			col4 varchar(200),
			col5 varchar(200),
			col6 varchar(200),
			col7 varchar(200),
			col8 varchar(200),
			col9 varchar(200),
			col10 varchar(200),
			tcqtime timestamp TIMESTAMPCOLUMN) type archived;

alter stream streams.csvstream add wrapper csvwrapper;

		

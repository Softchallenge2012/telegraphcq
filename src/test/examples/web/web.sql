
-- drop existing objects
drop wrapper websource;
drop function web_rescan(integer);
drop stream quotes;

create function web_rescan(integer) returns boolean 
as '/Users/owenc/postgres/lib/libweb.so','web_rescan' language 'C';


create wrapper websource (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done,rescan=web_rescan) 
 with wrapperinfo 'BATCHSIZE=10,WRAPPERTYPE=PULL,HOST=localhost,PORT=2000,QUERY=file ../quotes.hget,ERROR_INDICATOR=ERROR:';


create stream quotes(symbol varchar(200),
		     last varchar(200), price float,
		     tcqtime TIMESTAMP TIMESTAMPCOLUMN) 
type unarchived;
alter stream quotes add wrapper websource WITH BINDINGS (symbol);

		


drop stream trafficincidents;

drop wrapper trafficincidents;
drop function trafficincidents_init(int);
drop function trafficincidents_next(int);
drop function trafficincidents_done(int);

create stream trafficincidents(tcqtime timestamp TIMESTAMPCOLUMN, 
				incidentID integer,
				-- locsize integer,
				location varchar(1000),
				-- citysize integer,
				city varchar(1000),
				-- descsize integer,
				description varchar(1000)
				) type archived;

create function trafficincidents_init(integer) returns boolean
as '/home/emach/postgres/lib/libtcqsample.so', 'trafficincidents_init' language 'C';
create function trafficincidents_next(integer) returns boolean
as '/home/emach/postgres/lib/libtcqsample.so', 'trafficincidents_next' language 'C';
create function trafficincidents_done(integer) returns boolean
as '/home/emach/postgres/lib/libtcqsample.so', 'trafficincidents_done' language 'C';

create wrapper trafficincidents(init=trafficincidents_init,
				next=trafficincidents_next,
				done=trafficincidents_done);

alter stream trafficincidents add wrapper trafficincidents;

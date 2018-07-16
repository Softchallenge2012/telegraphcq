drop stream measurements;
drop wrapper measurements;
drop function measurements_init(int);
drop function measurements_next(int);
drop function measurements_done(int);

create function measurements_init(integer) returns boolean 
as '/home/amol/postgres-cvs/tcq_sigmod_03_branch/lib/libtcqsample.so', 'measurements_init' language 'C';
create function measurements_next(integer) returns boolean 
as '/home/amol/postgres-cvs/tcq_sigmod_03_branch/lib/libtcqsample.so', 'measurements_init' language 'C';
create function measurements_done(integer) returns boolean 
as '/home/amol/postgres-cvs/tcq_sigmod_03_branch/lib/libtcqsample.so', 'measurements_init' language 'C';

create wrapper measurements(init=measurements_init, 
			    next=measurements_next,
			    done=measurements_done);
                
create stream measurements(tcqtime timestamp TIMESTAMPCOLUMN,
			 stationid integer, speed real) type archived;

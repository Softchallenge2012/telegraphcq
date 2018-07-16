
-- drop existing objects
drop function wrappers.csv_init(int);
drop function wrappers.csv_next(int);
drop function wrappers.csv_done(int);
drop wrapper csvwrapper;

drop schema wrappers;
drop schema streams;

create schema wrappers;
create schema streams;
create function wrappers.csv_init(integer) returns boolean 
as 'libcsv.so','csv_init' language 'C';
create function wrappers.csv_next(integer) returns boolean 
as 'libcsv.so','csv_next' language 'C';
create function wrappers.csv_done(integer) returns boolean 
as 'libcsv.so','csv_done' language 'C';

create wrapper csvwrapper (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 


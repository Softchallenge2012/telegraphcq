-- csv functions
-- drop existing objects
drop function wrappers.csv_init(int);
drop function wrappers.csv_next(int);
drop function wrappers.csv_done(int);

create schema wrappers;
create function wrappers.csv_init(integer) returns boolean 
as 'libcsv.so','csv_init' language 'C';
create function wrappers.csv_next(integer) returns boolean 
as 'libcsv.so','csv_next' language 'C';
create function wrappers.csv_done(integer) returns boolean 
as 'libcsv.so','csv_done' language 'C';


-- drop all the wrappers
drop wrapper csvwrapper1; 
drop wrapper csvwrapper2; 
drop wrapper csvwrapper3; 
drop wrapper csvwrapper4; 
drop wrapper csvwrapper5; 
drop wrapper csvwrapper6; 
drop wrapper csvwrapper7; 
drop wrapper csvwrapper8; 
drop wrapper csvwrapper9; 
drop wrapper csvwrapper10; 

-- create 10 wrappers
create wrapper csvwrapper1 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper2 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper3 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper4 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper5 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper6 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper7 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper8 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper9 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 
create wrapper csvwrapper10 (init=wrappers.csv_init, next=wrappers.csv_next, done=wrappers.csv_done); 

-- drop 10 streams
drop stream stream1;
drop stream stream2;
drop stream stream3;
drop stream stream4;
drop stream stream5;
drop stream stream6;
drop stream stream7;
drop stream stream8;
drop stream stream9;
drop stream stream10;


-- create 10 streams
create stream stream1(i1 int, j1 int, k1 int, l1 int, tcqtime timestamp TIMESTAMPCOLUMN) type unarchived;
create stream stream2(i2 int, j2 int, k2 int, l2 int, tcqtime timestamp TIMESTAMPCOLUMN) type unarchived;
create stream stream3(i3 int, j3 int, k3 int, l3 int, tcqtime timestamp TIMESTAMPCOLUMN) type unarchived;
create stream stream4(i4 int, j4 int, k4 int, l4 int, tcqtime timestamp TIMESTAMPCOLUMN) type unarchived;
create stream stream5(i5 int, j5 int, k5 int, l5 int, tcqtime timestamp TIMESTAMPCOLUMN) type unarchived;
create stream stream6(i6 int, j6 int, k6 int, l6 int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream stream7(i7 int, j7 int, k7 int, l7 int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream stream8(i8 int, j8 int, k8 int, l8 int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream stream9(i9 int, j9 int, k9 int, l9 int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream stream10(i10 int, j10 int, k10 int, l10 int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;

-- add wrappers
alter stream stream1 add wrapper csvwrapper1;
alter stream stream2 add wrapper csvwrapper2;
alter stream stream3 add wrapper csvwrapper3;
alter stream stream4 add wrapper csvwrapper4;
alter stream stream5 add wrapper csvwrapper5;
alter stream stream6 add wrapper csvwrapper6;
alter stream stream7 add wrapper csvwrapper7;
alter stream stream8 add wrapper csvwrapper8;
alter stream stream9 add wrapper csvwrapper9;
alter stream stream10 add wrapper csvwrapper10;

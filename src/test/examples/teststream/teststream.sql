-- drop existing objects
drop stream teststreamstream;
drop wrapper teststreamwrapper;
drop function teststream_init(int);
drop function teststream_next(int);
drop function teststream_done(int);

-- create the stream.  Every stream must have a column named tcqtime 
-- with datatype timestamp and the special TIMESTAMP constraint declared 
-- on it.  
-- telegraphCQ uses this attribute to determine the time for the tuple 
-- in windowed stream operations.
 
create function teststream_init(integer) returns boolean as 'libwrapper.so','teststream_init' language 'C';
create function teststream_next(integer) returns boolean as 'libwrapper.so','teststream_next' language 'C';
create function teststream_done(integer) returns boolean as 'libwrapper.so','teststream_done' language 'C';

create wrapper teststreamwrapper(init=teststream_init, next=teststream_next, done=teststream_done);

create stream teststreamstream(i int, j int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;

alter stream teststreamstream add wrapper teststreamwrapper;

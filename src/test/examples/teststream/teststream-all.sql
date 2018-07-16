-- drop existing objects
drop stream teststream;
drop function teststream_init(int);
drop function teststream_next(int);
drop function teststream_done(int);

-- create the stream.  Every stream must have a column named tcqtime 
-- with datatype timestamp and the special TIMESTAMPCOLUMN constraint declared 
-- on it.  
-- telegraphCQ uses this attribute to determine the time for the tuple 
-- in windowed stream operations.
 
create stream teststream(i int, j int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create function teststream_init(integer) returns boolean 
as 'libwrapper.so','teststream_init' language 'C';
create function teststream_next(integer) returns boolean 
as 'libwrapper.so','teststream_next' language 'C';
create function teststream_done(integer) returns boolean 
as 'libwrapper.so','teststream_done' language 'C';

-- drop existing objects
drop stream teststream1;
drop function teststream1_init(int);
drop function teststream1_next(int);
drop function teststream1_done(int);

-- create the stream.  Every stream must have a column named tcqtime 
-- with datatype timestamp and the special TIMESTAMP constraint declared 
-- on it.  
-- telegraphCQ uses this attribute to determine the time for the tuple 
-- in windowed stream operations.
 
create stream teststream1(i int, j int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create function teststream1_init(integer) returns boolean 
as 'libwrapper.so','teststream1_init' language 'C';
create function teststream1_next(integer) returns boolean 
as 'libwrapper.so','teststream1_next' language 'C';
create function teststream1_done(integer) returns boolean 
as 'libwrapper.so','teststream1_done' language 'C';

-- drop existing objects
drop stream teststream2;
drop function teststream2_init(int);
drop function teststream2_next(int);
drop function teststream2_done(int);

-- create the stream.  Every stream must have a column named tcqtime 
-- with datatype timestamp and the special TIMESTAMP constraint declared 
-- on it.  
-- telegraphCQ uses this attribute to determine the time for the tuple 
-- in windowed stream operations.
 
create stream teststream2(i int, j int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create function teststream2_init(integer) returns boolean 
as 'libwrapper.so','teststream2_init' language 'C';
create function teststream2_next(integer) returns boolean 
as 'libwrapper.so','teststream2_next' language 'C';
create function teststream2_done(integer) returns boolean 
as 'libwrapper.so','teststream2_done' language 'C';

-- drop existing objects
drop stream teststream3;
drop function teststream3_init(int);
drop function teststream3_next(int);
drop function teststream3_done(int);

-- create the stream.  Every stream must have a column named tcqtime 
-- with datatype timestamp and the special TIMESTAMP constraint declared 
-- on it.  
-- telegraphCQ uses this attribute to determine the time for the tuple 
-- in windowed stream operations.
 
create stream teststream3(i int, j int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create function teststream3_init(integer) returns boolean 
as 'libwrapper.so','teststream3_init' language 'C';
create function teststream3_next(integer) returns boolean 
as 'libwrapper.so','teststream3_next' language 'C';
create function teststream3_done(integer) returns boolean 
as 'libwrapper.so','teststream3_done' language 'C';


-- -- Create the streams for internal traffic monitoring
-- 
-- create stream tcq_queries(tcqtime timestamp TIMESTAMPCOLUMN, 
-- 						   qrynum  int,
-- 						   qid     int,
-- -- 'E' - Entry
-- -- 'X' - Exit
-- 						   kind    char,
-- 						   qrystr  varchar(1000)
-- ) type unarchived;
-- 
-- create stream tcq_operators(tcqtime timestamp TIMESTAMPCOLUMN, 
-- 							 opnum   int,
-- 							 opid    int,
-- 							 numqrs  int,
-- -- 'E' - Enter operator
-- -- 'A' - Add query to operator
-- -- 'R' - Remove query from operator
-- -- 'X' - Exit operator
-- 							 kind    char,
-- 							 qid     int,
-- 							 opstr   varchar(1000),
-- -- 'S' - SteM
-- -- 'G' - GSFilter
-- -- 'M' - ScanModule
--                                                          opkind  char,
--                                                          opdesc  varchar(100)
-- ) type unarchived;
-- 
-- create stream tcq_queues(tcqtime timestamp TIMESTAMPCOLUMN, 
-- 						  opid    int,
-- -- 'I' - Input queue
-- -- 'O' - Output queue
-- 						  qkind   char,
-- -- 'E' - Successful enqueue
-- -- 'F' - Failed enqueue
-- -- 'D' - Successful dequeue
-- -- 'N' - NULL dequeue
-- 						  kind    char
-- ) type unarchived;
-- 
-- -- Create the streams for network traffic monitoring
-- 


create stream network.tcpdump(
  src     cidr,
  sport   int,
  dst     cidr,
  dport   int,
  flags   varchar(10),
  tcqtime timestamp TIMESTAMPCOLUMN
) type archived;

-- Create the streams for physical traffic monitoring
create stream traffic.measurements (
       stationid int,
       speed     real,
       tcqtime   timestamp TIMESTAMPCOLUMN
) type archived;

create stream traffic.incidents (
  incidentID   integer,
  location     varchar(1000),
  city         varchar(1000),
  description  varchar(1000),
  highway      varchar(100),
  street       varchar(100),
  tcqtime      timestamp TIMESTAMPCOLUMN
) type archived;

-- Create the streams for physical traffic monitoring
create stream test.r(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type archived;

create stream test.s(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type archived;
 
create stream test.t(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type archived;

create stream test.u(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type archived;





create stream web.quotes(symbol varchar(200),
		     last varchar(200), price float,
		     tcqtime TIMESTAMP TIMESTAMPCOLUMN) 
type unarchived;

-- 12/03/2004 anil WITH syntax
create stream numbers.nat(i int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream numbers.nat_src(i int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream numbers.prime(i int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream numbers.prime_src(i int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
		

-- 5/28/2005 Shariq ... First event query
create stream store.rfid(tag_id varchar(10), loc_id varchar(10), tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream store.counter(tag_id varchar(10), tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream store.door(tag_id varchar(10), tcqtime timestamp TIMESTAMPCOLUMN) type archived;

-- 5/28/2005 Shariq ... Second event query
create stream library.books(book_name varchar(50), shelf_id int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;
create stream library.books1(book_name varchar(50), shelf_id int, tcqtime timestamp TIMESTAMPCOLUMN) type archived;

-- Associate streams with wrappers

alter stream network.tcpdump add wrapper csvwrapper;
alter stream traffic.measurements add wrapper csvwrapper;
alter stream traffic.incidents add wrapper csvwrapper;
alter stream test.r add wrapper csvwrapper;
alter stream test.s add wrapper csvwrapper;
alter stream test.t add wrapper csvwrapper;
alter stream test.u add wrapper csvwrapper;
alter stream web.quotes add wrapper websource WITH BINDINGS (symbol);

-- 12/03/2004 anil WITH syntax
alter stream numbers.nat_src add wrapper csvwrapper;
alter stream numbers.prime_src add wrapper csvwrapper;

-- 5/28/2005 Shariq... First event query		

alter stream store.rfid add wrapper csvwrapper;

-- 5/28/2005 Shariq... Second event query		

alter stream library.books add wrapper csvwrapper;



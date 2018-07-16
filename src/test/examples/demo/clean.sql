-- Drop the streams
drop stream tcq_queries;
drop stream tcq_operators;
drop stream tcq_queues;
drop stream network.tcpdump;
drop stream traffic.measurements;
drop stream traffic.incidents;	
drop stream web.quotes;
drop stream test.r;
drop stream test.s;
drop stream test.t;
drop stream test.u;

drop stream store.rfid;
drop stream store.counter;
drop stream store.door;

drop stream library.books;

-- Drop the tables
drop table traffic.stations;
drop table traffic.intersections;

-- Drop the wrappers

-- drop functions
drop function wrappers.web_rescan(integer);
-- Drop the schema
drop schema network;
drop schema traffic;
drop schema wrappers;
drop schema web;
drop schema test;

drop schema store;
drop schema library;

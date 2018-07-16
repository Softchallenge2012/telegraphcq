create stream network.tcpdump(
  src     cidr,
  sport   int,
  dst     cidr,
  dport   int,
  flags   varchar(10),
  tcqtime timestamp TIMESTAMPCOLUMN
) type archived;

create stream traffic.measurements (
       stationid int,
       speed     real,
       tcqtime   timestamp TIMESTAMPCOLUMN
) type unarchived;

create stream traffic.incidents (
  incidentID   integer,
  location     varchar(1000),
  city         varchar(1000),
  description  varchar(1000),
  highway      varchar(100),
  street       varchar(100),
  tcqtime      timestamp TIMESTAMPCOLUMN
) type archived;

alter stream network.tcpdump add wrapper csvwrapper;
alter stream traffic.measurements add wrapper csvwrapper;
alter stream traffic.incidents add wrapper csvwrapper;


-- Assumes the existence of the csv wrapper
drop stream streams.tcpdump;

create stream streams.tcpdump(
	src     cidr,
	sport   int,
	dst     cidr,
	dport   int,
	flags   varchar(10),
	tcqtime timestamp TIMESTAMPCOLUMN
) type archived;

alter stream streams.tcpdump add wrapper csvwrapper;

		

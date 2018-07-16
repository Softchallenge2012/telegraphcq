
-- Assumes the existence of the mscsv wrapper
drop stream streams.measurements;

create stream streams.measurements(
	stationid integer,
	speed     real,
	tcqtime timestamp TIMESTAMPCOLUMN
) type archived;

alter stream streams.measurements add wrapper mscsvwrapper;

		

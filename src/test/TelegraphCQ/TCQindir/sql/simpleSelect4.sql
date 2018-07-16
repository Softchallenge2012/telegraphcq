SELECT s.stationId, m.speed
FROM measurements m, station s
WHERE m.stationId = s.stationId;

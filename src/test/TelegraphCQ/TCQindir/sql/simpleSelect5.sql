SELECT ms.stationId, MAX(s.mile), AVG(ms.speed)
FROM measurements ms, station s
WHERE ms.stationId = s.stationId
GROUP BY ms.stationId
HAVING ms.stationId > 999406
WINDOW ms ['1 minutes'];

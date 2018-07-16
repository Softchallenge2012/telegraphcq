select tinc.*,tint.latitude,tint.longitude,tm.*,ts.latitude,ts.longitude
from   traffic.incidents     as tinc,
       traffic.measurements  as tm,
       traffic.intersections as tint,
       traffic.stations      as ts
where  (tinc.highway||tinc.street) = (tint.highway||tint.street) and
       tm.stationid = ts.stationid                               and
       (round(cast(ts.latitude as numeric),1)||round(cast(ts.longitude as numeric),1)) 
                        =
       (round(cast(tint.latitude as numeric),1)||round(cast(tint.longitude as numeric),1))
window tinc ['15 minutes'], tm ['5 minutes'];

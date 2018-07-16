select tcqtime, incidentID, location, city, description, tinc.highway, tinc.street, latitude, longitude
from   traffic.incidents as tinc, traffic.intersections as tint
where  (tinc.highway||tinc.street) = (tint.highway||tint.street);
--where  tinc.highway = tint.highway and tinc.street = tint.street;
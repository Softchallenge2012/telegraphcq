-- This table will contain a mopped up set of lat-long for sensor stations
create table upts as 
   select region,stationid,name,unknown,ts.highway,mile,direction,ti.latitude,ti.longitude
   from   traffic.intersections as ti, traffic.stations as ts
   where  upper(ts.name) = (case 
                              when ti.street like '%AV%' then (ti.street||'E') 
                              else ti.street 
                            end) and 
          ('FWY '||ts.highway) = ti.highway and 
          ts.latitude = 0                   and 
          ts.longitude = 0;


-- Check how many we could mop up
select count(*) from upts;

-- Check how many sensor stations there are in all
select count(*) from traffic.stations;

-- Brutally remove all sensor stations for which we don't have lat-long info
delete from traffic.stations where latitude = 0 and longitude = 0;

-- Check again !
select count(*) from traffic.stations;

-- Now insert all the brand new matches that were formed in upts
insert into traffic.stations 
   select * from upts;

-- Check yet again !
select count(*) from traffic.stations;

-- Blow off upts
drop table upts;


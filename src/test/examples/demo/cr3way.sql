drop stream bug.u;
drop stream bug.t;
drop stream bug.s;
drop stream bug.r;
drop schema bug;
create schema bug;

create stream bug.r(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type unarchived;

create stream bug.s(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type unarchived;
 
create stream bug.t(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type unarchived;
 
create stream bug.u(
  i int,
  j int,
  tcqtime timestamp TIMESTAMPCOLUMN
) type unarchived;

alter stream bug.r add wrapper csvwrapper;
alter stream bug.s add wrapper csvwrapper;
alter stream bug.t add wrapper csvwrapper;
alter stream bug.u add wrapper csvwrapper;

select *
from   test.r as r,
       test.s as s,
       test.t as t
where  r.i = s.i and
       s.i = t.i
window r['2 seconds'],s['2 seconds'],t['2 seconds'];

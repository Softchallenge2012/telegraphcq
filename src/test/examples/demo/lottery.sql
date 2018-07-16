select    top.opdesc as stem, count(*) as c
from      tcq_queues as tq, tcq_operators as top
where     tq.opid = top.opid and
          top.opkind = 'F' and
          tq.qkind   = 'I'  and
          tq.kind    = 'E'
group by  stem
window    tq ['5 seconds'];
 
 

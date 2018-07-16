
/*-------------------------------------------------------------------------
 *
 * nodeFEventAgg.c - A Fjord style operator to handle event aggregation on 
 * streams
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Event aggregation is like a continuous UDA, and allows easy expression
 * of queries that are not natural in traditional SQL (like sequence queries)
 *
 * Original implementation for our SIGMOD 2005 demo
 *
 * Shariq Rizvi <rizvi[AT]cs.berkeley.edu>  
 *
 * $Id: nodeFEventAgg.c,v 1.3 2005/07/27 20:04:05 phred Exp $
 *
 * Note
 * ----
 *
 * This operator relies on getting tuples on the input streams in TOTAL ORDER
 * of timestamps. 
 * It has a unified view of time across all input streams, and if this is not 
 * true then unexpected things can happen, like TCQ sanity event queries 
 * failing.
 * For e.g., an instance of the NOTFOLLOWS event operator, which is looking for
 * R tuples "not followed" by an S tuple within 3 seconds will work correctly 
 * only if both streams bring in a unified notion of time. Otherwise, it is not
 * clear whether the "3 seconds" should be measured on the timeline of R, or 
 * the timeline of S. 
 * This has been ensured currently by making the Eddy choose source tuples in
 * Timestamp order. 
 *
 *
 * Memory Management for Event Processing
 * --------------------------------------
 *
 * This operator does its own memory management on top of the underlying
 * mechanisms provided by MemoryContexts, pfree() and heap_freetuple(). 
 *
 * Like the SteM, this operator has to cache base tuples until the "window" 
 * slides. However, unlike the SteM, there is no global window here. This
 * is because event queries have semantic windows, which are different for 
 * different event operators inside EventAgg - like Follows, Both, NotPrecedes
 *
 * Event operators maintain a list of input/output Events. This is currently
 * implemented as a PostgreSQL List of EventEntries (which apart from Events,
 * maintains some book-keeping information). In the future, these lists 
 * can be replaced by SteMs, I think. When Events expire, they (and their 
 * EventEntries) need to be pfree'd. Also, the base tuples they point to 
 * need to be pfree'd when they are not needed anymore. 
 *
 * In summary, this operator faces TWO kinds of memory management issues:
 *
 * (1) For Events (IHTs) and EventEntries
 *
 * (2) For base tuples pointed to by Events
 *
 * (1) is done by pfreeing the Events (IHTs) and EventEntries when they are
 * not needed by the operator they are inputs of.
 *
 * (2) is tricky. As a base tuple may be pointed to by several Events in 
 * different lists all around the operator tree. 
 * For this, we maintain a cache of base tuples, whose size is defined by the 
 * macro MAX_BASE_TUPLES_CACHED. A pin count mechanism has been implemented 
 * inside this operator to expire base tuples when they have no Event (IHT) 
 * pointing to them. 
 *
 * A default strategy used throughout this code is to stay in the tempContext 
 * which gets reset by the Fjord dispatcher on a per-call basis. Note that the 
 * timestamp functions leak big-time. Only when we really need, we switch to 
 * the main context.
 *
 * Thanks to a painful stint at finding memory leaks for the SIGMOD 2005 demo, 
 * this operator currently has no known memory leaks.
 *
 * rizvi [AT] cs.berkeley.edu
 * June 5, 2005
 * 
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "executor/nodeFjord.h"
#include "executor/nodeFEventAgg.h"
#include "telegraphcq/events.h"
#include "parser/parse.h" 


#define MAX_BASE_TUPLES_CACHED 1000


/* 
 * CORE EVENT-PROCESSING FUNCTIONS 
*/

static void
publishSimpleEvent(EventExpr * e, Event * event, FEventAggState *mystate);

static void
expireEvents(EventExpr * e, Timestamp ts, FEventAggState *mystate);

static Event* 
fetchNextComplexEvent(FEventAggState * mystate);

static void
constructComplexEvents(EventExpr * e, Timestamp lastTimestampSeen, FEventAggState *mystate);


/* 
 * INTERNAL STATIC FUNCTIONS
 */


static int
findIndexOfSourceForSimpleEvent(Event *e);

static bool
isRelevantSimpleEvent(EventExpr * e, Event * event);

static void
putSimpleEventOnOutputList(EventExpr * e, Event * event, FEventAggState *mystate);
    

static Event*
copyEventInPrivateContext(Event * event, FEventAggState * mystate);

static bool
haveSimpleEventConflict(Event *c1, Event *c2);

/*List*
expireEventsFromListTimestampBased(List *l, Timestamp ts);
*/

static void
expireAllEventsFromList(List *l, FEventAggState *mystate);


static List*
expireEventsFromListIntervalBased(List *l, Timestamp ts, Interval i, FEventAggState *mystate);


static char*
ts_cstr(Timestamp t);

static Event*
combineEvents(Event *e1, Event *e2, FEventAggState *mystate);

static Event*
replicateEvent(Event *e, FEventAggState *mystate);

static Timestamp
getSimpleEventTimestamp(Event *e);


static void
deleteEvent(Event * e, FEventAggState *mystate);

static void
deleteEventEntry(EventEntry * ee, FEventAggState *mystate);

static void
deleteEventEntryButNotEvent(EventEntry * ee);

static bool
arePartitionConsistent(Event * e1, Event * e2, FEventAggState *mystate);

static bool
isPartitionConsistent(Event * e, FEventAggState *mystate);


/* static bool */
/* isPartitionConsistentOneAttr(Event * e, List * vars, FEventAggState *mystate); */


static Event *
getEventFromEventEntry(EventEntry * ee);


static EventEntry *
CreateEventEntry(Event * e);

static bool
haveBeenMatched(EventEntry *ee1, EventEntry *ee2);


static void
rememberMatch(EventEntry *ee1, EventEntry *ee2);

static void
setParentPointers(EventExpr *e, EventExpr *parent);


static void
MakeTimeoutRequest(EventExpr *e, Timestamp ts, FEventAggState *mystate);

static void
ServiceTimeouts(FEventAggState *mystate, Timestamp lastTimestampSeen);

static Expr *
makeEqualityExpression(Var * var1, Var * var2);


static void 
incrementBaseTuplePinCount(FEventAggState *mystate, void *bt);

static void 
decrementBaseTuplePinCount(FEventAggState *mystate, void *bt);

/* static void */
/* removeBaseTupleFromCache(FEventAggState *mystate, void *bt); */

static int
getPositionOfBaseTuplePointer(FEventAggState *mystate, void *bt);


static void
addBaseTuplePointerToCache(FEventAggState *mystate, void *bt);

static void
freeAllDoneBaseTuples(FEventAggState *mystate);


static void 
incrementBaseTuplePinCountsForEvent(FEventAggState *mystate, Event * e);


static void 
decrementBaseTuplePinCountsForEvent(FEventAggState *mystate, Event * e);


static void
addBaseTuplePointersToCacheForEvent(FEventAggState *mystate, Event * e);

/* SIGMOD HACK */
static bool
passAdditionalBothTest(Event * e1, Event * e2);


/* SIGMOD HACK */
static bool
passAdditionalNotFollowsTest(Event * e1, Event * e2);


static void
prepareOutputIHTForDelivery(IntermediateHeapTuple oiht, FEventAggState * mystate);

static void 
generateOutputPunct(IntermediateHeapTuple oiht, FEventAggState * mystate);

static void
initializeExprCube(FEventAggState * mystate);


/* 
 * EXTERNAL FUNCTIONS USED IN THIS FILE 
 */


extern bool 
ihtIsPunct(IntermediateHeapTuple iht);


extern bool 
ts_geq(Timestamp t1, Timestamp t2);


extern Timestamp
ts_plus_interval(Timestamp ts, Interval i);

extern Timestamp
ts_max(Timestamp t1, Timestamp t2);

extern bool
ts_equals(Timestamp t1, Timestamp t2);

extern Oid
exprType(Node * expr);

extern Expr *
make_op(List * opname, Node * var1, Node * var2);

extern Oper *
replace_opid(Oper * oper);

extern HeapTuple 
form_punct_tup(TupleDesc tupleDescriptor, Timestamp ts);



/* 
 * PUBLIC FUNCTIONS
 */

bool 
ProcessInitFEventAgg(FEventAgg * node, EState * es, Plan * parent) {

  MemoryContext mymctx, octx;
  int i;
  FEventAggState *mystate = &(node->mystate);

  FjordInitialize(&(node->fjord), es);
  

  
  node->fjord.usesTempContext = TRUE; /* So that the Fjord dispatcher resets our tempContext per-call */


  
  
  mystate->mymctx = AllocSetContextCreate(TopMemoryContext,
					  "FEventAgg Main Context",
					  ALLOCSET_DEFAULT_MINSIZE,
					  ALLOCSET_DEFAULT_INITSIZE,
					  ALLOCSET_DEFAULT_MAXSIZE);

  mystate->punctContext = AllocSetContextCreate(TopMemoryContext,
					  "FEventAgg Punctuation Context",
					  ALLOCSET_DEFAULT_MINSIZE,
					  ALLOCSET_DEFAULT_INITSIZE,
					  ALLOCSET_DEFAULT_MAXSIZE);
  
  mymctx = mystate->mymctx;


  octx = MemoryContextSwitchTo(mymctx);
  
  mystate->queryid = -1;
  
  mystate->sources = BitArrayCreate(TCQMAXSOURCES);
  
  
  mystate->timeoutRequests = NIL;
  mystate->tempContext = ((Fjord *)node)->tempContext;
  mystate->feventagg = (Node *) node;
  mystate->eddy = (Node *) parent;
  mystate->lastTupleTimestamp = 0;

  /* Book-keeping for base-tuple memory management */
  mystate->baseTuplesCached = 0;
  mystate->baseTuplePointers = (void **) palloc(MAX_BASE_TUPLES_CACHED * sizeof(void *));
  mystate->baseTuplePinCounts = (int *) palloc(MAX_BASE_TUPLES_CACHED * sizeof(int));
 
  MemSet(mystate->baseTuplePointers, 0x0, MAX_BASE_TUPLES_CACHED * sizeof(void *));
  
  for(i = 0; i < MAX_BASE_TUPLES_CACHED; i++)
    mystate->baseTuplePinCounts[i] = -1;


  MemoryContextSwitchTo(octx);


  Assert(node->eventExpr);

  setParentPointers((EventExpr *) node->eventExpr, NULL);


  /*CommonScanState *scanstate = node->fjord.scanstate;*/
  /*ExprContext *econtext  = scanstate->cstate.cs_ExprContext;*/
  
  /* econtext->ecxt_per_query_memory = econtext->ecxt_per_tuple_memory; */ /* As we want to reset the per_query context per-tuple
								      * This is because ExecEvalExpr allocates stuff in the per_query context, and 
								      * we are calling ExecEvalExpr per-tuple for a new expression every time 
								      * (for the Partition By clause)
								      * 
								      * Ideally, we want to create all the expressions at compile-time and just use
								      * them here. 
								      */ 
  mystate->needToEmitPunct = FALSE;
  mystate->needToResetPunctContext = FALSE;

  initializeExprCube(mystate);

  return TRUE;

}

bool
ProcessEndFEventAgg(FEventAgg * node, Plan * parent) {

  
  FjordEnd(&(node->fjord), (Eddy *)parent);
 
  MemoryContextDelete(node->mystate.mymctx);
  MemoryContextDelete(node->mystate.punctContext);
 
  /* Self-destruct to help dangling pointer detection */
  MemSet(node, 0x0, sizeof(FEventAgg));
  
 

  return TRUE;
  
}

void
ProcessNextFEventAgg(FEventAgg * node, Plan * parent) {

  bool deqsucc, enqsucc;
  IntermediateHeapTuple iht;
  IntermediateHeapTuple *piiht = &iht;
  FEventAggState *mystate = &(node->mystate);
  Fjord *fjord = &(node->fjord);
  MemoryContext mymctx, octx, tempContext;
  Event * event;
  int operator  = node->fjord.operator;
  IntermediateHeapTuple oiht = &(((Eddy *)parent)->eddystate->ModuleIHT_Table[operator]);
  Event *result;

  mymctx = mystate->mymctx; /* A Memory context with operator life-time */
  tempContext = mystate->tempContext; /* A Memory Context with tuple life-time */


  elog(DEBUG1, "ProcessNextFEventAgg: Got control");

  
  /*--------------------------- Init Stuff ---------------------------------
   * First, do some Init stuff, that should ideally be in Init above,
   * but it seems all that we need for doing this is not available till that
   * point in time, for e.g., queryid 
   */
  
  if(mystate->queryid == -1) {
    Assert(length(fjord->usedByQueries) == 1);
    mystate->queryid = lfirsti(fjord->usedByQueries); /* We have asserted that we have only one query */
    BitArrayClone(mystate->sources, ((Eddy *)parent)->eddystate->sources[mystate->queryid], TCQMAXSOURCES); /* Remember what my base streams are */
  }
  else {
    /* We have done this Init in a previous call */
  }

  
  /*-------------------------- Done Init Stuff --------------------------- */
  
  
  if(mystate->needToResetPunctContext) {
    MemoryContextReset(mystate->punctContext);
    mystate->needToResetPunctContext = FALSE;
  }



  FJORD_DEQ_INP_PTR(deqsucc,node,&piiht);

  
  octx = MemoryContextSwitchTo(tempContext);



  if (deqsucc)
    {

      Timestamp lastTimestampSeen = getSimpleEventTimestamp(iht);
    

      if(ts_geq(mystate->lastTupleTimestamp, lastTimestampSeen)) {


	
	elog(DEBUG1, "FEventAgg: Alas... The tuple I got this time is out of order w.r.t. the last tuples. Last: %s vs. This: %s",  ts_cstr(mystate->lastTupleTimestamp), ts_cstr(lastTimestampSeen));
	


      }
      else {

	
	elog(DEBUG1, "FEventAgg: Pheww..... The tuple I got this time is ordered w.r.t. the last tuples. Last: %s vs. This: %s",  ts_cstr(mystate->lastTupleTimestamp), ts_cstr(lastTimestampSeen));
	


      }
      

      mystate->lastTupleTimestamp = lastTimestampSeen;


      /* Service Timeouts here */

      ServiceTimeouts(mystate, lastTimestampSeen);
   
      /**********************************************************/


      /* Disregard Fred-generated punctuation tuples */
      if(ihtIsPunct(iht)) { 

	elog(DEBUG1, "Got a punctuation tuple with timestamp %s", ts_cstr(lastTimestampSeen));


	BitArraySetBit(iht->t_queriesCompleted, mystate->queryid); /* Must selectively kill the puncuation tuple, as its schema does not match that of result */
	FJORD_ENQ_OUT_PTR(enqsucc, node, &iht); /* So that other operators can use the punctuation */
	mystate->needToEmitPunct = TRUE; /* Punct in => Punct out */
	MemoryContextSwitchTo(octx);
	return; 
      }
      
      elog(DEBUG1, "ProcessNextFEventAgg: Got a non-punct tuple from the CQ Eddy. Printing it: ");
      /* debugitup_debug(iht); */
      
   
      /* ---------------- Publish this event to our event tree ------------------*/


      
      freeAllDoneBaseTuples(mystate);

      event = copyEventInPrivateContext(iht, mystate); /* So that it remains safe in our private context, till our base tuple management pfree's it */

      event->event_timestamp = lastTimestampSeen;
      
      expireEvents((EventExpr *)node->eventExpr, lastTimestampSeen, mystate);
      publishSimpleEvent((EventExpr *)node->eventExpr, event, mystate); /* Will allocate stuff, like lists, IHTs etc., in our private context */
      deleteEvent(event, mystate); /* People have already cached this event into their private copies, THIS SHOULD NOT FREE THE ACTUAL HEAP TUPLES */
      
      
      /* Now selectively kill the tuple that arrived, for this query */
      BitArraySetBit(iht->t_queriesCompleted, mystate->queryid);
      
      
      
      /* Let go of "iht" */
      FJORD_ENQ_OUT_PTR(enqsucc, node, &iht);
    
    

    }
  else {

      
    elog(DEBUG1, "ProcessNextFEventAgg: Did not get a tuple from the CQ Eddy. So, will do my work");    
     

    /* First, check if we have a pending punctuation emission */
    if(mystate->needToEmitPunct) {
      /* Generate (in punctContext) and enqueue an output punctuation tuple */
      generateOutputPunct(oiht, mystate);
      prepareOutputIHTForDelivery(oiht, mystate);

      elog(DEBUG1, "FEventAgg: Sending out an output punctuation tuple. Printing it:");
      /* debugitup_debug(oiht); */  


      FJORD_ENQ_OUT_PTR(enqsucc, node, &oiht);
      
      mystate->needToEmitPunct = FALSE;
      mystate->needToResetPunctContext = TRUE;

      MemoryContextSwitchTo(octx);
      return;

    }


    result = fetchNextComplexEvent(mystate);
    
    if(result == NULL) {
      MemoryContextSwitchTo(octx);
      return;
    }

    
    /* Now, we will first copy this event IHT into our module's output IHT slot, provided by the Eddy */
    copyIntermediateHeapTuple(oiht, result);
    deleteEvent(result, mystate); /* If required, the base tuples will be garbage-collected the next time */
    
    prepareOutputIHTForDelivery(oiht, mystate); /* Will set bit arrays correctly */

    
    /* Send this event tuple to the CQ Eddy */
    FJORD_ENQ_OUT_PTR(enqsucc, node, &oiht);


  }


  MemoryContextSwitchTo(octx);
  
}


bool
ProcessRemoveQueryFEventAgg(FEventAgg * node, int queryid, Plan * parent) {


     
  return FjordRemoveQuery(&(node->fjord), queryid);
  
}


void 
ProcessPrintStatsFEventAgg(FEventAgg * node, Plan * parent) {

  elog(LOG, "ProcessPrintStatsFEventAgg: Currently nothing printed here");

}


static int
findIndexOfSourceForSimpleEvent(IntermediateHeapTuple iht) {

  BitArray sources = iht->t_sources;
  if(BitArrayNumSet(sources, TCQMAXSOURCES) > 1) {
    elog(LOG, "findIndexOfSourceForSimpleEvent: Multiple sources for this IHT... printing event..");
    elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
    debugitup_debug(iht);
    elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
    elog(FATAL, "findIndexOfSourceForSimpleEvent: Multiple sources for this IHT... not a simple event!");

  }

  else if(BitArrayNumSet(sources, TCQMAXSOURCES) < 1)
    elog(FATAL, "findIndexOfSourceForSimpleEvent: No sources for this IHT!");
  else  /* Exactly one source */
    return BitArrayFirstBit(sources, TCQMAXSOURCES);

  Assert(0); /* Should never reach here */
  return -1; /* Keep compiler quiet */
}



void
publishSimpleEvent(EventExpr * e, Event * event, FEventAggState *mystate) {
  Timestamp lastTimestampSeen = event->event_timestamp; 
  
  if(e == NULL)
    elog(FATAL, "publishSimpleEvent: Got a NULL Event Expression");


  switch(e->event_op) {
    
  case EVENT_SIMPLE:
    if(isRelevantSimpleEvent(e, event)) {
      putSimpleEventOnOutputList(e, event, mystate);
      if(e->parent)
	constructComplexEvents(e->parent, lastTimestampSeen, mystate);

    }

    break;

  case EVENT_ANYONE:
    publishSimpleEvent(e->lchild, event, mystate);
    publishSimpleEvent(e->rchild, event, mystate);

    break;
    
  case EVENT_BOTH:
    publishSimpleEvent(e->lchild, event, mystate);
    publishSimpleEvent(e->rchild, event, mystate);

    break;
    
  case EVENT_FOLLOWS:
    publishSimpleEvent(e->lchild, event, mystate);
    publishSimpleEvent(e->rchild, event, mystate);

    break;
    
  case EVENT_PRECEDES:
    publishSimpleEvent(e->lchild, event, mystate);
    publishSimpleEvent(e->rchild, event, mystate);

    break;
    
  case EVENT_NOTFOLLOWS:
    publishSimpleEvent(e->lchild, event, mystate);
    publishSimpleEvent(e->rchild, event, mystate);

    break;
    
  case EVENT_NOTPRECEDES:
    publishSimpleEvent(e->lchild, event, mystate);
    publishSimpleEvent(e->rchild, event, mystate);

    break;
    

  default:
    elog(FATAL, "publishSimpleEvent: Unexpected Event Operator");
    break;
  }
  

}

bool
isRelevantSimpleEvent(EventExpr * e, Event * event) {
  int expectedSourceIndex;
  int whichSource;

  if(e == NULL)
    elog(FATAL, "isRelevantSimpleEvent: Received a NULL Event Expression");
  
  if(e->event_op != EVENT_SIMPLE)
    elog(FATAL, "isRelevantSimpleEvent: Received a complex event tree when expecting a simple one");
  
  expectedSourceIndex = e->var->varno - 1;

  whichSource = findIndexOfSourceForSimpleEvent(event);
  
  elog(DEBUG1, "isRelevantSimpleEvent: whichSource = %d, expectedSourceIndex = %d", whichSource, expectedSourceIndex);

  return (whichSource == expectedSourceIndex);

}

void
putSimpleEventOnOutputList(EventExpr * e, Event * event, FEventAggState *mystate) {

  MemoryContext octx;

  if(e == NULL)
    elog(FATAL, "isRelevantSimpleEvent: Received a NULL Event Expression");

  if(e->event_op != EVENT_SIMPLE)
    elog(FATAL, "isRelevantSimpleEvent: Received a complex event tree when expecting a simple one");

  
  octx = MemoryContextSwitchTo(mystate->mymctx);
  e->outputList = lappend(e->outputList, CreateEventEntry(replicateEvent(event, mystate))); /* The event is already in our private context */
  MemoryContextSwitchTo(octx);


  elog(DEBUG1, "putSimpleEventOnOuputList: Just put a tuple into an output list. Printing the tuple");
  /* debugitup_debug(event);*/   

}


/* NOTE: Set context to private context when calling this  */
Event*
copyEventInPrivateContext(Event * event, FEventAggState *mystate) {



  MemoryContext octx = MemoryContextSwitchTo(mystate->mymctx);
    
  Event *newEvent = CreateIntermediateHeapTuple();
 
  
  copyIntermediateHeapTuple(newEvent, event);
  
  /* Copying base tupels here is necessary, as they get PFREEed by other people */
  copy_base_tuples_into_context(newEvent, CurrentMemoryContext); 
     
  addBaseTuplePointersToCacheForEvent(mystate, newEvent);
  incrementBaseTuplePinCountsForEvent(mystate, newEvent);

  MemoryContextSwitchTo(octx);

  return newEvent;

}



/* List* */
/* expireEventsFromListTimestampBased(List *l, Timestamp ts) { */
  
/*   if(l == NIL) */
/*     return NIL; */


/*   List *result = NIL, *iter; */

/*   foreach(iter, l) { */

/*     EventEntry *currentEventEntry = lfirst(iter); */
/*     Event *currentEvent = getEventFromEventEntry(currentEventEntry); */

/*     if(ts_geq(currentEvent->event_timestamp, ts)) { /\* Check correctness *\/ */
/*       result = lappend(result, currentEvent); */
/*     } */
/*     else { */
/*       elog(DEBUG1, "FEventAgg: Expiring Event - "); */
/*       elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"); */
/*       debugitup_debug(currentEvent); */
/*       elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo"); */
/*       deleteEvent(currentEvent); */
/*     } */
/*   } */
  
/*   freeList(l); /\* Does not free the actual Events .. memory leak ? *\/ */

/*   return result; */

/* } */

void
expireAllEventsFromList(List *l, FEventAggState *mystate) {

  List *iter;
  foreach(iter, l) {

    EventEntry *ee = lfirst(iter);
    deleteEventEntry(ee, mystate);

  }

  freeList(l);
}



List*
expireEventsFromListIntervalBased(List *l, Timestamp ts, Interval i, FEventAggState *mystate) {
  
  MemoryContext octx;
  List *result = NIL, *iter;

  if(l == NIL)
    return NIL;



  foreach(iter, l) {

    EventEntry *currentEventEntry = lfirst(iter);
    Event *currentEvent = currentEventEntry->event;
    if(ts_geq(ts_plus_interval(currentEvent->event_timestamp, i), ts)) { /* Check correctness */
      octx = MemoryContextSwitchTo(mystate->mymctx);
      result = lappend(result, currentEventEntry);
      MemoryContextSwitchTo(octx);
    }
    else {
      elog(DEBUG1, "FEventAgg: Expiring Event - ");
      elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
      /* debugitup_debug(currentEvent);*/
      elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
      deleteEventEntry(currentEventEntry, mystate);
    }
      
  }
  
  freeList(l);

  return result;

}



/* Important function: defines event expiry semantics */
/* An operator expires tuples from the output queue of its children, that is, its own input queues */
void
expireEvents(EventExpr * e, Timestamp ts, FEventAggState *mystate) {
  
  if(e == NULL)
    elog(FATAL, "expireEvents: NULL event expression passed");
  
  switch(e->event_op) {
  
  case EVENT_SIMPLE:
    /* Do nothing */
    break;
    
  case EVENT_ANYONE:
    /* Do nothing to self */
    expireEvents(e->lchild, ts, mystate);
    expireEvents(e->rchild, ts, mystate);
    break;
    
    
  case EVENT_BOTH:
    e->lchild->outputList = expireEventsFromListIntervalBased(e->lchild->outputList, ts, e->time_delay, mystate);
    e->rchild->outputList = expireEventsFromListIntervalBased(e->rchild->outputList, ts, e->time_delay, mystate);
    expireEvents(e->lchild, ts, mystate);
    expireEvents(e->rchild, ts, mystate);
    break;

  case EVENT_FOLLOWS:
    e->lchild->outputList = expireEventsFromListIntervalBased(e->lchild->outputList, ts, e->time_delay, mystate);

    expireAllEventsFromList(e->rchild->outputList, mystate);
    e->rchild->outputList = NIL;

    expireEvents(e->lchild, ts, mystate);
    expireEvents(e->rchild, ts, mystate);
    break;

  case EVENT_PRECEDES:

    expireAllEventsFromList(e->lchild->outputList, mystate);
    e->lchild->outputList = NIL;

    e->rchild->outputList = expireEventsFromListIntervalBased(e->rchild->outputList, ts, e->time_delay, mystate);
    expireEvents(e->lchild, ts, mystate);
    expireEvents(e->rchild, ts, mystate);
    break;
    
  case EVENT_NOTFOLLOWS:
    e->lchild->outputList = expireEventsFromListIntervalBased(e->lchild->outputList, ts, e->time_delay, mystate);

    expireAllEventsFromList(e->rchild->outputList, mystate);
    e->rchild->outputList = NIL;

    expireEvents(e->lchild, ts, mystate);
    expireEvents(e->rchild, ts, mystate);
    break;

  case EVENT_NOTPRECEDES:

    expireAllEventsFromList(e->lchild->outputList, mystate);
    e->lchild->outputList = NIL;
    
    e->rchild->outputList = expireEventsFromListIntervalBased(e->rchild->outputList, ts, e->time_delay, mystate);
    expireEvents(e->lchild, ts, mystate);
    expireEvents(e->rchild, ts, mystate);
    break;
    

  default:
    elog(FATAL, "expireEvents: Unexpected Event Operator");
    break;
  }

}

void 
constructComplexEvents(EventExpr * e, Timestamp lastTimestampSeen, FEventAggState *mystate) {

  List *iter, *iter1, *iter2;
  bool detected = FALSE;
  MemoryContext octx;
  Event *e1,*e2,*e3;
  EventEntry *ee1,*ee2,*ee3;
  
  if(e == NULL)
    elog(FATAL, "constructComplexEvents: NULL event expression passed");
  
  switch(e->event_op) {
  
  
  case EVENT_SIMPLE:
    /* Do nothing.. a simple event operator produces its events during publishSimpleEvent() */
    
    break;
    
  
  case EVENT_ANYONE:

    
    /* Look at both the children's list, pass everything up */
   
    if(e->lchild->outputList == NIL && e->rchild->outputList == NIL)
      return;

    octx = MemoryContextSwitchTo(mystate->mymctx);

    foreach(iter, e->lchild->outputList) {
      e->outputList = lappend(e->outputList, lfirst(iter));
    }
    freeList(e->lchild->outputList); /* Does not free the actual events, which have been enqueued in the list above */ 
    e->lchild->outputList = NIL;
    
    foreach(iter, e->rchild->outputList) {
      e->outputList = lappend(e->outputList, lfirst(iter));
    }

    freeList(e->rchild->outputList);
    e->rchild->outputList = NIL;

    MemoryContextSwitchTo(octx);

    if(e->parent)
      constructComplexEvents(e->parent, lastTimestampSeen, mystate);

    break;
    
    
  case EVENT_BOTH:

    detected = FALSE;

    foreach(iter1, e->lchild->outputList) {

      ee1 = lfirst(iter1);
      e1 = getEventFromEventEntry(ee1);

      foreach(iter2, e->rchild->outputList) {
	

	ee2 = lfirst(iter2);
	e2 = getEventFromEventEntry(ee2);
	
	if(haveSimpleEventConflict(e1, e2))
	  continue;
	
	/* Now test that they are within time_delay of each other */
	if(!(ts_geq(ts_plus_interval(e1->event_timestamp, e->time_delay), e2->event_timestamp)
	     &&
	     ts_geq(ts_plus_interval(e2->event_timestamp, e->time_delay), e1->event_timestamp)))
	  continue;

	if(haveBeenMatched(ee1, ee2))
	  continue;
	  
	if(!passAdditionalBothTest(e1, e2))
	  continue;

        octx = MemoryContextSwitchTo(mystate->mymctx);

	e3 = combineEvents(e1, e2, mystate);
	
	ee3 = CreateEventEntry(e3);

	e->outputList = lappend(e->outputList, ee3);
	rememberMatch(ee1, ee2);
	
	MemoryContextSwitchTo(octx);
	
	detected = TRUE;

      }
    }

    if(detected)
      if(e->parent)
	constructComplexEvents(e->parent, lastTimestampSeen, mystate);
    
    
    break;

  case EVENT_FOLLOWS:

    
    detected = FALSE;

     foreach(iter1, e->lchild->outputList) {
       
       ee1 = lfirst(iter1);
       e1 = getEventFromEventEntry(ee1);

       
       foreach(iter2, e->rchild->outputList) {
	 
	    ee2 = lfirst(iter2);
	    e2 = getEventFromEventEntry(ee2);
       
	 
	 if(haveSimpleEventConflict(e1, e2))
	   continue;
	 
	 

	 if(!ts_geq(ts_plus_interval(e1->event_timestamp, e->time_delay), e2->event_timestamp))
	   continue;
	 
	 if(haveBeenMatched(ee1, ee2))
	   continue;

	  
	 octx = MemoryContextSwitchTo(mystate->mymctx);

	 e3 = combineEvents(e1, e2, mystate);
	 
	 ee3 = CreateEventEntry(e3);
	 
	 e->outputList = lappend(e->outputList, ee3);
	 
	 rememberMatch(ee1, ee2);

	 MemoryContextSwitchTo(octx);

	 detected = TRUE;
	 
       }
     }

     /* No events from the right child are important any more ...  */
     expireAllEventsFromList(e->rchild->outputList, mystate);
     e->rchild->outputList = NIL; 
    
     
     
     if(detected)
       if(e->parent)
	 constructComplexEvents(e->parent, lastTimestampSeen, mystate);
     
     
     
     break;
     
  case EVENT_PRECEDES:
    
    detected = FALSE;

    foreach(iter1, e->lchild->outputList) {
      
      ee1 = lfirst(iter1);
      e1 = getEventFromEventEntry(ee1);

      
      foreach(iter2, e->rchild->outputList) {
	
	ee2 = lfirst(iter2);
	e2 = getEventFromEventEntry(ee2);

	
	if(haveSimpleEventConflict(e1, e2))
	  continue;
	
	
	if(!ts_geq(ts_plus_interval(e2->event_timestamp, e->time_delay), e1->event_timestamp))
	  continue;
	
	octx = MemoryContextSwitchTo(mystate->mymctx);
	e3 = combineEvents(e1, e2, mystate);
	ee3 = CreateEventEntry(e3);
	e->outputList = lappend(e->outputList, ee3);
	rememberMatch(ee1, ee2);
	MemoryContextSwitchTo(octx);
	
	detected = TRUE;
	
      }
    }

    /* No events from the left child are important any more ...  */
    expireAllEventsFromList(e->lchild->outputList, mystate);
    e->lchild->outputList = NIL; 
    
    if(detected)
      if(e->parent)
	constructComplexEvents(e->parent, lastTimestampSeen, mystate);
    


    break;
    
    
  case EVENT_NOTFOLLOWS:

    detected = FALSE;
    
    foreach(iter1, e->lchild->outputList) {
      
      bool TimeToStopLooking = FALSE;
      bool SeenATerminatingEvent = FALSE;
      
      ee1 = lfirst(iter1);
      e1 = getEventFromEventEntry(ee1);

      if(ee1->marked) /* We have already detected this guy as +ve */
	continue;


      if(!ts_geq(ts_plus_interval(e1->event_timestamp, e->time_delay), lastTimestampSeen)) {
	
	/* elog(DEBUG1, "%d is NOT >= %d", ts_plus_interval(e1->event_timestamp, e->time_delay), lastTimestampSeen); */
	TimeToStopLooking = TRUE;
	
      }
      else {
	/* elog(DEBUG1, "%d is >= %d", ts_plus_interval(e1->event_timestamp, e->time_delay), lastTimestampSeen); */
	
      }
      
      
      foreach(iter2, e->rchild->outputList) {
	

	ee2 = lfirst(iter2);
	e2 = getEventFromEventEntry(ee2);

	
	if(haveSimpleEventConflict(e1, e2))
	  continue;

	if(!passAdditionalNotFollowsTest(e1, e2))
	  continue;



	if(!arePartitionConsistent(e1, e2, mystate))
	  continue;
	
	
	
	if(ts_geq(ts_plus_interval(e1->event_timestamp, e->time_delay), e2->event_timestamp)) {
	  SeenATerminatingEvent = TRUE;
	  break;
	}
      }


      elog(DEBUG1, "NOTFOLLOWS: TimeToStopLooking is %d", TimeToStopLooking);
      elog(DEBUG1, "NOTFOLLOWS: SeenATerminatingEvent is %d", SeenATerminatingEvent);
      elog(DEBUG1, "NOTFOLLOWS: lchild list is of length %d", length(e->lchild->outputList));
      elog(DEBUG1, "NOTFOLLOWS: rchild list is of length %d", length(e->rchild->outputList));


      if(SeenATerminatingEvent) {
	/* delete e1 immediately !!! TO DO */

	ee1->marked = TRUE;

      }
      else {
	if(TimeToStopLooking) {
	  
	  octx = MemoryContextSwitchTo(mystate->mymctx);
	  e->outputList = lappend(e->outputList, CreateEventEntry(replicateEvent(e1, mystate)));
	  MemoryContextSwitchTo(octx);

	  ee1->marked = TRUE; /* To make us remember that we have detected this guy */

	  detected = TRUE;
	  
	}

	else { /* ee1 has a future... we need to remember this by making a TimeoutRequest for this node */

	  if(!ee1->requestedTimeout) {
	    Timestamp timeoutTime = ts_plus_interval(e1->event_timestamp, e->time_delay);
	    octx = MemoryContextSwitchTo(mystate->mymctx);
	    MakeTimeoutRequest(e, timeoutTime, mystate);
	    MemoryContextSwitchTo(octx);
	    
	    ee1->requestedTimeout = TRUE;
	  }
	  
	}

      }
    }

    /* No events from the right child are important any more ...  */
    expireAllEventsFromList(e->rchild->outputList, mystate);
    e->rchild->outputList = NIL; 

    
    if(detected)
      if(e->parent)
	constructComplexEvents(e->parent, lastTimestampSeen, mystate);
    
    
    break;


  case EVENT_NOTPRECEDES:
    
    detected = FALSE;
    
    foreach(iter1, e->lchild->outputList) {
      
      bool SeenATerminatingEvent = FALSE;
      
      ee1 = lfirst(iter1);
      e1 = getEventFromEventEntry(ee1);

      if(ee1->marked) /* Have seen this guy earlier */
	continue;
      else 
	ee1->marked = TRUE;

      
      foreach(iter2, e->rchild->outputList) {
	


	ee2 = lfirst(iter2);
	e2 = getEventFromEventEntry(ee2);
      
	
	if(haveSimpleEventConflict(e1, e2))
	  continue;
	
	if(!arePartitionConsistent(e1, e2, mystate))
	  continue;
	
	if(!ts_geq(e1->event_timestamp, ts_plus_interval(e2->event_timestamp, e->time_delay))) {
	  SeenATerminatingEvent = TRUE;
	  elog(DEBUG1, "FEventAgg, NOTPRECEDES: I have decided to kill the following event");
	  elog(DEBUG1, "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
	  /* debugitup_debug(e1);*/
	  elog(DEBUG1, "ooooooooooooooooooooooooooooooooo Because of ooooooooooooooooooooooooooooooooooooooooooo");
	  /* debugitup_debug(e2);*/
	  elog(DEBUG1, "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
	  break;
	}
      }
      
      if(!SeenATerminatingEvent) {
	elog(DEBUG1, "FEventAgg, NOTPRECEDES: I have decided that the following event is not preceded by any one, and so will put this in output list");
	elog(DEBUG1, "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
	/* debugitup_debug(e1);*/
	elog(DEBUG1, "oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");

	octx = MemoryContextSwitchTo(mystate->mymctx);
	e->outputList = lappend(e->outputList,  CreateEventEntry(replicateEvent(e1, mystate)));
	MemoryContextSwitchTo(octx);

	detected = TRUE;
	
      }
      
    }
    /* No events from the left child are important any more ...  */
    expireAllEventsFromList(e->lchild->outputList, mystate);
    e->lchild->outputList = NIL;
    
    if(detected)
      if(e->parent)
	constructComplexEvents(e->parent, lastTimestampSeen, mystate);
    
    break;
    
    
  default:
    elog(FATAL, "constructComplexEvents: Unexpected Event Operator");
    break;
  }
  
}

bool
haveSimpleEventConflict(Event *c1, Event *c2) {
  int i;

  if(!c1 || !c2)
    elog(FATAL, "haveSimpleEventConflict: Atleast one NULL argument");

  for(i = 0 ; i < TCQMAXSOURCES; i++) {
    if(c1->t_base_tuples[i] == NULL)
      continue;
    if(c2->t_base_tuples[i] == NULL)
      continue;
    
    if(c1->t_base_tuples[i] != c2->t_base_tuples[i])
      return TRUE;
  }

  /* SIGMOD HACK... as we can not reference the same stream multiple times in an event query */
  /* Reverse SIGMOD hack */
  /*if(ts_equals(c1->event_timestamp, c2->event_timestamp))
    return TRUE;*/
  /* End reverse SIGMOD hack */
  /* END SIGMOD HACK */

  return FALSE;

}

Event*
combineEvents(Event *e1, Event *e2, FEventAggState *mystate) {

  MemoryContext octx;
  Event *newEvent;

  if(e1 == NULL || e2 == NULL)
    elog(FATAL, "combineEvents: Got a NULL event");

  newEvent = CreateIntermediateHeapTuple();
  AddBaseTuples(newEvent, e1);
  AddBaseTuplesAllowingOverlap(newEvent, e2);
 


  /* The higher timestamp wins */
  octx = MemoryContextSwitchTo(mystate->tempContext);
  newEvent->event_timestamp = ts_max(e1->event_timestamp, e2->event_timestamp);
  MemoryContextSwitchTo(octx);

  incrementBaseTuplePinCountsForEvent(mystate, newEvent);

  return newEvent;

}

Event*
replicateEvent(Event *e, FEventAggState *mystate) {
  Event *newEvent;

  if(e == NULL)
    elog(FATAL, "replicateEvent: Got a NULL event");

  newEvent = CreateIntermediateHeapTuple();
  AddBaseTuples(newEvent, e);

  newEvent->event_timestamp = e->event_timestamp;

  incrementBaseTuplePinCountsForEvent(mystate, newEvent);

  return newEvent;

}


Event* 
fetchNextComplexEvent(FEventAggState *mystate) {

  EventEntry *resultentry;
  Event *result;

  EventExpr *root = (EventExpr *)((FEventAgg *)mystate->feventagg)->eventExpr;


  if(root == NULL)
    elog(FATAL, "fetchNextComplexEvent: Root of the event tree is NULL!");

  if(root->outputList == NIL)
    return NULL;

  

  resultentry = lfirst(root->outputList);
  root->outputList = lremove(resultentry, root->outputList); 

  result = getEventFromEventEntry(resultentry);

  /*** Partitioning code begins ****/
  if(isPartitionConsistent(result, mystate)) {
    /* Now we will return this event as an IHT. We should pfree the event entry, but not the actual event (IHT), 
       as it will be used by people outside this operator */
    deleteEventEntryButNotEvent(resultentry);
  
    
    return result;

  }

  else {
   
    /* This event entry and its enclosed event are both useless, so we will pfree them both */
    deleteEventEntry(resultentry, mystate);
    return fetchNextComplexEvent(mystate);
  
  }

}

Timestamp
getSimpleEventTimestamp(Event *e) {

  int sourceID = findIndexOfSourceForSimpleEvent(e); /* Also asserts that it is actually a simple event */
  
  Timestamp result = get_timestamp_val(e->t_base_tuple_descs[sourceID], e->t_base_tuples[sourceID]);
  
  return result;

}

void
deleteEvent(Event * e, FEventAggState *mystate) {

  if(!e)
    elog(FATAL, "deleteEvent: Passed a NULL pointer");

  decrementBaseTuplePinCountsForEvent(mystate, e);

  pfree(e);

}

void 
deleteEventEntry(EventEntry * ee, FEventAggState *mystate) {

  
  if(!ee)
    elog(FATAL, "deleteEventEntry: Passed a NULL pointer");

  deleteEvent(ee->event, mystate);
  
  freeList(ee->matched); 

  pfree(ee);

}

void 
deleteEventEntryButNotEvent(EventEntry * ee) {

  
  if(!ee)
    elog(FATAL, "deleteEventEntryButNotEvent: Passed a NULL pointer");

  ee->event = NULL;
  
  freeList(ee->matched);

  pfree(ee);

}


bool
arePartitionConsistent(Event * e1, Event * e2, FEventAggState * mystate) {
  

  Event * e = combineEvents(e1, e2, mystate); /* Can allocate in tempContext... as we will delete really soon */
  
  bool result = isPartitionConsistent(e, mystate);

  deleteEvent(e, mystate); /* ... like right here ! */
  
  return result;

}


bool
isPartitionConsistent(Event * e, FEventAggState * mystate) {

  List *var_lists = ((EventExpr *)((FEventAgg *)mystate->feventagg)->eventExpr)->var_lists;
  FEventAgg * node = (FEventAgg *)mystate->feventagg;
  CommonScanState *scanstate = node->fjord.scanstate;
  ExprContext *econtext  = scanstate->cstate.cs_ExprContext;
  TupleTableSlot *slot = scanstate->css_ScanTupleSlot;
  bool result, isNull = FALSE;
  Datum expr_value;
  int numPartitionAttrs = length(var_lists);
  int i, j, k;
  HeapTuple ht1, ht2;


  
  if(!var_lists) /* No partitioning => All in same partition */
    return TRUE; 

  Assert(numPartitionAttrs >= 1);

  
  /* The event goes into the slot */
  slot->val = (HeapTuple) e; 
  slot->ttc_isIntermediateTuple = TRUE; 
  econtext->ecxt_scantuple = slot; 
	
  for(i = 0; i < TCQMAXSOURCES; i++) {
    
    ht1 = e->t_base_tuples[i];
    if(!ht1)
      continue;

    for(j = i + 1; j < TCQMAXSOURCES; j++) {

      ht2 = e->t_base_tuples[j];
      if(!ht2)
	continue;

      /* We are here => This event has non-null base tuples for positions i and j */
      /* So, we will now compare these two tuples for each of the partitioning attributes */

      for(k = 0; k < numPartitionAttrs; k++) {

	expr_value = ExecEvalExpr((Node *)mystate->exprCube[k][i][j], econtext, &isNull, NULL);     
	result = (!isNull) && (DatumGetBool(expr_value)); 
	
	if(result == FALSE) 
	  return FALSE;
	
      }

    }

  }

  return TRUE;


}


/* bool */
/* isPartitionConsistent(Event * e, FEventAggState *mystate) { */


/*   List *var_lists = ((EventExpr *)((FEventAgg *)mystate->feventagg)->eventExpr)->var_lists; */
/*   List *iter; */

/*   if(!var_lists)  /\* When we have no "Partition by" clause, everything is in the same partition *\/ */
/*     return TRUE; */

/*   foreach(iter, var_lists) { */

/*     List *vars = lfirst(iter); */
/*     if(!isPartitionConsistentOneAttr(e, vars, mystate)) */
/*       return FALSE; */
/*   } */

/*   return TRUE; */
/* } */

/* bool */
/* isPartitionConsistentOneAttr(Event * e, List * vars, FEventAggState *mystate) { */
  
/*   List *iter1, *iter2, *remainingList; */
/*   Var *var1, *var2; */
/*   bool isNull = FALSE; */


/*   HeapTuple ht1, ht2; */
/*   FEventAgg * node = (FEventAgg *)mystate->feventagg; */
/*   CommonScanState *scanstate = node->fjord.scanstate; */
/*   ExprContext *econtext  = scanstate->cstate.cs_ExprContext; */

/*   TupleTableSlot *slot = scanstate->css_ScanTupleSlot; */

/*   Expr *expr; */

/*   bool result; */
/*   Datum	expr_value; */
  
/*   foreach(iter1, vars) { */
/*     var1 = lfirst(iter1); */
    
/*     remainingList = lnext(iter1); */

/*     foreach(iter2, remainingList) { */

      
/*       var2 = lfirst(iter2); */

/*       Assert(IsA(var1, Var)); */
/*       Assert(IsA(var2, Var)); */
      
      
/*       ht1 = e->t_base_tuples[var1->varno - 1]; */
/*       ht2 = e->t_base_tuples[var2->varno - 1]; */
      
/*       if(!ht1 || !ht2) */
/* 	continue; */


      
/*       slot->val = (HeapTuple) e; */
/*       slot->ttc_isIntermediateTuple=TRUE; */
/*       econtext->ecxt_scantuple = slot; */


/*       /\* Time to make an expression *\/ */
/*       /\* We will have an EQ_OP node on top, with Var's hanging around *\/ */
      

/*       expr = makeEqualityExpression(var1, var2); */

      
/*       expr_value = ExecEvalExpr((Node *)expr, econtext, &isNull, NULL); */
      
/*       result = (!isNull) && (DatumGetBool(expr_value)); */

/*       if(result == FALSE) */
/* 	return FALSE; */
/*     } */


/*   } */

/*   return TRUE; */

/* } */

Event *
getEventFromEventEntry(EventEntry * ee) {
  
  return ee->event;

}

EventEntry *
CreateEventEntry(Event * e) {

  EventEntry *ee = (EventEntry *) palloc(sizeof(EventEntry));
  ee->event = e;
  ee->matched = NIL;
  ee->requestedTimeout = FALSE;
  ee->marked = FALSE;

  return ee;

}

bool
haveBeenMatched(EventEntry *ee1, EventEntry *ee2) {

  if(!ee1 || !ee2)
    elog(FATAL, "haveBeenMatched: Atleast one NULL input");

  if(ptrMember(ee1, ee2->matched) || ptrMember(ee2, ee1->matched))
    return TRUE;

  return FALSE;


}

void
rememberMatch(EventEntry *ee1, EventEntry *ee2) {


  if(!ee1 || !ee2)
    elog(FATAL, "rememberMatch: Atleast one NULL input");
  
  ee1->matched = lappend(ee1->matched, ee2);
  ee2->matched = lappend(ee2->matched, ee1);

}


void
setParentPointers(EventExpr *e, EventExpr *parent) {

  if(e == NULL)
    elog(FATAL, "setParentPointers: Got NULL argument");

  switch(e->event_op) {

  case EVENT_SIMPLE:
    e->parent = parent;
    break;

  default:
    e->parent = parent;
    setParentPointers(e->lchild, e);
    setParentPointers(e->rchild, e);
    break;

  }
}


void
MakeTimeoutRequest(EventExpr *e, Timestamp ts, FEventAggState *mystate) {

  MemoryContext octx;


  TimeoutRequest *newRequest = (TimeoutRequest *) palloc(sizeof(TimeoutRequest));

  newRequest->node = (Node *)e;
  newRequest->ts = ts;

  mystate->timeoutRequests = lappend(mystate->timeoutRequests, newRequest);
  
  octx = MemoryContextSwitchTo(mystate->tempContext); /* As the call to ts_cstr() leaks memory */

  elog(DEBUG1, "MakeTimeoutRequest: Added a request for timestamp %s", ts_cstr(ts));

  MemoryContextSwitchTo(octx);

  return;

}

void
ServiceTimeouts(FEventAggState *mystate, Timestamp lastTimestampSeen) {
  
  MemoryContext octx;
  List *requests = mystate->timeoutRequests;
  List *iter;
  List *leftovers = NIL;
  
  elog(DEBUG1, "Length of the Timeout request queue is %d", length(requests));

  if(requests == NIL)
    return;
  
  
  foreach(iter, requests) 
    {
      
      TimeoutRequest *request = (TimeoutRequest *) lfirst(iter);
      
      if(ts_geq(lastTimestampSeen, request->ts)) 
	{
	  

	  elog(DEBUG1, "ServiceTimeouts: Servicing a request for timestamp %s at time %s ", ts_cstr(request->ts), ts_cstr(lastTimestampSeen));

	  
	  constructComplexEvents((EventExpr *)request->node, lastTimestampSeen, mystate);
	  
	  request->node = NULL;
	  pfree(request);
	  
	}
      else 
	{
	  octx = MemoryContextSwitchTo(mystate->mymctx);
	  leftovers = lappend(leftovers, request);
	  MemoryContextSwitchTo(octx);
	  elog(DEBUG1, "ServiceTimeouts: Not servicing a request for timestamp %s at time %s, later...", ts_cstr(request->ts), ts_cstr(lastTimestampSeen));
	}
    }
  
  freeList(requests);
  
  mystate->timeoutRequests = leftovers;
  
}
    


Expr *
makeEqualityExpression(Var *var1, Var *var2) {
    Expr *expr;

  elog(DEBUG1, "makeEqualityExpression: var1 has type %d, var2 has type %d", exprType((Node *)var1), exprType((Node *)var2));
  
  
  expr = make_op((List *)makeList1(makeString("=")), (Node *) var1, (Node *)var2);
  
  replace_opid((Oper *)expr->oper);

  if(OidIsValid(((Oper *)expr->oper)->opid))
    elog(DEBUG1, "makeEqualityExpression: Successfully replaced the OPID of the equality Expr's operator");
  else
    elog(FATAL, "makeEqualityExpression: Failed to replace the OPID of the equality Expr's operator");

  return expr;


}


char * 
ts_cstr(Timestamp ts) {
    return 
        DatumGetCString(
            DirectFunctionCall1(timestamp_out, TimestampGetDatum(ts)));
}


/* SIGMOD Hack */
bool
passAdditionalBothTest(Event * e1, Event * e2) {


  return TRUE;
  /* return !(ts_equals(e1->event_timestamp, e2->event_timestamp));*/

}

/* SIGMOD Hack */
bool
passAdditionalNotFollowsTest(Event * e1, Event * e2) {

  return !(ts_equals(e1->event_timestamp, e2->event_timestamp));

}


void 
incrementBaseTuplePinCount(FEventAggState *mystate, void *bt) {

  int pos = getPositionOfBaseTuplePointer(mystate, bt);

  if(pos == -1)
    elog(FATAL, "incrementBaseTuplePinCount: No pointer in cache for this base tuple");

  if(mystate->baseTuplePinCounts[pos] < 0)
    elog(FATAL, "incrementBaseTuplePinCount: This tuple has a current pin count of < 0... this is unexpected!");

  mystate->baseTuplePinCounts[pos]++;

}




void 
decrementBaseTuplePinCount(FEventAggState *mystate, void *bt) {

  int pos = getPositionOfBaseTuplePointer(mystate, bt);

  if(pos == -1)
    elog(FATAL, "decrementBaseTuplePinCount: No pointer in cache for this base tuple");

  if(mystate->baseTuplePinCounts[pos] < 1)
    elog(FATAL, "decrementBaseTuplePinCount: This tuple has a current pin count of < 1... this is unexpected!");

  mystate->baseTuplePinCounts[pos]--;

}

/* void */
/* removeBaseTupleFromCache(FEventAggState *mystate, void *bt) { */

/*   int pos = getPositionOfBaseTuplePointer(mystate, bt); */
  
/*   if(pos == -1) */
/*     elog(FATAL, "removeBaseTupleFromCache: No pointer in cache for this base tuple"); */


/*   heap_freetuple((HeapTuple) bt); */

/*   mystate->baseTuplePointers[pos] = NULL; */
/*   mystate->baseTuplePinCounts[pos] = -1; */

/* } */


int
getPositionOfBaseTuplePointer(FEventAggState *mystate, void *bt) {

  int i;
  
  for(i = 0 ; i < MAX_BASE_TUPLES_CACHED; i++) {
    if(mystate->baseTuplePointers[i] == bt)
      return i;
  }


  return -1;

}

void
addBaseTuplePointerToCache(FEventAggState *mystate, void *bt) {

  int pos;


  if(getPositionOfBaseTuplePointer(mystate, bt) != -1)
    elog(FATAL, "addBaseTuplePointerToCache: Base tuple already in cache");
  
  for(pos = 0; pos < MAX_BASE_TUPLES_CACHED; pos++) {
    if(mystate->baseTuplePinCounts[pos] == -1)
      break;
  }

  if(pos < MAX_BASE_TUPLES_CACHED) {
    mystate->baseTuplePinCounts[pos] = 0;
    mystate->baseTuplePointers[pos] = bt;
    mystate->baseTuplesCached++;
    
    elog(DEBUG1, "addBaseTuplePointerToCache: Added another base tuple to cache, current count is %d", mystate->baseTuplesCached);

  }
  else {

    elog(FATAL, "addBaseTuplePointerToCache: Out of cache slots for base tuple pointers... Increase the limit given by MAX_BASE_TUPLES_CACHED");

  }

  Assert(mystate->baseTuplesCached <= MAX_BASE_TUPLES_CACHED); 

}


void
freeAllDoneBaseTuples(FEventAggState *mystate) {

  int pos;
  int freed = 0;

  
  for(pos = 0; pos < MAX_BASE_TUPLES_CACHED; pos++) {
    if(mystate->baseTuplePinCounts[pos] == 0) {
      heap_freetuple((HeapTuple)mystate->baseTuplePointers[pos]);
      mystate->baseTuplePinCounts[pos] = -1;
      mystate->baseTuplePointers[pos] = NULL;
      freed++;
    }
  }

  mystate->baseTuplesCached -= freed;
  
  elog(DEBUG1, "freeAllDoneBaseTuples: Freed %d base tuples in this round, now the total count is %d", freed, mystate->baseTuplesCached);

}


void 
incrementBaseTuplePinCountsForEvent(FEventAggState *mystate, Event * e) {
  
  int i;

  for(i = 0 ; i < TCQMAXSOURCES; i++) {
    
    if(e->t_base_tuples[i] == NULL)
      continue;
    
    incrementBaseTuplePinCount(mystate, e->t_base_tuples[i]);

  }  

}


void 
decrementBaseTuplePinCountsForEvent(FEventAggState *mystate, Event * e) {
  
  int i;

  for(i = 0 ; i < TCQMAXSOURCES; i++) {
    
    if(e->t_base_tuples[i] == NULL)
      continue;
    
    decrementBaseTuplePinCount(mystate, e->t_base_tuples[i]);

  }  

}



void
addBaseTuplePointersToCacheForEvent(FEventAggState *mystate, Event * e) {

  
  int i;
  
  for(i = 0 ; i < TCQMAXSOURCES; i++) {
    
    if(e->t_base_tuples[i] == NULL)
      continue;
    
    addBaseTuplePointerToCache(mystate, e->t_base_tuples[i]);

  }  
  
}


void
prepareOutputIHTForDelivery(IntermediateHeapTuple oiht, FEventAggState * mystate) {
  /* 1. Set ready bits */
  BitArrayClearAll(oiht->t_ready, TCQMAXOPERATORS); /* The slotted tuple may have had some ready bits set */
  
  /* 2. done bits should not matter */
  
  /* 3. queriesCompleted - this tuple should go only to this query's output */
  BitArraySetAll(oiht->t_queriesCompleted, TCQMAXQUERIES); 
  BitArrayClearBit(oiht->t_queriesCompleted, mystate->queryid);
  
  /* 4. sources bit array is already set properly */
  
  /* 5. isAggTuple should be set correctly... this was screwing up things for a while */
  oiht->isAggTuple = FALSE;
  
  
  elog(DEBUG1, "ProcessNextFEventAgg: Sending out a result tuple. Printing it now");
  elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");
  /* debugitup_debug(oiht);*/
  elog(DEBUG1, "ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo");

}

/*
 * generateOutputPunct()
 * 
 * Genenrates a punctuation tuple that will go out as my output
 * This is because all operators are required to produce output puncts
 * when they get input puncts. 
 *
 * We do an overkill here. The "schema" of this punct will be a superset of
 * the schema required in my output (for e.g., if we have a NOTPRECEDES event 
 * operator)
 *
 */

void 
generateOutputPunct(IntermediateHeapTuple oiht, FEventAggState * mystate) {

  BitArray sources = mystate->sources;
  int i;
  HeapTuple ht;
  TupleDesc td;
  MemoryContext octx;

  ClearBaseTuples(oiht);

  octx = MemoryContextSwitchTo(mystate->punctContext);

  for(i = 0; i < TCQMAXSOURCES; i++) {
    
    if(BitArrayBitIsSet(sources, i)) {
      td = ((Eddy *)mystate->eddy)->base_tuple_descs[i];
      ht = form_punct_tup(td, mystate->lastTupleTimestamp);

      AddBaseTuple(oiht, ht, td, i, 0); /* Passing '0', as we don't care about Amol-timestamps */

    }
  }
  MemoryContextSwitchTo(octx);
}



/*
 * initializeExprCube()
 *
 * To initialize a 3D array of Expr's that enforce the "Partition By"
 * clause.
 *
 * The size of this cube is N x TCQMAXSOURCES x TCQMAXSOURCES
 *
 * N is the number of attributes in the Partitioning clause
 * For every such attribute, we then have a 2D array of Expr's that compare
 * that attribute in every pair of streams
 * This is a wastage of memory, as in place of TCQMAXSOURCES, we can subsitute
 * the "number of streams in this query". However, then using the cube will be
 * more difficult as the user will have to map source IDs to positions in the 
 * cube.
 *
 * Any one enforcement of the Paritioning constraint will use a subset of 
 * this cube's Expr's. The reason we initialize all of them together is because
 * we don't want to contruct these Expr's using make_op on a per-tuple basis.
 * Ideally, this cube should be created in the frontend at compile-time.
 * But them we will have to modify the cube's Expr's when the frontend plan
 * gets merged with the CQEddy (rangetable changes). So, we do this at FEventAgg init time, 
 * in the backend. 
 *
 */
void
initializeExprCube(FEventAggState * mystate) {


  MemoryContext octx;
  List * var_lists = ((EventExpr *)((FEventAgg *)mystate->feventagg)->eventExpr)->var_lists;
  int i, j, k; /* Preparing to nest .... */
  List *iter1, *iter2, *iter3;
  int numPartitionAttrs;

  if(!var_lists) /* No partitioning => No need to create cube. Cube user should be aware of this */
    return;


  numPartitionAttrs = length(var_lists);

  Assert(numPartitionAttrs >= 1);

  octx = MemoryContextSwitchTo(mystate->mymctx);

  mystate->exprCube = (Expr ****) palloc(sizeof(Expr ***) * numPartitionAttrs);
  
  for(i = 0 ; i < numPartitionAttrs; i++) {
    
    mystate->exprCube[i] = (Expr ***) palloc(sizeof(Expr **) * TCQMAXSOURCES);
    
    for(j = 0; j < TCQMAXSOURCES; j++) {
      mystate->exprCube[i][j] = (Expr **) palloc(sizeof(Expr *) * TCQMAXSOURCES);

      for(k = 0; k < TCQMAXSOURCES; k++) {
	
	mystate->exprCube[i][j][k] = NULL; 

      }

    }
    
  }

  /* We have now allocated all the required arrays.... we now need to fill them */
  
  i = 0;
  j = 0;
  k = 0;

  foreach(iter1, var_lists) {
    List * var_list = lfirst(iter1);

    foreach(iter2, var_list) {
      Var * var1 = lfirst(iter2);
      

      List * remainingList = lnext(iter2);
      
      foreach(iter3, remainingList) {
	
    Expr *expr;
	Var * var2 = lfirst(iter3);
	
	Assert(IsA(var1, Var));
	Assert(IsA(var2, Var));
	
	j = var1->varno - 1;
	k = var2->varno - 1;
	
	/* Now we will generate an Expr for comparing the ith partitioning attribute over the tuples of jth and kth streams */
	
	expr = makeEqualityExpression(var1, var2);

	mystate->exprCube[i][j][k] = expr; /* As comparison between j and k .... */ 
	mystate->exprCube[i][k][j] = expr; /* ... is symmetric */
	

      }
    }

    i++;

  }

  Assert(i == numPartitionAttrs);

  MemoryContextSwitchTo(octx);
}


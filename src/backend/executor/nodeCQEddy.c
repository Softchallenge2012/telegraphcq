
/*-------------------------------------------------------------------------
 *
 * nodeCQEddy.c
 *	  Routines to handle some of CQ Eddy execution
 *
 * This respects the new eddy operator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeCQEddy.c,v 1.35 2006/02/14 22:34:39 phred Exp $
 *
 *-------------------------------------------------------------------------
 */


#include "postgres.h"

#include <ctype.h>

#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <utils/lsyscache.h>

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "postgres.h"
#include "miscadmin.h"
#include "misceddy.h"


#include "optimizer/bindings.h"
#include "executor/executor.h"
#include "executor/execdebug.h"
#include "executor/nodeEddy.h"
#include "executor/nodeGSFilter.h"
#include "executor/nodeFSteM.h"
#include "executor/nodeFjord.h"
#include "executor/nodeScanModule.h"
#include "executor/nodeSteM.h"
#include "executor/staticplan.h"
#include "nodes/copyrte.h"
#include "optimizer/eddy.h"
#include "parser/parsetree.h"
#include "parser/parse_node.h"
#include "parser/parse_coerce.h"
#include "utils/builtins.h"
#include "rewrite/rewriteManip.h"
#include "rqueue/controlq.h"
#include "rqueue/planq.h"
#include "rqueue/rqdest.h"
#include "telegraphcq/dyncatalog.h"
#include "telegraphcq/tcq_usleep.h"
#include "tcop/pquery.h"
#include "utils/bit.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "catalog/namespace.h"

#include "executor/nodeScanSock.h"
#include "executor/nodeStrmScan.h"

#include "telegraphcq/strmsource.h"

/* #define AMOLDEBUGON 1   */

/* #define DEBUG_CQEDDY */

bool goodtoprint = false;
int aggtup = 0;

/*
 * CONSTANTS
*/
#define CQ_EDDY_SEED 1


#define CQ_EDDY_MIN_SLEEP_TIME 0
    /* Minimum amount of microseconds we sleep during an iteration of the 
     * main CQEddy loop. */

#define CQ_EDDY_MAX_SLEEP_TIME 20000
    /* The maximum number of microseconds we sleep during an iteration of the
     * main CQEddy loop. */

#define CQ_EDDY_SLEEP_TIME_INCR 1
    /* Each time we go through the main CQEddy loop without accomplishing
     * anything, we increment the sleep timer by this much. */

#define CQ_EDDY_SLEEP_TIME_THRESH 10000
    /* We only call tcq_usleep() if the sleep amount is over this amount. */
    

#define TEMPORARY_DEFAULT_PORT 5544
    /* HACK ALERT:
     * Until we have the catalog machinery to specify a port for stream data
     * connections we use this port. */


/*
 * PROTOTYPES OF INTERNAL FUNCTIONS 
 */
bool  isPullSource(Eddy * e, int sourceid);
void		debugEddy(Eddy * e, IntermediateHeapTuple iht);
void		printOpBits(Eddy * e, bits8 *bits);
bool		canRemoveQuery(Eddy * node);
int			chooseSourceRandom(Eddy * e);
int chooseSourceTimestampOrder(Eddy * e); /* - TCQ SR */
void		cancelFinishedQueries(Eddy * e);
bool  isSourceFinished(Eddy * e, int sourceid);
void		printEddyStats_debug(Eddy * e);
bool deliverTupleToFAgg(Eddy * cur, IntermediateHeapTuple iht,
			bits8 * whereToSendOutput ,bool faggDelivered);
IntermediateHeapTuple  getFAggResult(Eddy * cur, IntermediateHeapTuple * piht,
				     bits8 * whereToSendOutput);
List * find_queries_using_stream(Eddy * e, Oid streamoid);
static void checkSlideTimeout(Eddy * e, DR_resultqdest * resultqdest);

void setupServerSock(Eddy *node); 
void acceptSrvrConns(Eddy *node);
void getStreamNames(Eddy *node);
void attachSocksToStrms(Eddy *node);
void suckUpUnusedTuples(Eddy *node);

extern bool ts_geq(Timestamp t1, Timestamp t2);

static 	int num_queries = 0;

/*
 *  Prototypes of static functions local to this compilation unit
 */ 
/* #if 0 */
void dumpEddy(Eddy * e);
/* #endif */
static List *getNextMetaOp(Eddy *eddy);
static void printStack(Eddy *eddy);

static void runStaticPlan(Eddy *node);                          /* @dsigmodSK */

void
RunCQEddy()
{
	/*
	 * Initialize the cqeddy plan
	 */
	Eddy	   *cqeddy = NULL;
	EState	   *state = NULL;

	cqeddy = makeNode(Eddy);
	cqeddy->sources = cqeddy->modules = NULL;
	cqeddy->numSources = cqeddy->numModules = 0;
	cqeddy->cacq = TRUE;
	cqeddy->rangetable = NULL;
	cqeddy->eddystate = NULL;

#define CQEDDYNUMSLOTS 32
	state = CreateExecutorState();
	state->es_tupleTable = ExecCreateTupleTable(CQEDDYNUMSLOTS);

	ExecInitEddy(cqeddy, state, NULL);
    elog(LOG,"Calling the CQEddy");
	ExecProcNode((Plan *) cqeddy, NULL);
	elog(LOG,"Returned after calling CQEddy");
	ExecEndEddy(cqeddy);
    elog(LOG,"Done calling the CQEddy");
}

/* ----------------------------------------------------------------
 *		ExecInitCQEddy
 * ----------------------------------------------------------------
 */
bool
ExecInitCQEddy(Eddy * node, EState *estate, Plan *parent)
{
	EddyState  *eddystate;
	int			i;
	const char *tmp=NULL;

	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);
	Assert(node->cacq);

	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * create new EddyState for Eddy
	 */
	eddystate = makeNode(EddyState);
	node->eddystate = eddystate;

	tmp = GetConfigOption("cqeddy_checkplanqiters");
	if(tmp)
		{
			node->eddystate->cqeddy_checkplanqiters=atoi(tmp);
		}
	else
		{
			node->eddystate->cqeddy_checkplanqiters=1;
		}
	/*elog(NOTICE, "cqeddy_checkplanqiters=%d", node->eddystate->cqeddy_checkplanqiters);*/

	tmp = GetConfigOption("cqeddy_checkcontrolqiters");
	if(tmp)
		{
			node->eddystate->cqeddy_checkcontrolqiters=atoi(tmp);
		}
	else
		{
			node->eddystate->cqeddy_checkcontrolqiters=1;
		}
	/*elog(NOTICE, "cqeddy_checkcontrolqiters=%d", node->eddystate->cqeddy_checkcontrolqiters);*/
	tmp = GetConfigOption("cqeddy_checkcancelediters");
	if(tmp)
		{
			node->eddystate->cqeddy_checkcancelediters=atoi(tmp);
		}
	else
		{
			node->eddystate->cqeddy_checkcancelediters=500;
		}
	/*elog(NOTICE, "cqeddy_checkcancelediters=%d", node->eddystate->cqeddy_checkcancelediters);*/
	tmp = GetConfigOption("cqeddy_fixedplan");
	if(tmp)
		{
			node->eddystate->cqeddy_fixedplan=atoi(tmp);
		}
	else
		{
			node->eddystate->cqeddy_fixedplan=-1;
		}
	/*elog(NOTICE, "cqeddy_fixedplan=%d",node->eddystate->cqeddy_fixedplan);*/
	node->eddystate->numsourceswithbindings=0;
	node->eddystate->iterations=0;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &eddystate->jstate);
	ExecAssignExprContext(estate, &eddystate->pstate);
	eddystate->pstate.cs_ExprContext->ecxt_scantuple = MakeTupleTableSlot();

	/*
	 * tuple table initialization
	 */
	if (!ExecUseNewSlots((Plan *) node, parent))
	{
	  ExecInitResultTupleSlot(estate, &eddystate->jstate);
	  ExecInitResultTupleSlot(estate, &eddystate->pstate);
	}
	else
	{
		ExecInitNewResultTupleSlot(estate, &eddystate->jstate);
		ExecInitNewResultTupleSlot(estate, &eddystate->pstate);

		InitSlotForEddy(eddystate->jstate.cs_ResultTupleSlot);
		InitSlotForEddy(eddystate->pstate.cs_ResultTupleSlot);
	}



	/*
	 * now initialize children
	 */
	for (i = 0; i < node->numSources; i++)
	{
		if (node->sources[i] == NULL)
			continue;

		ExecInitNode((Plan *) node->sources[i], estate, (Plan *) node);
	}
	
	/*
	 * Initialize the random number generator's seed
	 */
	srandom(CQ_EDDY_SEED);

	eddystate->stack = NULL;


	eddystate->shutdownPending = FALSE;
	eddystate->queryIds = NULL;
	eddystate->queriesToRemove = NULL;
	eddystate->pjolist = NULL;
	eddystate->driver = -1;

	/*
	 * Initialize the operators and sourceStems bitmasks, and metaops array
	 */
	for (i = 0; i < TCQMAXSOURCES; i++) 
	{
		BitArrayClearAll(eddystate->operators[i], TCQMAXOPERATORS);
		BitArrayClearAll(eddystate->sourceStems[i], TCQMAXOPERATORS);
		eddystate->transitions[i] = NULL;
	}
	
	
	/* No operator is an event operator initially */
	BitArrayClearAll(eddystate->eventOperators, TCQMAXOPERATORS);

	/*
	 * Initialize the completionMask bitmaps
	 */
	for (i = 0; i < TCQMAXQUERIES; i++)
		BitArrayClearAll(eddystate->completionMask[i], TCQMAXOPERATORS);
	for (i = 0; i < TCQMAXQUERIES; i++)
		BitArrayClearAll(eddystate->sources[i], TCQMAXSOURCES);


	/*
	 * Initialize the module stack
	 */
	eddystate->stack = (Node **) palloc(TCQMAXOPERATORS * sizeof(Node *));
	eddystate->stackptr = -1;	/* Set the stack to empty */
	eddystate->numqueries = 0;	/* Clear active query counter to 0 */
	eddystate->ihtValid = false;	/* Mark the IHT as invalid */

	/*
	 * Initialize the routing policy/statistics etc.
	 *
	 * Right now, we have weights for each module. Set them all to 1.
	 */
	for (i = 0; i < node->numModules; i++)
	{
		if (node->modules[i] == NULL)
			continue;
		eddystate->weights[i] = 1;
		eddystate->current_tuples_returned[i] = 0;
	}

	eddystate->total_base_tuples_remaining = MaxTuplesScannedBeforeQuit;
	/* initialization for recursive query support.  Queries in a
	   recursive query are batched in the backend before being integrated
	*/
	eddystate->queryList=NIL;											       

	/*
	 * Initialize debugging
	 */
	eddystate->debugctx = AllocSetContextCreate(TopMemoryContext, "Eddy debugging Memory Context", ALLOCSET_DEFAULT_MINSIZE, ALLOCSET_DEFAULT_INITSIZE, ALLOCSET_DEFAULT_MAXSIZE);

	eddystate->tempContext = AllocSetContextCreate(TopMemoryContext, "Eddy Temporary Memory Context", ALLOCSET_DEFAULT_MINSIZE, ALLOCSET_DEFAULT_INITSIZE, ALLOCSET_DEFAULT_MAXSIZE);

	
	node->eddystate->timestampOrderedSourceSelection = TRUE; /* - SIGMOD 2005 demo TCQ SR */

	return TRUE;
}

TupleTableSlot *CQExecEddy(Eddy * node);

PostJoinOperator *
findPJOfromSources(Eddy * node, BitArray sources)
{
	List	   *iter = NULL;
	PostJoinOperator *pjo;

	foreach(iter, node->eddystate->pjolist)
	{
		pjo = (PostJoinOperator *) lfirsti(iter);

		if (BitArrayEqual(pjo->sources, sources, TCQMAXSOURCES))
			return pjo;
	}
	return NULL;
}

void
cleanOperatorFromPJOList(Eddy * node, int operator)
{
	List	   *iter = NULL;
	PostJoinOperator *pjo;

	foreach(iter, node->eddystate->pjolist)
	{
		pjo = (PostJoinOperator *) lfirsti(iter);

		BitArrayClearBit(pjo->ready, operator);
	}
}

/*
 *	 Augment the ready bits of the IHT with all the operators of any pjo that is
 *	 a subset of the sources of this IHT.
 */
void
setReadyFromPJOList(Eddy * node, IntermediateHeapTuple iht)
{
	List	   *iter = NULL;
	PostJoinOperator *pjo;

	elog(DEBUG1, "In setReadyFromPJOList ");

	foreach(iter, node->eddystate->pjolist)
	{
		pjo = (PostJoinOperator *) lfirsti(iter);

		/*
		 * If the incoming iht's sources is a superset of the pjo's
		 * sources, then we turn on the ready bits of the IHT
		 */
		if (BitArrayContained(iht->t_sources, pjo->sources, TCQMAXSOURCES))
		{
			elog(DEBUG1, "Containment satisfied ! ");
			BitArrayOrSelf(iht->t_ready, pjo->ready, TCQMAXOPERATORS);
		}
	}
}


/*
 *	processControlQ
 *
 *	This routine checks the Eddy's Control Queue for any messages from a TCQ-FE
 *	process. Accordingly, the action corresponding to any message is
 *	performed. At this time, the following messages are supported:
 *
 *	   Query Removal request
 *	   Shutdown request
 *
 *  Returns TRUE if it processed any messages, FALSE otherwise.
 */
static bool
processControlQ(Eddy * node)
{

	Controlqentry   *cqe = NULL;
    bool            did_work = false;

	/*
	 * Check for messages in the Control Queue
	 */
	while (dequeue(*p_control_queue,
				   (char **) &cqe,
				   false,
				   &controlqentry_serializer))
	{
		elog(DEBUG1, "Got controlQ message for query id %d ", cqe->data.queryid);

        did_work = true;

		switch (cqe->command)
		{
			case QUERY_REMOVE:

				/*
				 * elog(DEBUG1, "printing stats before removing a query");
				 * printEddyStats_debug(node);
				 */

				node->eddystate->queriesToRemove
					= lconsi(cqe->data.queryid, node->eddystate->queriesToRemove);
			if(node->eddystate->recursiveQueryDeps[cqe->data.queryid])
					{
						int queryid=-1;
						List * iter = node->eddystate->recursiveQueryDeps[cqe->data.queryid];

						elog(DEBUG1, "eddy got a request to remove query %d", cqe->data.queryid);
						foreach(iter, 
								node->eddystate->recursiveQueryDeps[cqe->data.queryid])
							{
								queryid = lfirsti(iter);
								elog(DEBUG1, "adding queryid %d to removal list because it is dependent on query %d", queryid, cqe->data.queryid);
								node->eddystate->queriesToRemove = lconsi(queryid, node->eddystate->queriesToRemove);
							}

					}
			node->eddystate->recursiveQueryDeps[cqe->data.queryid]=NIL;	
			elog(DEBUG1, "eddy got a request to remove query %d", cqe->data.queryid);
				break;
			case SHUTDOWN:
				node->eddystate->shutdownPending = true;
				elog(INFO,"Shutdown pending set to true in process control queue");
	            printEddyStats_debug_with_level(node, INFO);
				break;
		case WCH_ERROR:
		  {
		    Oid streamoid = cqe->data.w.streamoid;
		    char * message = cqe->error;
		    List * queries = find_queries_using_stream(node, streamoid);
		    TCQHandleErrorMessageForQueries(queries, message);
		    pfree(message); /* in shared memory */
		    break;
		  }

		case EXPLAIN_EDDY:
          {
			char *s, *f;
			result r;
            text *t;

			elog(LOG, "ProcessControlQ(): Explain Eddy");
			s = nodeToString(node);
			f = pretty_format_node_dump(s);
			pfree(s);
			r.type = RESULT_ERROR;
			t = DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(f)));
			r.data.error = t;
			elog(LOG, "ProcessControlQ(): Enqueing EXPLAIN EDDY as RESULT_ERROR");
			if(enqueue(GetQueue(cqe->data.queryid), (char *) &r, false, &result_serializer) == false)
				{
					elog(LOG, "ProcessControlQ(): Error sending EXPLAIN EDDY");
				}
			pfree(f);
			pfree(t);
			break;
          }

		case CONTROLCOMMAND_UNKNOWN:
			default:
				elog(DEBUG1, "eddy received an unknown control queue message");
		}

		pfree(cqe);
	}

    return did_work;

}

/*
 * ProcessQueryRemove
 *
 * 1. Make sure that it is safe to remove a query from the eddy.
 * 2. Call the Fjords query remomal method.  This gives the operator
 *	  the chance to register that a query has left.
 * 3. If the remove function says the operator is no longer used
 *	  (a) shut down the operator via ProcessEndNode
 *	  (b) remove query state from the eddy.
 *
 *	 TODO: Recompute the windows of our FSteMs to take into account the
 *	 newly-shrunken set of queries.
 *
 */
static void
processQueryRemove(Eddy * node)
{
	int32		queryid;
	List	   *iter = NULL;
	List	   *newQueriesToRemove = NIL;

	if (!canRemoveQuery(node) || node->eddystate->queriesToRemove == NIL)
		return;


	elog(INFO,"Removing one or more queries from the CQEddy");
	printEddyStats_debug_with_level(node, INFO);

	foreach(iter, node->eddystate->queriesToRemove)
	{
		queryid = lfirsti(iter);

		/*
		 * Haven't integrated the query into the eddy yet, it must be
		 * waiting on the plan queue.
		 */
		if (!intMember(queryid,
					   node->eddystate->queryIds))
		{
			elog(DEBUG1, "found queryid %d in the queries to remove list, but it"
				 " isn't in the eddy yet.", queryid);
			newQueriesToRemove = lconsi(queryid, newQueriesToRemove);
			continue;
		}
		remove_query(node, queryid);
	}

	elog(DEBUG1, "ProcessQueryRemove removed %d queries,saving %d removal msgs",
		 (length(node->eddystate->queriesToRemove) - length(newQueriesToRemove)),
		 length(newQueriesToRemove));
	freeList(node->eddystate->queriesToRemove);
	node->eddystate->queriesToRemove = newQueriesToRemove;
}

/* processPlanQ
 *
 * This routine checks for any new plans in the Plan Queue.  As long as there
 * are no active queries in the Eddy, we block on the Plan Queue
 *
 * Returns TRUE if it pulled something off the queue, FALSE if it did not.
 */
static
bool
processPlanQ(Eddy * node)
{
	planqentry *pqe = NULL;
	bool		blockpq;
	bool		newplan;
	EddyState  *eddystate = node->eddystate;
    bool        did_work = false;

	blockpq = (eddystate->numqueries == 0);
	newplan =
		dequeue(plan_queue, (char **) &pqe, blockpq, &planqentry_serializer);
	if (newplan)
	{							/* We have a new plan */
        did_work = true;

		if (pqe->plan == NULL)
		{
			/* we've received a wakeup message */
			pfree(pqe);
			return true;
		}
		if (intMember(pqe->queueNo, eddystate->queriesToRemove))
		{
			elog(DEBUG1, "just deueued a plan for queryid %d, which aleardy"
				 " has a removal message", pqe->queueNo);
			eddystate->queriesToRemove = lremovei(pqe->queueNo,
											 eddystate->queriesToRemove);
			if (intMember(pqe->queueNo,
						  eddystate->queryIds) ||
				intMember(pqe->queueNo, eddystate->queriesToRemove))
			{
				elog(DEBUG1, "got a query with a queryid %d, but it's already"
					 "in the eddy", pqe->queueNo);
			}

			FreeQueue(pqe->queueNo);
		}
		else
		{
		  List * iter=NIL;
		  eddystate->queryList = nconc(eddystate->queryList, pqe->pqe_list);
		  eddystate->queryList = lappend(eddystate->queryList, pqe);
		  Assert(pqe->isLast);
		  if(pqe->isLast)
		    {
		      bool needDepQueries=false;
		      Oid oid = InvalidOid;

		      List * depQueries = NIL;
		      int32 depMaster=-1;
		      
			
      
		      if(length(eddystate->queryList) > 1)
			needDepQueries =true;
		      
		      while(eddystate->queryList != NIL)
			{
			  pqe=lfirst(eddystate->queryList);
			  
			  eddystate->queryList=lnext(eddystate->queryList);
			  eddystate->numqueries++;
			  eddystate->queryIds = lappendi(eddystate->queryIds, pqe->queueNo);
			  
			  if(needDepQueries)
			    {
			      if(pqe->querytree->isRecursiveOutput)
				{
				  depMaster = pqe->queueNo;
				}
			      else
				depQueries = lconsi(pqe->queueNo, depQueries);
			    }		
			  
			  add_plan_to_cq_eddy(node,
					      (Eddy *) pqe->plan,
					      pqe->querytree,
					      pqe->queueNo);
			  elog(DEBUG1, "CQEDDY: New query plan is: %p", (void *) node);
			  elog(DEBUG1, nodeToString(node));
			  
			  if(pqe->querytree->recurseRelation != InvalidOid)
			    {
			      node->eddystate->recursiveQueryFeedback[pqe->queueNo] = pqe->querytree->recurseRelation;
			    }

	
			  /*
			   * Time to ensure our sanity
			   */
			  elog(DEBUG1, "Added new query to running CQEddy. We'll now get a dump of the eddy\n");
			  /* dumpEddy(node); */
			  
			  tcq_queries_enter(pqe);
			}
			  
		    
		  eddystate->queryList=NIL;
		  if(needDepQueries && depMaster == -1)
		    elog(FATAL, "query dependency bug");
		  if (depMaster != -1) 
		    {
		      eddystate->recursiveQueryDeps[depMaster] = depQueries;
		      
		      elog(DEBUG1, "Recursive Query added with masterid = %d",
			   depMaster);
		      
		      /* testing code OSC */
		      if(!IsOutputQuery(node, depMaster))
			{
			  elog(FATAL, "depMaster must be an output query");
			}
		    }
		  foreach(iter, depQueries)
		  {
		    if(IsOutputQuery(node, lfirsti(iter)))
		      {
			elog(FATAL, "a dependent query cannot be an output query");
		      }
		    oid = GetRecurseRelation(node, lfirsti(iter));
		    if(oid == InvalidOid)
		      {
			elog(FATAL, " a dependent query must send its output to another query");
		      }
		    else
		      {
			elog(DEBUG1, " query %d sends its output to relation with oid %d", lfirsti(iter), oid);
		      }
		  }
		  
		  if (pqe->mctx)
		    {
		      SHMContextDelete(pqe->mctx);
		      pqe=NULL;
		      
		    }
		  if (pqe)
		    {
		      pfree(pqe);
		      pqe = NULL;
		    }


		    }
		}
	}
    return did_work;
}

/*
 *	canRemoveQuery -- this function returns true if it is safe to
 *	remove queries (and possibly sources and modules) from the eddy
 *	right now.
 */
bool
canRemoveQuery(Eddy * node)
{
	return (node->eddystate->ihtValid == false && empty(node));
}

/*
 * processOutputQ
 *
 * This routine checks the Eddy's intermediateHeapTuple to see if it should be
 * output to any query. If so, it sends it to the appropriate output.
 *
 * Returns TRUE if it did anything, FALSE if it did nothing.
 */
static bool
processOutputQ(Eddy * node, DR_resultqdest * resultqdest)
{
   IntermediateHeapTuple tmpiht= NULL;
	int							 i 			   = 0;
	bool						 needToOutput  = false;
	bits8						 scratch[BASIZE(TCQMAXOPERATORS)];
	EddyState  					*eddystate 	   = node->eddystate;
	IntermediateHeapTuple 		 iht 		   = eddystate->iht;
    bool        				 did_something = false;
	List 						*queries 	   = node->eddystate->queryIds;
	List 						*ql;
	bool            faggDelivered=false;
	
	if (iht == NULL) 
	{
		elog(DEBUG1, "iht in processOutputQ is null");
	
	}	
	BitArrayClearAll(resultqdest->whereToSendOutput, TCQMAXQUERIES);

	foreach (ql, queries)
	{
	        BitArray	cmask;
	        BitArray	done;


		i = lfirsti(ql);

		cmask = eddystate->completionMask[i]; /* Mask for query i */
		done = iht->t_done;					 /* Done bits for IHT */

		BitArrayAnd(cmask, done, scratch, TCQMAXOPERATORS);

		if (BitArrayEqual(scratch, cmask, TCQMAXOPERATORS))
		{
			if (!BitArrayBitIsSet(iht->t_queriesCompleted, i))
			{
				BitArraySetBit(iht->t_queriesCompleted, i);
				BitArraySetBit(resultqdest->whereToSendOutput, i);
				
				needToOutput = true;
			}
		}
	}


	if (needToOutput)
	{
	  faggDelivered = deliverTupleToFAgg(node,iht, resultqdest->whereToSendOutput,faggDelivered);
	  
	  did_something = true;

/* 		elog(DEBUG1,"**********************************************************************"); */
/* 		elog(DEBUG1, "GOT ONE: finished proc. a tuple... sending to output"); */
/* 		elog(DEBUG1, "NEED TO OUTPUT: completionMask for query 1, iht->t_done, iht->t_ready, iht->t_queriesCompleted"); */
/* 		BitArrayPrintDebug(eddystate->completionMask[0],TCQMAXOPERATORS); */
/* 		BitArrayPrintDebug(iht->t_done,TCQMAXOPERATORS); */
/* 		BitArrayPrintDebug(iht->t_ready,TCQMAXOPERATORS); */
/* 		BitArrayPrintDebug(iht->t_queriesCompleted,TCQMAXOPERATORS); */
/* 		debugitup_debug(iht); */
/* 		elog(DEBUG1,"**********************************************************************"); */

		elog(DEBUG1, "GOT ONE: finished proc. a tuple... sending to output");
/* #ifdef AMOLDEBUGON */
		debugitup_debug(iht);
/* #endif */

		if (! UseSuppressOutput) 
		{
			resultqdest->pub.receiveTuple(
				(HeapTuple)(iht),
				NULL,	/* Boo Hiss */
				(DestReceiver *) resultqdest);
		}
		while ((tmpiht = getFAggResult(node, &tmpiht,
									   resultqdest->whereToSendOutput)))
		{
			if (! UseSuppressOutput) 
			{
				resultqdest->pub.receiveTuple(
					(HeapTuple)(tmpiht),
					NULL,	/* Boo Hiss */
					(DestReceiver *) resultqdest);
			}
		}
	}
	else
	{
/* 		elog(DEBUG1,"**********************************************************************"); */
/* 		elog(DEBUG1, "NO NEED TO OUTPUT: completionMask for query 1, iht->t_done, iht->t_ready, iht->t_queriesCompleted"); */
/* 		BitArrayPrintDebug(eddystate->completionMask[0],TCQMAXOPERATORS); */
/* 		BitArrayPrintDebug(iht->t_done,TCQMAXOPERATORS); */
/* 		BitArrayPrintDebug(iht->t_ready,TCQMAXOPERATORS); */
/* 		BitArrayPrintDebug(iht->t_queriesCompleted,TCQMAXOPERATORS); */
/* 		debugitup_debug(iht); */
/* 		elog(DEBUG1,"**********************************************************************"); */
	}

    return did_something;
}

bool deliverTupleToFAgg(Eddy * cur, IntermediateHeapTuple iht,
			bits8 * whereToSendOutput ,bool faggDelivered)
{
  bool enqsucc=false;
  int i=0;
  int32 queryid=0;

 if(faggDelivered) return faggDelivered;

  for(i = 0; i < cur->numModules; i++)
    {
      List *li=NIL;
      FAgg * a = (FAgg *)cur->modules[i];
      if(!a || 
	 !IsA(a, FAgg)) continue;



      foreach(li, a->fjord.usedByQueries)
	{
	  queryid = lfirsti(li);
	  if(BitArrayBitIsSet(whereToSendOutput,queryid))
	    {
			aggtup++;
	      FJORD_ENQ_INP_PTR(enqsucc,a,&iht);
	      ProcessNextNode((Plan*)a, (Plan*)cur);
	      BitArrayClearBit(whereToSendOutput,queryid);
	      Assert(enqsucc);
	    }
	}
    }
  return faggDelivered;
}

/** 
 * Pull the next available result tuple from any FAgg node's operator tree.  
 */
IntermediateHeapTuple  getFAggResult(Eddy * cur, IntermediateHeapTuple * piht,
				     bits8 *  whereToSendOutput)
{
    int i=0;
    List * li = NIL;
    int queryid=0;
    bool enqsucc=false;
    *piht = NULL;
    for(i = 0; i < cur->numModules; i++)
    {
        FAgg * a = (FAgg *)cur->modules[i];
        if(!a || !IsA(a, FAgg)) continue;

        /* If we get here, <a> is an FAgg node.  See if it has any results
         * available. */
        FJORD_DEQ_OUT_PTR(enqsucc,a,&piht);
        if(!enqsucc)
        {
            /* No results yet.  Tell the FAgg to produce some. */
            ProcessNextNode((Plan *)a, (Plan*)cur);
            FJORD_DEQ_OUT_PTR(enqsucc,a,&piht);
        } 
        if(!enqsucc) continue;

        /* TEMPORARY: Ignore NULL output tuples. */
        /*
        if (NULL == (*piht)->t_base_tuples[0]) {
            elog(LOG, "Ignoring an empty FAgg result.");
            continue;
        }
        */

        Assert(NULL != (*piht)->t_base_tuples[0]);

        /* If we get here, the FAgg node returned a result. */
        BitArrayClearAll(whereToSendOutput, TCQMAXQUERIES);
        foreach(li, a->fjord.usedByQueries)
        {
            queryid = lfirsti(li);
            BitArraySetBit(whereToSendOutput, queryid);
        }
        return *piht;

    }
    return NULL;
}

/*
 * buildStems
 *
 * Builds the source's intermediateHeapTuple into all matching FSteMs
 */
void buildStems(Eddy *node, IntermediateHeapTuple iht, int sourceID)
{
	bool ret = buildStemsNoDup(node, iht, sourceID, false);
	if(ret == false)
		{
			elog(FATAL, "buildStemsNoDup returned false from buildStems");
		}
}


/* returns true if build stems worked, false if not. */
bool
buildStemsNoDup(Eddy * node, IntermediateHeapTuple iht, int sourceID, bool nodup)
{

	int			i;
	bool enqsucc=false;
	BitArray	stems = node->eddystate->sourceStems[sourceID];

	for (i = 0; i < TCQMAXOPERATORS; i++)
	{
		if (BitArrayBitIsSet(stems, i))
		{
			Fjord	   *fjord = (Fjord *) (node->modules[i]);
			if(nodup && FSteMContainsTuple(((FSteM*)node->modules[i]), iht,node))
				return false;
			FJORD_ENQ_INP_PTR(enqsucc,fjord,&iht);
			ProcessNextNode(node->modules[i], (Plan *) node);
			BitArraySetBit(iht->t_done, i);		/* See explanation below */

		}
	}

	/*
	 * Note that the for-loop above also sets the done bits of this tuple
	 * for all stems it's built in. This is because this tuple can go on
	 * to be a probe tuple into other stems and it must record the fact
	 * that it has already visited all stems built on this source.
	 */

	/*
	 * Now set the ready bits.
	 */
	BitArrayClone(iht->t_ready,
				  node->eddystate->operators[sourceID], TCQMAXOPERATORS);
#ifdef DEBUG_CQEDDY
	elog(DEBUG1, "Set IHT's bitarray to: \n");
	BitArrayPrintDebug(iht->t_ready, TCQMAXOPERATORS);
#endif

	return true;
}


static void
increment_current_tuples_returned(Eddy * node, Node *cur)
{
	int			i;

	Assert (IsA_Fjord(cur));

	if (IsA(cur,ScanModule)) 
	{
		return;
	}

	i = ((Fjord *) cur)->operator;
	
/* 	for (i = 0; i < node->numModules; i++) */
/* 	{ */
/* 		if (node->modules[i] == NULL) */
/* 			continue; */

/* 		if ((void *) node->modules[i] == (void *) cur) */
/* 		{ */
			node->eddystate->current_tuples_returned[i]++;
			return;
/* 		} */
/* 	} */
	/* It is probably a source module */
	/* Assert(0); */
}

static void
update_weights(Eddy * node, Node *popped)
{
	int			i;

	Assert(IsA_Fjord(popped));

	i = ((Fjord *) popped)->operator;

/* 	for (i = 0; i < node->numModules; i++) */
/* 	{ */
/* 		if (node->modules[i] == NULL) */
/* 			continue; */

/* 		if ((void *) node->modules[i] == (void *) popped) */
/* 		{ */
			if (node->eddystate->current_tuples_returned[i] > 1)
			{
				node->eddystate->weights[i] = node->eddystate->weights[i] * .95 +
					5 * (node->eddystate->current_tuples_returned[i] - 1);
			}
			else
				node->eddystate->weights[i] = node->eddystate->weights[i] * .95;
			if (node->eddystate->weights[i] < 1)
				node->eddystate->weights[i] = 1;
			node->eddystate->current_tuples_returned[i] = 0;
			return;
/* 		} */
/* 	} */
/* 	Assert(0); */
}

static Node *
choose_random_module(Eddy * node, BitArray ready)
{
	BitIndex	position = random() % TCQMAXOPERATORS;

	while (!BitArrayBitIsSet(ready, position))
		position = (position + 1) % TCQMAXOPERATORS;
	return (Node *) node->modules[position];
}


static Node *
choose_based_on_selectivity(Eddy * node, BitArray ready)
{
	Node	   *module = NULL;

	double		cumulative_wt = 0,
				total_wt = 0,
				random_wt;
	int			i;

	for (i = 0; i < node->numModules; i++)
	{
		if (node->modules[i] == NULL)
			continue;
		if (BitArrayBitIsSet(ready, i))
			total_wt += 1 / node->eddystate->weights[i];
	}

	random_wt = drand48() * total_wt;
	for (i = 0; i < node->numModules; i++)
	{
		if (node->modules[i] == NULL)
			continue;
		if (BitArrayBitIsSet(ready, i))
		{
			cumulative_wt += 1 / node->eddystate->weights[i];

			if (cumulative_wt > random_wt)
				return (Node *) node->modules[i];
		}
	}


	while ((module = choose_random_module(node, ready)) == NULL);
	return module;
}

#if 0

/* chooseEarlyAccessModule()
*
* This function lets special operators, like FEventAggs, have a source tuple 
* that they need before anyone else gets it. This is important because FEventAgg operators
* do not compose with SteMs... they do not accept multi-source tuples sent to them.
*
*    - TCQ SR
*/
static Node *
chooseEarlyAccessModule(Eddy * node, BitArray ready)
{

  int i;
  
  for (i = 0; i < node->numModules; i++)
    {
      if (node->modules[i] == NULL)
	continue;
      if (BitArrayBitIsSet(ready, i))
	{
	  if(IsA(node->modules[i], FEventAgg)) /* FEventAgg is the only "special" module currently */
	    return (Node *)node->modules[i];
	}
	
    }

  return NULL;

}

#endif /* 0 */


/*
 *	routeCQEddy
 *
 *	This routine is responsible for choosing the next operator for the Eddy to
 *	invoke. It implements the Eddy's routing policy. In addition to choosing an
 *	operator, it also informs the Eddy of the "type" of the operator chosen
 *	(SOURCE vs MODULE) as well as if that operator expects a fresh tuple to be
 *	enqueued.
 *
 *	node	 (IN)  Eddy node
 *	ptype	 (OUT) The "type" of operator chosen (SOURCE/MODULE)
 *	pmustEnq (OUT) Whether or not the Eddy must enqueue its current IHT
 *
 */
static Node *
routeCQEddy_new_but_not_in_use(Eddy * node, EddyItem * ptype, bool *pmustEnq)
{
	EddyState  *eddystate = node->eddystate;
	IntermediateHeapTuple iht = eddystate->iht;
	Node	   *module;

	Assert(0);

	routeCQEddy_new_but_not_in_use(node, ptype, pmustEnq);		/* Stop that annoying
																 * warning */

	*ptype = EDDY_MODULE;

	if (eddystate->ihtValid)
	{							/* There is a possibly valid tuple in the
								 * eddy The module on stack-top has NOT
								 * been drained */
		BitArray	ready = iht->t_ready;


		if (!BitArrayIsAnyCleared(iht->t_queriesCompleted, TCQMAXQUERIES))
		{
			/*
			 * Note that this case is particularly inefficient for modules
			 * such as GSFilter which DO NOT produce more than one tuple
			 * at a time.
			 *
			 * It may be well worth having a special case here to test if
			 * top(node) is such a module and avoid trying to explicitly
			 * drain it.
			 *
			 */
			eddystate->ihtValid = false;		/* The tuple is actually
												 * done */
			*pmustEnq = false;	/* The module has NOT been drained */
			if (!empty(node))
				return (top(node));
		}
		else
		{
			/** We can not have that some query is interested in this tuple, and
			 * there is nowhere to go */
			Assert(BitArrayIsAnySet(ready, TCQMAXOPERATORS) || (BitArrayPrint(ready, TCQMAXOPERATORS), false));
			*pmustEnq = true;	/* Must enqueue */
			while ((module = choose_random_module(node, ready)) == NULL);
			push(node, module);
			return module;
		}
	}
	else
	{							/* No valid tuple in the eddy */
		*pmustEnq = false;		/* Ergo, nothing to enqueue */
		if (!empty(node))
		{
			Node	   *popped = pop(node);		/* Module on top is
												 * drained, pop it */

			/*
			 * We must also update the weight for this module based on how
			 * many tuples it returned.
			 */
			update_weights(node, popped);
		}
		if (!empty(node))
		{
			return top(node);	/* Backtrack: return the new stack top */
		}
	}

	if (eddystate->ihtValid == false)
	{
		processQueryRemove(node);
		if (node->eddystate->numqueries == 0)
			return NULL;
	}
	*ptype = EDDY_SOURCE;		/* So, pick a source */
	return (Node *) (node->sources[chooseSourceRandom(node)]);
}

/*
 *	routeCQEddy
 *
 *	This routine is responsible for choosing the next operator for the Eddy to
 *	invoke. It implements the Eddy's routing policy. In addition to choosing an
 *	operator, it also informs the Eddy of the "type" of the operator chosen
 *	(SOURCE vs MODULE) as well as if that operator expects a fresh tuple to be
 *	enqueued.
 *
 *	node	 (IN)  Eddy node
 *	ptype	 (OUT) The "type" of operator chosen (SOURCE/MODULE)
 *	pmustEnq (OUT) Whether or not the Eddy must enqueue its current IHT
 *
 */
static Node *
routeCQEddy(Eddy * node, EddyItem * ptype, bool *pmustEnq)
{
	int			source = 0;

	bool		pickFromSource = false;
	EddyState  *eddystate = node->eddystate;
	IntermediateHeapTuple iht = eddystate->iht;
	Node	   *module;

	*ptype = EDDY_MODULE;

	if (eddystate->ihtValid)
	{							/* There is a possibly valid tuple in the
								 * eddy. The module on stack-top has NOT
								 * been drained */
		BitArray	ready = iht->t_ready;


		if (!BitArrayIsAnyCleared(iht->t_queriesCompleted, TCQMAXQUERIES))
		{
			/*
			 * Note that this case is particularly inefficient for modules
			 * such as GSFilter which DO NOT produce more than one tuple
			 * at a time.
			 *
			 * It may be well worth having a special case here to test if
			 * top(node) is such a module and avoid trying to explicitly
			 * drain it.
			 *
			 */
			eddystate->ihtValid = false;		/* The tuple is actually
												 * done */
			*pmustEnq = false;	/* The module has NOT been drained */
			if (!empty(node))
				return (top(node));
			else
			{					/* Continue processing top of stack */
				pickFromSource = true;
			}
		}
		else
		  {						/* This block returns from the function */
			if (BitArrayIsAnySet(ready, TCQMAXOPERATORS))
			{
				*pmustEnq = true;		/* Must enqueue */

				/* Before we make any smart routing decisions 
				* we want to let operators that want "early access"
				* to this tuple, have it... (e.g. FEventAgg)
				*                             - TCQ SR 
				*/
				/* Now that SteMs won't spit out tuples that 
				 * can go into the EventAgg (we fixed the SteM
				 * code), we don't need to give early access to 
				 * the FEventAgg
				 * Very soon, we should completely remove the 
				 * chooseEarlyAccessModule() function
				 *
				 * module = chooseEarlyAccessModule(node, ready);
				 * if(module)
				 *{
				 *
				 *elog(DEBUG1, "routeCQEddy: Chosen an early access module for the current IHT"); 
				 *
				 *
				 * push(node, module);
				 * return module;
				 * }
				 */
				

				if (enable_smart_routing)
					module = choose_based_on_selectivity(node, ready);
				else
					while ((module = choose_random_module(node, ready)) == NULL);
				push(node, module);
				return module;
			}
			else
			{
				*pmustEnq = false;
				eddystate->ihtValid = false;
				if (!empty(node))
					return top(node);
				else
					pickFromSource = true;
			}
		}
	}
	else
	{							/* No valid tuple in the eddy */
		*pmustEnq = false;		/* Ergo, nothing to enqueue */
		if (!empty(node))
		{
			Node	   *popped = pop(node);		/* Module on top is
												 * drained, pop it */

			update_weights(node, popped);
		}
		if (!empty(node))
		{
			return top(node);	/* Backtrack: return the new stack top */
		}
		else
		{						/* We've run out of modules */
			pickFromSource = true;
		}
	}



	if (pickFromSource == true)
	{
		if ((MaxTuplesScannedBeforeQuit != -1) &&
			(node->eddystate->total_base_tuples_remaining <= 0))
		{
			node->eddystate->shutdownPending = true;
			elog(INFO,"CQEddy Ending: processed %d tuples", MaxTuplesScannedBeforeQuit);
			return NULL;
		}
		if (eddystate->ihtValid == false)
		{
			processQueryRemove(node);
			if (node->eddystate->numqueries == 0)
				return NULL;
		}
		*ptype = EDDY_SOURCE;	/* So, pick a source */

		if(node->eddystate->numsourceswithbindings > 0 &&
		   (node->eddystate->iterations %
			node->eddystate->cqeddy_checkcancelediters) == 0 )
			{
			
				cancelFinishedQueries(node);
			}
 
		if(node->eddystate->timestampOrderedSourceSelection) /* We will select the one which has the lowest timestamp to offer */
		  {
		    source = chooseSourceTimestampOrder(node); /* - TCQ SR */
		    Assert(node->sources[source] != NULL);
		    /* Assert(!((ScanModule *) node->sources[source])->isDone);*/
		  }

		else /* We will select randomly */ 
		  {

		    while ((source = chooseSourceRandom(node)))
		      {
			if (node->sources[source] == NULL)
			  continue;
			if (!((ScanModule *) node->sources[source])->isDone)
			  break;
			
		      }

		  }

		return (Node *) (node->sources[source]);
	}
	else
	{
		Assert(1 == 0);
		return NULL;
	}
}

static void
printStack(Eddy *eddy)
{
	int i;
	EddyState *eddystate = eddy->eddystate;

	elog(DEBUG1,"------------------------------------------------------------");
	elog(DEBUG1,"Stack:");
	for (i = 0; i <= eddystate->stackptr; i++)
	{
		Fjord *f = (Fjord*)(eddystate->stack[i]);
		List  *tmolist = f->thisMOlist;
		List  *nm;

		elog(DEBUG1,"[%d] [%d] ",i,f->operator);
		elog(DEBUG1,"\tThisMOList: ");
		foreach (nm, tmolist) 
		{
			PrintMetaOperator(eddy, (MetaOperator *)lfirst(nm));
		}
	}
	elog(DEBUG1,"------------------------------------------------------------");
}


/*
 *  getNextMetaOp
 *
 *  PRECONDITION:
 *
 *     BitArrayIsAnySet(eddy->eddystate->iht->t_ready,TCQMAXOPERATORS) == false
 *
 */
static List*
getNextMetaOp(Eddy *eddy)
{
	EddyState 					*eddystate 	 = eddy->eddystate;
	IntermediateHeapTuple 		 iht 	 	 = eddystate->iht;
	List 						*nmolist 	 = NIL;
	List 						*nl 		 = NIL;
	MetaOperator 				*mo;
	int 						 driver  	 = eddystate->driver;
	Word_t 						 Index;
	Pvoid_t 					 transitions = eddystate->transitions[driver];
	Word_t 						*Pvalue;
	bits8   					 notdone[BASIZE(TCQMAXOPERATORS)];

	Index = GetBitArrayAsWord_t(iht->t_sources, TCQMAXSOURCES);
	
	JLG (Pvalue, transitions, Index);
	
#ifdef AMOLDEBUGON
	elog(DEBUG1,"gNMO: Index: %x\n",(int)Index);
#endif

	if (Pvalue == (Word_t *) NULL) 
	{
#ifdef AMOLDEBUGON
		elog(DEBUG1,"No more transitions !");
#endif
		/* No more transitions ! */
		return NIL;
	}
	
	nmolist = (List *) (*Pvalue);
	
	foreach (nl, nmolist) 
	{
		mo = (MetaOperator *) lfirst(nl);

#ifdef AMOLDEBUGON
		elog(DEBUG1,"Testing MO - visited ?");
#endif
		
		if (! mo->visited)
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1,"\t No !");
			elog(DEBUG1,"Current MetaOperator is :");
			PrintMetaOperator(eddy, mo);
#endif
			/* Notdone = ~ iht->t_done */
			BitArrayNot(notdone,iht->t_done,TCQMAXOPERATORS);
			/* iht->t_ready = mo->ready & Notdone */		
			BitArrayAnd(mo->ready,notdone,iht->t_ready,TCQMAXOPERATORS);
			if (! BitArrayIsAnySet(iht->t_ready,TCQMAXOPERATORS)) 
			{
				continue;
			}
			return nl;
		}
	}
	return NIL;
}

/*
 *	routeCQEddyOpPrec
 *
 *	This routine is responsible for choosing the next operator for the Eddy to
 *	invoke. It implements the Eddy's routing policy. In addition to choosing an
 *	operator, it also informs the Eddy of the "type" of the operator chosen
 *	(SOURCE vs MODULE) as well as if that operator expects a fresh tuple to be
 *	enqueued.
 *
 *  Note that when this module picks a source it also sets the eddystate->driver
 *
 *	node	 (IN)  Eddy node
 *	ptype	 (OUT) The "type" of operator chosen (SOURCE/MODULE)
 *	pmustEnq (OUT) Whether or not the Eddy must enqueue its current IHT
 *
 */
static Node *
routeCQEddyOpPrec(Eddy * node, EddyItem * ptype, bool *pmustEnq)
{
	int	source = 0;

	bool						 pickFromSource = false;
	EddyState  					*eddystate 		= node->eddystate;
	IntermediateHeapTuple 		 iht 			= eddystate->iht;
	Node	   					*module;
	List 						*tmolist 		= NIL;
	MetaOperator *mo;
				
	*ptype = EDDY_MODULE;

#ifdef AMOLDEBUGON
	if (goodtoprint) 
	{
		elog(DEBUG1,"------------------------------------------------------------");
		elog(DEBUG1,"Entering routeCQEddyOpPrec ............. ");
		printStack(node);
	}
#endif
		
	if (eddystate->ihtValid)
	{							/* There is a possibly valid tuple in the
								 * eddy. The module on stack-top has NOT
								 * been drained */
		BitArray	ready = iht->t_ready;

#ifdef AMOLDEBUGON		
		debugitup_debug(iht);
#endif
		
		if (BitArrayIsAnySet(ready, TCQMAXOPERATORS) ||
			((tmolist = getNextMetaOp(node)) != NIL))
		{
			*pmustEnq = true;		/* Must enqueue */

			if (enable_smart_routing)
				module = choose_based_on_selectivity(node, ready);
			else
				while ((module = choose_random_module(node, ready)) == NULL);
			push(node, module);
			((Fjord *)module)->thisMOlist = tmolist;
			if (tmolist != NIL) 
			{
				((MetaOperator*)lfirst(tmolist))->visited = true;
			}
			
#ifdef AMOLDEBUGON
			elog(DEBUG1,"Returning module %d ",((Fjord*)module)->operator);
			elog(DEBUG1,"------------------------------------------------------------");
#endif
			return module;
		}
		else
		{
			*pmustEnq = false;
			eddystate->ihtValid = false;
			if (!empty(node)) 
			{
#ifdef AMOLDEBUGON
				if (goodtoprint) 
				{
					elog(DEBUG1,"Returning from top of the stack ... ");
					elog(DEBUG1,"Returning module %d ",((Fjord*)top(node))->operator);
					elog(DEBUG1,"------------------------------------------------------------");
				}
#endif
				return top(node);
			}
			else
			{
#ifdef AMOLDEBUGON
				if (goodtoprint) 
				{
					elog(DEBUG1,"Need to pick from source ");
					elog(DEBUG1,"------------------------------------------------------------");
				}
#endif
				pickFromSource = true;
			}
		}
	}
	else
	{							/* No valid tuple in the eddy */
#ifdef AMOLDEBUGON
		if (goodtoprint)
		{
			elog(DEBUG1,"No valid tuple ... ");
		}
#endif
		*pmustEnq = false;		/* Ergo, nothing to enqueue */
		if (!empty(node))
		{
			Node	   *popped = pop(node); /* Module on top drained, pop it */

			tmolist = ((Fjord *)popped)->thisMOlist;

			if (tmolist != NIL) 
			{
				mo = (MetaOperator *) lfirst(tmolist);

				if (! mo->visited) 
				{

					elog(DEBUG1, "Oh oh .. we're in trouble .. print the mo");
					PrintMetaOperator(node,mo);
					elog(DEBUG1, "Oh oh .. we're in trouble .. print the stack");
					printStack(node);
					PrintMetaOperator(node,mo);
				}
				Assert(mo->visited);
				mo->visited = false;
			}

#ifdef AMOLDEBUGON
			elog(DEBUG1,"Drain top module in stack .. ");
			elog(DEBUG1,"Popped operator %d ", ((Fjord *)popped)->operator);
#endif
			update_weights(node, popped);
		}
		if (! empty(node))
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1,"Returning from top of the stack ... ");
			elog(DEBUG1,"Returning module %d ",((Fjord*)top(node))->operator);
			elog(DEBUG1,"------------------------------------------------------------");
#endif
			return top(node);	/* Backtrack: return the new stack top */
		}
		else
		{						/* We've run out of modules */
#ifdef AMOLDEBUGON
			if (goodtoprint)
			{
				elog(DEBUG1,"Pick from source");
				elog(DEBUG1,"------------------------------------------------------------");
			}
#endif
			pickFromSource = true;
		}
	}

	if (pickFromSource == true)
	{
		if ((MaxTuplesScannedBeforeQuit != -1) &&
			(node->eddystate->total_base_tuples_remaining <= 0))
		{
			node->eddystate->shutdownPending = true;
			elog(INFO,"CQEddy Ending: processed %d tuples", MaxTuplesScannedBeforeQuit);
			return NULL;
		}
/* 		if (eddystate->ihtValid == false) */
/* 		{ */
/* 			processQueryRemove(node); */
/* 			if (node->eddystate->numqueries == 0) */
/* 				return NULL; */
/* 		} */
		*ptype = EDDY_SOURCE;	/* So, pick a source */
		/* cancelFinishedQueries(node); */
		source = chooseSourceRandom(node);
/* 		while ((source = chooseSourceRandom(node))) */
/* 		{ */
/* 			if (node->sources[source] == NULL) */
/* 				continue; */
/* 			if (!((ScanModule *) node->sources[source])->isDone) */
/* 				break; */
/* 		} */
		if (UseOperatorPrecedence) 
		{
			eddystate->driver = source;
		}
		if (goodtoprint) 
		{
#ifdef AMOLDEBUGON
			elog(DEBUG1,"Returning source .. %d ",source);
#endif
		}
		return (Node *) (node->sources[source]);
	}
	else
	{
		Assert(1 == 0);
		return NULL;
	}
}

/* ----------------------------------------------------------------
 *		ExecEddy(node)
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecCQEddy(Eddy * node)
{
	EddyState  *eddystate;
	IntermediateHeapTuple *piht;
	bool		iterationsUntilCheck = 0;
	bool		mustEnq;
	Plan	   *module;
	EddyItem	mtype;
	DR_resultqdest *resultqdest;
	bool		enqsucc;
    bool        did_work_this_iteration = true;
    int         sleep_usec = -1;
    bool        ret = false;
    int         maxqueries = TCQMAXQUERIES;

	if (UseStaticPlanNumber != -1)
	{
		maxqueries = MyStaticPlans[UseStaticPlanNumber].num_queries;
	}

	elog(LOG,"Check: maxqueries is %d", maxqueries);
	elog(LOG,"MaxTuplesScannedBeforeQuit is %d", MaxTuplesScannedBeforeQuit);
	
	eddystate = node->eddystate;

	resultqdest = (DR_resultqdest *) DestToFunction(ResultQueue);

	/* TBD TBD TBD - Check this setup ! */

	resultqdest->pub.setup((DestReceiver *) resultqdest, 
            CMD_SELECT, NULL, NULL);
	resultqdest->isIHT = true;
	resultqdest->eddy = node;

    /* Prepare to listen for stream data connections. */
    setupServerSock(node);

    did_work_this_iteration = true;


	do
	{
        /* Backoff.  Each time that we go through the loop without doing any 
         * work, we sleep a little longer.  If we get any work done, we set our
         * sleep timer back to the minimum amount of time. */
        node->eddystate->iterations++;
	    if (did_work_this_iteration) 
	    {
			sleep_usec = CQ_EDDY_MIN_SLEEP_TIME;
	    }
		else 
		{
            sleep_usec = Min(sleep_usec + CQ_EDDY_SLEEP_TIME_INCR, 
							 CQ_EDDY_MAX_SLEEP_TIME);
        }
        if (sleep_usec >= CQ_EDDY_SLEEP_TIME_THRESH) 
		{
            tcq_usleep(sleep_usec);
        }

		/* Assume false until told otherwise... */
		did_work_this_iteration = false;
		
		CHECK_FOR_INTERRUPTS();

        /* Maintain our set of stream data connections. */
        acceptSrvrConns(node);
        getStreamNames(node);
        attachSocksToStrms(node);
        suckUpUnusedTuples(node);
        

        /* TODO: Refactor!!! */
		if (((iterationsUntilCheck-- <= 0) ||
		 	 (sleep_usec >=CQ_EDDY_SLEEP_TIME_THRESH)) &&
            (num_queries < maxqueries))
		{
        
            ret = processControlQ(node);		 /* Process ControlQ Messages */
            if (ret)	 
				did_work_this_iteration = true;
			if (node->eddystate->shutdownPending)  
			{
				elog(LOG,"Hmm .. eddystate->shutdownPending is true");
				return NULL;
			}
			
			if (! (UseStaticPlan && (StaticEddy != NULL)))
			{
				ret = processPlanQ(node);					/* Process PlanQ */ 
				if (ret) 
					did_work_this_iteration = true;
			}
			iterationsUntilCheck = EDDYINPUTCHECKINTERVAL;
		}
		else /* if (num_queries >= 2) */ 
		{
		  did_work_this_iteration = true;
		  if (node->eddystate->shutdownPending)  
		  {
			  elog(LOG,"Hmm .. eddystate->shutdownPending is true");
			  return NULL;
		  }
	    }

		if (UseStaticPlan) 
		{
			if (StaticEddy != NULL)
			{
				runStaticPlan(node);
				did_work_this_iteration = true;
			}
		}
		else 
		{

			if (! UseOperatorPrecedence)					  /* Find next module */
			{
			  if((node->eddystate->iterations % 100 == 0))
			    {
			      static Const * stopint = NULL;
			      if(stopint == NULL)
				{
				  Const *tmp=NULL;
				  tmp  = make_const(makeString("60 seconds"));
				  stopint = (Const*) coerce_type((Node *) tmp, tmp->consttype,
								 (Oid) INTERVALOID,
								 COERCION_IMPLICIT, COERCE_IMPLICIT_CAST);
				}


			      checkSlideTimeout(node, resultqdest);

			      /* MemoryContextReset(node->eddystate->tempContext);*/ /* Piggy-backing on this "once-in-100-iterations" thing - TCQ SR */
			      /* As no one is using the Eddy's tempContext as of now.... */

			    }

				module = (Plan *) routeCQEddy(node, &mtype, &mustEnq);	
			}
			else 
			{												  
				module = (Plan *) routeCQEddyOpPrec(node, &mtype, &mustEnq);
#ifdef AMOLDEBUGON
				if (goodtoprint) 
				{
					elog(DEBUG1,"Exiting routeCQEddyOpPrec ............. ");
					printStack(node);
					elog(DEBUG1,"------------------------------------------------------------");
				}
#endif
			}
		
			if (module == NULL)
			{
				did_work_this_iteration = true; /* Want to check next op ASAP */
				continue;
			}

#ifdef AMOLDEBUGON
			if (goodtoprint) 
			{
				elog(DEBUG1,"Next module is: %d ",((Fjord *)module)->operator);
			}
#endif

			piht = &(eddystate->iht);
			if (mustEnq) 
			{
#ifdef AMOLDEBUGON
				elog(DEBUG1,"Enqueueing tuple to module ...");
#endif
				FJORD_ENQ_INP_PTR(enqsucc, module, piht);
				did_work_this_iteration = true;
			}
#ifdef AMOLDEBUGON
			if (goodtoprint)
			{
				elog(DEBUG1,"Calling module .. ");
			}
			elog(DEBUG1, "CQEddy main Loop: Calling ProcessNextNode on some module");
#endif
			ProcessNextNode(module, (Plan *) node);
#ifdef AMOLDEBUGON
			if (goodtoprint)
			{
				elog(DEBUG1,"Done calling module .. ");
			}
#endif
			FJORD_DEQ_OUT_PTR(eddystate->ihtValid, module, &piht);
			if (eddystate->ihtValid)
			{
				int			operator = ((Fjord *) module)->operator;

				eddystate->iht = *piht;
				
				goodtoprint = true;

				if (mtype == EDDY_MODULE)
				{
					BitArrayClearBit(eddystate->iht->t_ready, operator);/* Clear ready bit */
					BitArraySetBit(eddystate->iht->t_done, operator);/* Set done bits */
				}
#ifdef AMOLDEBUGON
				elog(DEBUG1,"*********************************************************************");
				debugitup_debug(eddystate->iht); 
				elog(DEBUG1,"*********************************************************************");
#endif
				increment_current_tuples_returned(node, (Node *) module);
				node->numTuplesProcessed++;

				did_work_this_iteration = true;

				if (! UseOperatorPrecedence) 
				{
					ret = processOutputQ(node, resultqdest);/* Process OutputQ */
					if (ret)
						did_work_this_iteration = true;
				}
			}
		}
	}
	while (1);

	assert(!"CQEddy can't end.");
	return NULL;
}

/* ----------------------------------------------------------------
 *		ExecEndCQEddy
 *
 *		frees any storage allocated through C routines.
 * ----------------------------------------------------------------
 */
void
ExecEndCQEddy(Eddy * node)
{
	int			i = 0;
	List	   *listentry = NIL;
	int			queryid = 0;
	result		r;

	r.type = RESULT_DONE;

	elog(LOG,"Going to call printEddyStats_debug");
	printEddyStats_debug(node);
	elog(LOG,"Going to call printEddyStats_debug");

	if (UseStaticPlan && (node != StaticEddy))
	{
		EddyState *eddystate;
		
		/* if we're using a static plan, End the Eddy correctly to get FOutput output */
		elog(LOG,"Processing EndCQEddy for StaticEddy");

		/*
		 * create new EddyState for StaticEddy
		 */
		eddystate = makeNode(EddyState);
		StaticEddy->eddystate = eddystate;
		StaticEddy->rangetable = node->rangetable;
		eddystate->debugctx = node->eddystate->debugctx;
		eddystate->queryIds = node->eddystate->queryIds;
		
		ExecEndCQEddy(StaticEddy);
		return;
	}
	else 
	{
		for (i = 0; i < node->numSources; i++)
		{
			if (node->sources[i] == NULL)
				continue;
			ProcessEndNode((Plan *) node->sources[i], (Plan *) node);
		}

		for (i = 0; i < node->numModules; i++)
		{
			if (node->modules[i] == NULL)
				continue;
			ProcessEndNode((Plan *) node->modules[i], (Plan *) node);
		}
		foreach(listentry, node->eddystate->queryIds)
			{
				/*
				 * notify each of the fontends that the eddy is shutting down.
				 * this is probably superfulious as the queryies will go away
				 * anyway as a result of signals from the postmaster, but it can't
				 * hurt.
				 */
				queryid = lfirsti(listentry);

				enqueue(GetQueue(queryid), (char *) &r, false, &result_serializer);
			}
	}
}

HeapTuple
EddyExecProject(Eddy * eddy, IntermediateHeapTuple iht, int32 queryid)
{
	ResetExprContext(eddy->eddystate->pstate.cs_ExprContext);
	eddy->eddystate->pstate.cs_ProjInfo =
		eddy->eddystate->resultProjectionInfo[queryid];

	return ProjectTuple(eddy->eddystate->pstate.cs_ProjInfo,
						eddy->eddystate->resultTupleDescriptors[queryid],
						true,
						(HeapTuple) iht);
}

HeapTuple
ProjectTuple(ProjectionInfo *pi,
			 TupleDesc resultDesc,
			 bool isIntermediate,
			 void *tuple)
{
	ExprDoneCond isDone;
	TupleTableSlot *tts = NULL;
	HeapTuple	ret = NULL;

	pi->pi_slot->ttc_tupleDescriptor = resultDesc;
	pi->pi_exprContext->ecxt_scantuple->val = (HeapTuple) tuple;
	pi->pi_exprContext->ecxt_scantuple->ttc_isIntermediateTuple = isIntermediate;
	pi->pi_exprContext->ecxt_scantuple->ttc_shouldFree = false;
	tts = ExecProject(pi, &isDone);
	pi->pi_exprContext->ecxt_scantuple->val = NULL;
	pi->pi_exprContext->ecxt_scantuple->ttc_isIntermediateTuple = false;
	ret = tts->val;
	tts->val = NULL;
	return ret;

}

void
wakeup_eddy(void)
{
	struct planqentry pqe;

	memset(&pqe, 0, sizeof(struct planqentry));
	enqueue(*p_plan_queue,
			(char *) &pqe,
			false,
			&planqentry_serializer);
}


void
cancelFinishedQueries(Eddy * e)
{
	int i=0;
	List	   *iter = NIL;
	List       *iter2 = NIL;
	int			queryid;

	bits8   sourceRunning[BASIZE(TCQMAXSOURCES)];
	bits8   pullSources[BASIZE(TCQMAXSOURCES)];
	bits8   allSources[BASIZE(TCQMAXSOURCES)];
	bits8   scratch[BASIZE(TCQMAXSOURCES)];


	BitArrayRealClearAll(sourceRunning, TCQMAXSOURCES);
	BitArrayRealClearAll(pullSources, TCQMAXSOURCES);

	for( i=0; i < e->numSources; i++)
		{
			if(!isSourceFinished(e,i))
				{
					BitArraySetBit(sourceRunning,i);
				}
			if(isPullSource(e,i))
			  {
			    BitArraySetBit(pullSources,i);
			  }
		}



	foreach(iter, e->eddystate->queryIds)
	{

		queryid = lfirsti(iter);
		if(!IsOutputQuery(e, queryid))
			continue;

		BitArrayClone(allSources, e->eddystate->sources[queryid],TCQMAXSOURCES);
		foreach(iter2, e->eddystate->recursiveQueryDeps[queryid])
			{
				BitArrayOr(e->eddystate->sources[lfirsti(iter2)],
						   allSources,
						   allSources, 
						   TCQMAXSOURCES);
			}

		BitArrayAnd(allSources,
					sourceRunning,
					scratch,
					TCQMAXSOURCES);

		if (!BitArrayIsAnySet(scratch, TCQMAXSOURCES) )
		{
			result		r;
		  BitArrayAnd(allSources, pullSources,scratch,TCQMAXSOURCES);
		  if(!BitArrayIsAnySet(scratch,TCQMAXSOURCES)) continue;


			r.type = RESULT_DONE;
			enqueue((*shmResultQueues)[queryid],
					(char *) &r,
					false,
					&result_serializer);
		}

	}

}


bool  isSourceFinished(Eddy * e, int sourceid)
{
	ScanModule *cur = NULL;
	
	cur = (ScanModule *) e->sources[sourceid];
	if (cur == NULL)
		return true;
	if(!IsA(cur, ScanModule))
		elog(FATAL, "isSourceFinished assumed all sources are ScanModules");

	return IsScanModuleDone(cur,true);


}


bool  isPullSource(Eddy * e, int sourceid)
{
	ScanModule *cur = NULL;
	
	cur = (ScanModule *) e->sources[sourceid];
	if (cur == NULL)
		return true;
	if(!IsA(cur, ScanModule))
		elog(FATAL, "isSourceFinished assumed all sources are ScanModules");

	return ((ScanModule*)cur)->ispullsource;
}



/*#if 0 */
void
dumpEddy(Eddy * e)
{
	int			i;
	EddyState  *eddystate = e->eddystate;
	List	   *iter = NULL;
	PostJoinOperator *pjo;

	if (server_min_messages > DEBUG1)
	{
        elog(LOG, "Not dumping eddy...");
		return;					/* Don't mess with me */
	}

	elog(DEBUG1, "------------------------------------------------------------\n");
	elog(DEBUG1, "------------------------------------------------------------\n");
	elog(DEBUG1, "Number of sources: %d ", e->numSources);
	for (i = 0; i < e->numSources; i++)
	{
		Plan	   *source = e->sources[i];
		ScanModule *sm;
		Plan	   *scan;

		if (source == NULL)
			continue;
		sm = (ScanModule *) source;
		scan = sm->scan;

		elog(DEBUG1, "ScanModule: [%d] ", i);
		switch (nodeTag(scan))
		{
			case T_SeqScan:
				elog(DEBUG1, " SeqScan ");
				break;
			case T_StrmScan:
				elog(DEBUG1, " StrmScan ");
				break;
			default:
				Assert(!"dumpEddy: Bogus scan ");
				break;
		}
		elog(DEBUG1, " [oid:%d] \n", getRealSourceOid(source));
	}
	for (i = 0; i < e->numModules; i++)
	{
		Plan	   *module = e->modules[i];

		if (module == NULL)
			continue;
		elog(DEBUG1, "Module: [%d] ", i);
		switch (nodeTag(module))
		{
			case T_GSFilter:
				elog(DEBUG1, " GSFilter ");
				break;
			case T_FSteM:
				elog(DEBUG1, " FSteM ");
				break;
			case T_FOutput:
				elog(DEBUG1, " FOutput ");
				break;
			case T_FAgg:
				elog(DEBUG1, " FAgg ");
				break;
            default:
				elog(ERROR, "dumpEddy: Bogus module (type %d, eddy is %d)", 
                        nodeTag(module), T_Eddy);
				break;
		}
	}
	elog(DEBUG1, "\nDumping operators [only first 4] \n");
	for (i = 0; i < TCQMAXSOURCES; i++)
	{
/*		elog(DEBUG1,"operators[%d] = %x\n",i,eddystate->operators[i]); */
		elog(DEBUG1, "operators[%d]: ", i);
		BitArrayPrintDebug(eddystate->operators[i], TCQMAXOPERATORS);
	}
	elog(DEBUG1, "------------------------------------------------------------\n");
	elog(DEBUG1, "Dumping sourceStems [only first 4] \n");
	for (i = 0; i < TCQMAXSOURCES; i++)
	{
/*		elog(DEBUG1,"sourceStems[%d] = %x\n",i,eddystate->sourceStems[i]); */
		elog(DEBUG1, "sourceStems[%d]: ", i);
		BitArrayPrintDebug(eddystate->sourceStems[i], TCQMAXOPERATORS);
	}
	elog(DEBUG1, "------------------------------------------------------------\n");
	elog(DEBUG1, "Dumping completionmask [only first 1] \n");
	for (i = 0; i < TCQMAXQUERIES; i++)
	{
/*		elog(DEBUG1,"completionMask[%d] = %x\n",i,eddystate->completionMask[i]); */
		elog(DEBUG1, "completionMask[%d]: ", i);
		BitArrayPrintDebug(eddystate->completionMask[i], TCQMAXOPERATORS);

	}
	elog(DEBUG1, "------------------------------------------------------------\n");
	elog(DEBUG1, "Dumping PostJoinOperators \n");

	i = 0;
	foreach(iter, eddystate->pjolist)
	{
		pjo = (PostJoinOperator *) lfirsti(iter);
/*		elog(DEBUG1,"(%d) pjo->sources = %x\n", i, pjo->sources[0]); */
		elog(DEBUG1, "(%d) pjo->sources \n", i);
		BitArrayPrintDebug(pjo->sources, TCQMAXOPERATORS);
/*		elog(DEBUG1,"(%d) pjo->ready   = %x\n", i, pjo->ready[0]); */
		elog(DEBUG1, "(%d) pjo->ready \n", i);
		BitArrayPrintDebug(pjo->ready, TCQMAXOPERATORS);
	}
	elog(DEBUG1, "------------------------------------------------------------\n");
	elog(DEBUG1, "------------------------------------------------------------\n");
}
/*#endif */

int getTotalProbes(Eddy *node) 
{
	int t = 0, i = 0;

	for (i = 0; i < node->numModules; i++) 
	{
		Plan	   *module = node->modules[i];
		if (module == NULL)
			continue;
		if (IsA(module,FSteM)) 
		{
			t += (((FSteM *) module)->fstem_num_input_for_probes);
		}
	}
	return t;
}

static void
runStaticPlan(Eddy *node)
{
	Eddy *e = StaticEddy;
/* 	int numStaticSources = StaticEddy->numSources; */
	
/* 	StaticDriver = (StaticDriver + 1) % numStaticSources; */

	if (node->eddystate->total_base_tuples_remaining <= 0) 
	{
		node->eddystate->shutdownPending = true;
		elog(LOG,"CQEddy Ending: processed %d tuples", MaxTuplesScannedBeforeQuit);
		return;
	}
	StaticDriver = chooseSourceRandom(node);
	ProcessNextNode(e->sources[StaticDriver],(Plan *)node);
}


Oid GetRecurseRelation(Eddy * eddy, uint32 queryid)
{
	return eddy->eddystate->recursiveQueryFeedback[queryid];
}

bool IsOutputQuery(Eddy * eddy, uint32 queryid)
{
	return 	eddy->eddystate->recursiveQueryFeedback[queryid] == InvalidOid; 
}

/* return queryid of output query or -1 */

int findRecursiveOutputQueryId(Eddy * eddy, uint32 queryid)
{
	int idx=0;
	List *l=NIL;
	for(idx=0; idx < TCQMAXQUERIES; idx++)
		{
			if((l=eddy->eddystate->recursiveQueryDeps[idx]) &&
			   intMember(queryid, l))
				return idx;
		}
	return -1;
}
					
List * findOutputQueryList(Eddy * eddy, List * query_ids)
{
	List * newlist=NIL;
	List * iter=NIL;
	int qid=-1;
	foreach(iter, query_ids)
		{
			qid = lfirsti(iter);
			if(!IsOutputQuery(eddy, qid))
				{
					qid=findRecursiveOutputQueryId(eddy, qid);
				}
			if(qid >= 0)
				newlist=lconsi(qid, newlist);
		}
	return newlist;
}

static void checkSlideTimeout(Eddy * e, DR_resultqdest * resultqdest)
{

  IntermediateHeapTuple tmpiht=NULL; 

      while 
	( (tmpiht = getFAggResult(e, &tmpiht, 
				  resultqdest->whereToSendOutput)))
	{
	  
	  resultqdest->pub.receiveTuple(
					(HeapTuple)(tmpiht),
					NULL,	/* Boo Hiss */
					(DestReceiver *) resultqdest);
	}
}

List * find_queries_using_stream(Eddy * e, Oid streamoid)
{
  int i=0;
  ScanModule *scanmodule=NULL;
  List *qids=NIL;
  Oid tmpoid=0;
  int numSources = e->numSources;
  for(i=0; i < numSources; i++)
    {
      scanmodule=(ScanModule*)e->sources[i];
      if(scanmodule == NULL) continue;
      Assert(IsA(scanmodule,ScanModule));
      tmpoid = getSourceOid(scanmodule->scan, e);
      if(tmpoid != streamoid) continue;
      qids = nconc(qids, scanmodule->fjord.usedByQueries);
    }
  return qids;
}
      

/*
 * chooseSourceTimestampOrder()
 * 
 * This function will choose from the sources having a tuple to offer
 * the one which has the lowest timestamp (no greater than anyone else)
 * Written during our run for SIGMOD 2005 demo
 *
 *   - TCQ SR
 */

int 
chooseSourceTimestampOrder(Eddy * node)
{


  int currentMinSource = -1 , i;
  Timestamp currentMinTimestamp, cur;
  bool foundOne = FALSE;
      

  for(i = 0; i < node->numSources; i++) 
  {
	  
      if(node->sources[i] == NULL)
		  continue;
	  
      Assert(IsA(node->sources[i], ScanModule));

	  /* Not very sure whether such a source should be disregarded or given one
	   * more consideration... */
      if(((ScanModule *) node->sources[i])->isDone) 
		  continue;
      cur = lookAheadNextTupleTimestamp((ScanModule *)node->sources[i]);

#ifdef DEBUG_CQEDDY 
      elog(LOG, "chooseSourceTimestampOrder: Source %d returned Timestamp %d",
		   i, (int)cur);
#endif
	  
      if(!foundOne) 
	  {
		  currentMinSource = i;
		  currentMinTimestamp = cur;
		  foundOne = TRUE;
	  }
      else 
	  {
		  
		  if(ts_geq(currentMinTimestamp, cur)) 
		  {
			  currentMinSource = i;
			  currentMinTimestamp = cur;
		  }
	  }
    }


  if(!foundOne)
	  elog(FATAL, "chooseSourceTimestampOrder: Nothing to choose from... this is unexpected!");


  /*  elog(LOG, "chooseSourceTimestampOrder: Choosing source ID = %d", currentMinSource);*/
  return currentMinSource;
}

/*
 * Prepare to accept incoming connections for stream data.
 */
void setupServerSock(Eddy *node) 
{
	struct sockaddr_in serv_addr;
    int opt_true = 1;

    node->acceptSock = socket(AF_INET, SOCK_STREAM, 0);
    
    /* Create the socket on which we accept new connections */
	if (node->acceptSock < 0) {
		elog(ERROR, "Socket creation error");
	}

	/* Make sure we get the port we want, even if it's been used recently. */
	if (setsockopt(node->acceptSock, SOL_SOCKET, SO_REUSEADDR, 
                (void *) &opt_true,
				   sizeof(int)) < 0) {
		elog(ERROR, "setting SO_REUSEADDR failed");
    }

    /* Bind our local address so clients can connect to us */
	memset((void *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(TEMPORARY_DEFAULT_PORT);

	if (bind(node->acceptSock, (struct sockaddr *) &serv_addr, 
                sizeof(serv_addr)) < 0) 
    {
		elog(ERROR, "Could not bind to port for receiving connections: %s",
                strerror(errno));
	}

	listen(node->acceptSock, 5);		

    elog(LOG, "Eddy listening for connections on port %d",
            ntohs(serv_addr.sin_port));

	/* 
	 * Make the socket non-blocking so accept commands don't block the entire
     * backend.
	 */
	if (fcntl(node->acceptSock, F_SETFL, O_NONBLOCK) < 0) {
        elog(ERROR, "Couldn't enable nonblocking accept()");
	}

}


/*
 * Attempt to accept new stream data connections.
 */
void acceptSrvrConns(Eddy *node) 
{
    socklen_t len;
    SockState next;
    SockState *newState = NULL;

    memset(&next, 0x0, sizeof(next));

    /* Do nonblocking accepts until we don't have any more. */
    while (-1 != 
        (next.fd = accept(node->acceptSock, &(next.dataSourceAddr), &len)))
    {
        elog(LOG, "Got a stream data connection on socket %d",
                next.fd);
        /* Got a connection. */
        next.reloid = InvalidOid;


        /* Don't forget to make our reads nonblocking! */
        if (fcntl(next.fd, F_SETFL, O_NONBLOCK) < 0) {
            elog(ERROR, "Couldn't enable nonblocking read on socket %d",
                    next.fd);
        }

        /* Add our connection to the list of open connections. */
        newState = palloc(sizeof(*newState));
        memcpy(newState, &next, sizeof(next));

        newState->readBufUsed = 0;
        newState->readBufPos = 0;

        node->startingSocks = lcons(newState, node->startingSocks);

        memset(&next, 0x0, sizeof(next));
    }
}

/*
 * Attempt to get stream names for recently-opened data connections. 
 */
void getStreamNames(Eddy *node)
{
    SockState *sockstates[32];
    int nfds = 0;
    int i;
    List *itr = node->startingSocks;

    for (i = 0; i < 32 && itr != NULL; i++) {
        sockstates[i] = (SockState *)lfirst(itr);
        nfds++;
        itr = lnext(itr);
    }

    for (i = 0; i < nfds; i++) {
        int ret = fillReadBuf(sockstates[i]);

        if (ret < 0) {
            /* Socket is closed. */
            pfree(sockstates[i]);
            node->startingSocks = lremove(sockstates[i], node->startingSocks);
        } else if (0 == ret) {
            /* Got some data.  Try to get the first line. */
            char *line = getNextLine(sockstates[i]);

            if (NULL != line) {
                /* The first line should contain the fully qualified
                 * relation name. */
                List *names = NULL;
                char *ptr = line;
                RangeVar *rv = NULL;
                bool got_all_strings = false;;

                elog(LOG, "Read stream name '%s' on socket %d", 
                        line, sockstates[i]->fd);

                /* PostgreSQL uses lower case names internally. */
                for (ptr = line; *ptr != '\0'; ptr++) {
                    *ptr = tolower(*ptr);
                }

                /* Tokenize... */
                ptr = line; 
                while (!got_all_strings && *ptr != '\0') {
                    char *ptr2 = ptr;
                    Value *val = palloc(sizeof(*val));

                    while (*ptr2 != '.' && *ptr2 != '\0') ptr2++;
                    
                    if (*ptr2 == '\0') {
                        got_all_strings = true;
                    } else {
                        *ptr2 = '\0';
                    }

                    strVal(val) = ptr;
                    val->type = T_String;

                    names = lappend(names, val);

                    ptr = ptr2 + 1;
                }

                /* ...and convert into an OID */
                rv = makeRangeVarFromNameList(names);
                sockstates[i]->reloid = RangeVarGetRelid(rv, true);

                if (InvalidOid == sockstates[i]->reloid) {
                    elog(LOG, "Invalid stream name '%s' from data source.",
                            NameListToString(names));
                    close(sockstates[i]->fd);
                    pfree(sockstates[i]);
                } else {
                    char buf[256];
                    int len;
                    char *name = NameListToString(names);

                    elog(LOG, "Stream name '%s' mapped to stream with OID %d",
                            name, sockstates[i]->reloid);

                    /* Send a reply to the sender */
                    len = snprintf(buf, sizeof(buf), 
                            "Receiving tuples for stream %s\n", name);
                    lossyWrite(sockstates[i], buf, len);
                    pfree(name);

                    /* Now that we have a stream associated with this socket,
                     * we can start receiving tuples from it. 
                     * Move it to the list of sockets without scan modules. */
                    node->unscannedSocks = 
                        lcons(sockstates[i], node->unscannedSocks);
                }

                /* Clean up... */
                node->startingSocks = 
                    lremove(sockstates[i], node->startingSocks);
                freeList(names);
            }
        }
    }
}


/* 
 * Attempt to attach recently-opened connections to open StreamScans
 */
void attachSocksToStrms(Eddy *node) 
{
    SockState *sockstates[256];
    int nfds = 0;
    int i;
    List *itr = node->unscannedSocks;

    for (i = 0; i < 256 && itr != NULL; i++) {
        sockstates[i] = (SockState *)lfirst(itr);
        nfds++;
        itr = lnext(itr);
    }

    /*dumpEddy(node);*/

    for (i = 0; i < nfds; i++) {
        Oid reloid = sockstates[i]->reloid;
        int srcid;

        /*
        elog(LOG, "Trying to find a stream for socket %d",
                sockstates[i]->fd);
                */

        /* Search for a source that
         * a) Is a StreamScan
         * b) Is for the proper relation.
         * c) Has no socket associated with it
         */
        for (srcid = 0; srcid < node->numSources; srcid++)
        {
            Plan	   *source = node->sources[srcid];
            ScanModule *sm;
            Plan	   *scan;

            if (source == NULL || sockstates[i] == NULL)
                continue;

            if ((unsigned int)source < 0x00000f00) {
                /* HACK ALERT: Archived streams seem to lead to very small
                 * source "pointers" */
                elog(NOTICE, "Source pointer %d is very small (%p)",
                        srcid, source);

                continue;
            }

            sm = (ScanModule *) source;
            scan = sm->scan;

            if (IsA(scan, StrmScan)) {
                StrmScan *ss = (StrmScan*)(scan);

                /*
                elog(LOG, "Source %d is a StrmScan, with OID %d.", srcid,
                        GetRelOidStrmScan(ss));
                        */

                if (GetRelOidStrmScan(ss) == reloid
                    && !IsHeapAccessedStream(ss->streamtype)
                    && NULL == ScanSockGetSocket(&(ss->scansock)))
                {
                    elog(LOG, "Attaching socket %d to stream source %d",
                            sockstates[i]->fd, srcid);
                    ScanSockSetSocket(&(ss->scansock), sockstates[i], node);
                    node->unscannedSocks =
                        lremove(sockstates[i], node->unscannedSocks);
                    sockstates[i] = NULL;
                }
            } else {
                elog(LOG, "Source %d is NOT a StrmScan", srcid);
            }
        }
    }
}

/*
 * Discard input data from sockets that aren't associated with scans. 
 */
void suckUpUnusedTuples(Eddy *node) 
{
    List *itr = NULL;

    foreach (itr, node->unscannedSocks) {
        SockState *ss = (SockState *)lfirst(itr);  

        while (NULL != getNextLine(ss)) {
            /* We don't need to pfree() the ignored line, since it is a pointer
             * into an internal buffer! */
        }
    }
}


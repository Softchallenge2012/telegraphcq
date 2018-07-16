/*-------------------------------------------------------------------------
 *
 * nodeScanModule.c
 *	  An adaptor to turn a postgres scan operator into an eddy module.
 *
 * This operator uses the new Fjord "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeScanModule.c,v 1.37 2005/06/13 02:59:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"
#include "miscadmin.h"

#include "executor/execdebug.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeFjord.h"
#include "executor/nodeEddy.h"
#include "executor/nodeScanModule.h"
#include "executor/nodeSeqscan.h"
#include "executor/nodeStrmScan.h"
#include "optimizer/eddy.h"
#include "optimizer/bindings.h"
#include "parser/parsetree.h"
#include "storage/lmgr.h"
#include "telegraphcq/dyncatalog.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "rqueue/controlq.h"
#include "parser/parsetree.h"
#include "utils/lsyscache.h"

#define DEBUG_SCANM 1 

/* 
 * Prototype of dequeue_elt() internal function from resultq.c
 *
 * TODO: Either use a different function in this file or make dequeue_elt()
 * public.
 */
bool		dequeue_elt(Queue *, char **data, struct queue_serializer * qs);


/*
 *	Prototypes of static functions - local to this compilation unit
 */
static void ReleaseScanLock(Plan *p);
static void resetIdleCounts(Eddy * e, List * used_by);

/* ----------------------------------------------------------------------------
 *	Dynamically-dispatched functions
 * ----------------------------------------------------------------------------
 */

bool
ProcessInitScanModule(ScanModule * node, EState *estate, Plan *parent)
{

	bool				 ret  	  = false;
	struct queue_lock 	*ql   	  = NULL;
	TupleDesc   		 td;
	SeqScan             *seqscan;
	Eddy 				*eddy 	  = (Eddy *) parent;
	int 				 sourceID; 

	FjordInitialize(&node->fjord, estate);

	node->isDone = false;
	node->tuplesScanned = 0;
	ret = ExecInitNode((Plan *)node->scan,node->fjord.plan.state,(Plan *) node);
    /* needed for error handling code in ExecStrmScan */
	node->fjord.plan.parent = parent;  
	node->scan->parent = (Plan *) node;

	if (IsA(node->scan,StrmScan)) 
	{
		seqscan = ((StrmScan *) node->scan)->seqscan;
	}
	else 
	{
		Assert(IsA(node->scan,SeqScan));
		seqscan = (SeqScan *)node->scan;
	}
	td = CreateTupleDescCopyConstr(seqscan->scanstate->
								   css_ScanTupleSlot->ttc_tupleDescriptor);
	sourceID = sourceGetVarNo(node->scan) - 1;
	eddy->base_tuple_descs[sourceID] = td;

	if (ret && IsA(node->scan, StrmScan))
	{
		StrmScan   *strmscan = (StrmScan *) node->scan;
		NotifyWCH(getSourceOid(node->scan, eddy), STREAM_STARTUP);
		node->needsrescan = false;
		node->sourcehasvarbindings = false;
		node->ispullsource=false;

		
		if (strmscan->bindings != NULL)
		{
		  node->ispullsource=TRUE;
			if (!strmscan_has_var_bindings(strmscan))
				node->needsrescan = true;
			else
				node->sourcehasvarbindings = true;
		}

		/* Initialize result queue and result table slot for 
		 * recursive queries
 		 */
		
		make_lock_nolock(&ql);
#ifdef DEBUG_SCANM
		elog(DEBUG1, "making a recursive result q of size %d",
			 tcqrecursiveresultqsize);
#endif
		make_queue(&(node->recurseResult), ql, tcqrecursiveresultqsize);
		strmscan->recurseOutputSlot = MakeTupleTableSlot();
		strmscan->recurseOutputSlot->ttc_tupleDescriptor = td;
	}
	
	node->numpendingprobes = 0;

	/* when pending probes == 0, the scan ends */
	if (IsA(node->scan, SeqScan) ||
		(IsA(node->scan, StrmScan) &&
		 ((StrmScan *) node->scan)->bindings == NULL))
		node->numpendingprobes = 1;

	node->itersIdle=0;
	
	node->haveNewTuple = FALSE; /* - TCQ SR */
	node->currentTupleTimestamp = 0; /* - TCQ SR */
	((Fjord *)node)->usesTempContext = TRUE; /* - TCQ SR */
	node->isRecursivelyFed = FALSE; /* - TCQ SJ */

	return ret;

}

void
ScanModuleDeliverProbeTuple(ScanModule * node,
							IntermediateHeapTuple iht)
{

	if (node->fjord.scanstate->cstate.cs_ExprContext->ecxt_outertuple == NULL)
		node->fjord.scanstate->cstate.cs_ExprContext->ecxt_outertuple = MakeTupleTableSlot();

	node->fjord.scanstate->cstate.cs_ExprContext->ecxt_outertuple->val = (HeapTuple) iht;
	node->fjord.scanstate->cstate.cs_ExprContext->ecxt_outertuple->ttc_isIntermediateTuple = true;

	/*
	elog(NOTICE, "ScanModuleDeliverProbe: stream %s received tuple",
	     get_rel_name(getSourceOid(node->scan, (Eddy*)node->fjord.plan.parent)));
	*/
	/*	debugitup_lev(iht, NOTICE); */

	ExecReScan(node->scan,
			   node->fjord.scanstate->cstate.cs_ExprContext,
			   (Plan *) node);
}

bool
updatePendingProbes(Plan *p, HeapTuple tuple, bool doneTuple)
{
	/*
	 * return is: doneTuple == true: is this an end of probe tuple
	 * doneTuple == false: should this probe be processed
	 */
	bool		ret = false;

	/* tbd track pening probes */
	switch (nodeTag(p))
	{
		case T_ScanModule:
			{
				ScanModule *m = (ScanModule *) p;

				if (doneTuple)
				{
					if (doneWithProbe(m->scan, tuple))
					{

						m->numpendingprobes--;
						ret = true;
					}
					else
						ret = false;

				}
				else
				{
					ret = beginProbe(m->scan, tuple);
					if (ret)
					{
						m->numpendingprobes++;	/* keep a count */
						m->isDone = false;		/* scan is now ready */

					}

				}

			}
			break;
		default:
			elog(ERROR, "unknown node in updatePendingProbes");

			break;

	}

	return ret;
}


/* ----------------------------------------------------------------
 *		ProcessNextScanModule
 * ----------------------------------------------------------------
 */
void
ProcessNextScanModule(ScanModule * node, Plan *parent)
{

	int32		sourceID = -1;
	bool		enqsucc;

	FJORD_AUTOVAR_DECLS;
	FJORD_AUTOVAR_DEFNS;

	sourceID = sourceGetVarNo(node->scan) - 1;				  /* Set sourceID */

	
	/*
	 *  This is only needed for sources, as unfortunately, sources need to have
	 *  a separate "id" namespace compared to modules (think RangeTable)
	 */ 
	oiht = &(eddy->eddystate->SourceIHT_Table[sourceID]);

	if (IsScanModuleDone(node,false)) /* Early out */
	  return;


	if (node->needsrescan)
		/* set for scans with all const bindings to trigger rescan once */
	{
		ExecReScan(node->scan,
				   node->fjord.scanstate->cstate.cs_ExprContext,
				   (Plan *) node);
		node->needsrescan = false;

	}

	if(node->haveNewTuple == FALSE)
	  getNextTupleIntoSlot(node); /* - TCQ SR */
	  //lookAheadNextTupleTimestamp(node);

	output = scanstate->cstate.cs_ResultTupleSlot; /* The ResultTupleSlot has been fed into by the above function call */


	/*
	 * The output slot here *owns* the tuple being scanned.  this means it
	 * is not safe for two tuples from the same source to be in-flight at
	 * the same time without modifiying the ownership strategy as the next
	 * call to ExecProcNode will clear this slot.
	 *
	 */
	node->itersIdle++;
	if (output)
	  {
	    bool		wasend = false;
	    /*
	     *   TBD TBD TBD .... repetitive code - PERF PERF PERF
	     */ 
	    
	    if (TupIsNull(output))
	      {
		node->itersIdle++;
		return;
	      }
	    else
	      {
		
		node->itersIdle=0;
		if(node->numpendingprobes > 0)
		  {
		    wasend = updatePendingProbes((Plan *) node, output->val, true);
		    resetIdleCounts(eddy, node->fjord.usedByQueries);
		  }
		else
		  wasend=false;
	      }
	    if (node->numpendingprobes == 0)		/* scan is now finished.  */
	      {
		if(node->numpendingprobes == 0) node->isDone = true;
		return;
	      }
	    
	    if(wasend)
	      {
		return;
	      }
	    
	    
	    output->ttc_isIntermediateTuple = false;		/* Denote base */
	    
	    econtext->ecxt_scantuple = output;		/* Init expr context */
	    
	    
	    initFromBaseTuple(oiht, output->val,		/* Create OIHT */
			      output->ttc_tupleDescriptor, sourceID);
	    
	    Assert(IsA(parent, Eddy));
	    if ((! UseStaticPlan) && (! UseOperatorPrecedence) && UseTupleLineage)
	      //if ((! UseOperatorPrecedence)  && UseTupleLineage)
	      {
		Assert(IsA(parent, Eddy));
		/*		  debugitup_lev(oiht, NOTICE); */
		if(buildStemsNoDup((Eddy *) parent, oiht, sourceID, node->nodup) == false)
		  {
		    /*		      elog(NOTICE, " tuple is a duplicate ");*/
		    
		    return;
		  }
	      }
	    if (UseTupleLineage) 
	      {
		clearCompletionBits(oiht, fjord->usedByQueries);	 /* Init OIHT */
	      }
	    FJORD_ENQ_OUT_PTR(enqsucc, node, &oiht);

	    node->haveNewTuple = FALSE; /* - TCQ SR */
	    scanstate->cstate.cs_ResultTupleSlot = NULL; /* - TCQ SR */

	    /*		ENQUEUE_IHT(fjord->output, iht);	 */	/* Disable dyncat */
	    fjord->doneInput = true;
	    node->tuplesScanned++;
	    eddy->eddystate->total_base_tuples_remaining--;
	  }
	
	return;
}


/* 
 * getNextTupleIntoSlot()
 *
 * This function takes away some functionality from the old version of ProcessNextScanModule()
 * It does the job of fetching the next base tuple into the given slot. 
 *
 * The reason for encapsulating this functionality into a separate function is because we want to 
 * now support timestamp-ordered source selection in TCQ (during SIGMOD 2005 demo work) 
 *
 *   - TCQ SR
 *
 *
 */

void
getNextTupleIntoSlot(ScanModule * node)
{

  char *recTuple = NULL;
  TupleTableSlot **poutput = &(node->fjord.scanstate->cstate.cs_ResultTupleSlot);
 

  /*  Fetch the tuple.  
   *  Dequeue from recursive input queue, if the node is not recursive, or 
   *  the recurseResult is empty, the dequeue returns false and we proceed as 
   *  normal.  If the node is recursive and has a result, then we continue with
   *  that result.
   */
  

  /*
   * SJ, 6/5/05: SIGMOD Hack.  Only if its not a recursive scan module
   * do we ExecProcNode.  Thus, we no longer support true recursive
   * queries.  This fix, however, is needed to support efficient
   * subqueries.  In the future, we should support both subqueries and
   * recursive queries.  TODO!
   */
  if(node->isRecursivelyFed == FALSE) 
    {  
      
      *poutput = ExecProcNode((Plan *) node->scan, (Plan *) node);	/* Fetch tuple */
      if(*poutput && (*poutput)->val)
	{
	  /*
	    elog(DEBUG1, " scan module %s got wch tuple",
	    get_rel_name(getSourceOid(node->scan, (Eddy*)parent)));
	  */
	}
      else
	{
	  if (IsA(node->scan, SeqScan)) 
	    {
	      elog(LOG, "Stopping sequential scan on a table.");
	      node->isDone = true;
	    }   
	}
    } 
  else 
    {
      
      if(node->recurseResult != NULL && dequeue_elt(node->recurseResult, &recTuple, &result_serializer)) 
	{  

	    
	  if (((StrmScan *)node->scan)->recurseOutputSlot == NULL)
	    {
	      elog(FATAL,"This should never happen. recurseOutputSlot not initialized.");
	    }
	  *poutput = ((StrmScan *)node->scan)->recurseOutputSlot;
	  (void) ExecStoreTuple((HeapTuple) ((result *)recTuple)->data.tuple,
				*poutput, InvalidBuffer, true);
	  
	  if(*poutput &&
	     (*poutput)->val)
	    {
	      /*
		elog(NOTICE, " scan module %s got recursive tuple",
		get_rel_name(getSourceOid(node->scan, (Eddy*)parent)));
	      */
	    }
	}
    }
     
  if(*poutput && (*poutput)->val)
    node->haveNewTuple = TRUE;
      
  
}

/*
 * lookAheadNextTupleTimestamp()
 *
 * To support timestamp-ordered source selection, we need to "look ahead"
 * what timestamp we will get if we fetch from a particular source.
 * This is the purpose of this function. In the process, it may "prefetch" the 
 * next tuple on this source. This is fine, and will result in no work when the
 * next call to ProcessNextScanModule() is made
 * Done during our run for SIGMOD 2005 demo
 * 
 *  - TCQ SR
 *
 */

Timestamp
lookAheadNextTupleTimestamp(ScanModule *node)
{
  Timestamp result;
  MemoryContext tempContext, octx;
  HeapTuple ht;
  TupleDesc td;
  TupleTableSlot *output;

  tempContext = ((Fjord *)node)->tempContext;


  if(node->haveNewTuple) /* We already have a new tuple sitting in */
    {
      result = node->currentTupleTimestamp;
      
    }
  else /* We need to fetch a new tuple */ 
    {

      getNextTupleIntoSlot(node);
      if(node->haveNewTuple) /* We tried, We succeeded */
	{
	  output = node->fjord.scanstate->cstate.cs_ResultTupleSlot;
	  ht = output->val;
	  td = output->ttc_tupleDescriptor;
	  
	  
	  octx = MemoryContextSwitchTo(tempContext); /* Because timestamp functions will leak like hell */

	  if(IsA(node->scan, StrmScan))
	    {
	      result = get_timestamp_val(td, ht); /* Currently in triageq.c */
	    }
	  else /* We need to tell whoever is "looking ahead" that we are not a stream */ 
	    {
	      result = 0;

	    }

	  MemoryContextSwitchTo(octx);


	  node->currentTupleTimestamp = result;
	}
      else /* We tried and failed */
	{
	  result = DBL_MAX; /* WARNING: I am using DBL_MAX as the indicator of "No Tuple Available" */
	}
    }

  
  return result;

}

bool
ProcessEndScanModule(ScanModule * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	ReleaseScanLock(node->scan);
	if (IsA(node->scan, StrmScan))
	  {

		NotifyWCH(getSourceOid(node->scan, (Eddy *) parent),
			  STREAM_SHUTDOWN);
	  }
	if (IsA(node->scan, StrmScan)) 
	{
		free_queue(&(node->recurseResult));
	}
	
	ExecEndNode(node->scan, (Plan *) node);
	FjordEnd(&node->fjord, eddy);

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(ScanModule));

	return TRUE;
}

bool
ProcessRemoveQueryScanModule(ScanModule * node, int32 queryid, Plan *parent)
{
	return FjordRemoveQuery((Fjord *) node, queryid);
}

static void
ProcessPrintStatsScanModule_with_level(ScanModule * node, Plan *parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	elog(level, "ScanModule: [Tuples scanned: %d]",node->tuplesScanned);
}

void
ProcessPrintStatsScanModule(ScanModule *node, Plan *parent)
{
	ProcessPrintStatsScanModule_with_level(node,parent,INFO);
}


/* ----------------------------------------------------------------------------
 *	Static functions local to this compilation unit
 * ----------------------------------------------------------------------------
 */

/*
 *	ReleaseScanLock
 *
 *	This is a convenience routine for ExecEndScanModule and should only be
 *	called from there.	SeqScan grabs an AccessSharedLock on the relation when
 *	it is opened via ExecInitSeqScan.  This lock is not released until the end
 *	of the current xact, which never occurs in the tcqbackend.	Instead we'll
 *	release the lock when the object is removed from the eddy.
 */
static void
ReleaseScanLock(Plan *p)
{
	Relation	rel = NULL;
	LOCKMODE	locktype = NoLock;

	switch (nodeTag(p))
	{
		case T_SeqScan:
			rel = ((SeqScan *) p)->scanstate->css_currentRelation;
			locktype = AccessShareLock;		
			break;
		case T_StrmScan:

		    rel = (GetScanStateStrmScan((StrmScan *) p))->
		      css_currentRelation;
		    locktype = AccessShareLock;
		  

		  break;
		default:
			break;

	}
	if (rel != NULL)
		UnlockRelation(rel, locktype);
}

/*
 *	This function returns the real Oid of the source _without_ using the Eddy's
 *	rangetable data structure.
 */
Oid
getRealSourceOid(Plan *node)
{
	Plan	   *scan;
	Oid			oid = InvalidOid;

	if (node == NULL)
	{
		return oid;;
	}

	Assert(IsA(node, ScanModule));

	scan = ((ScanModule *) node)->scan;

	if (IsA(scan, SeqScan))
	{
		SeqScan    *ss = (SeqScan *) scan;

		oid = ss->scanstate->css_currentRelation->rd_id;
	}
	else if (IsA(scan, StrmScan))
	{
		StrmScan   *ss = (StrmScan *) scan;

		oid = ss->streamoid;
	}
	return oid;
}

Oid
getSourceOid(Plan *scan, Eddy * parent)
{
	int			sourceid = sourceGetVarNo(scan) - 1;
	Oid			ret = rt_fetch(sourceid + 1, parent->rangetable)->relid;

	return ret;
}

void
NotifyWCH(Oid streamoid, ControlCommand cmd)
{
	Controlqentry cqe;

	cqe.command = cmd;
	cqe.data.w.streamoid = streamoid;

	enqueue(*p_wrap_control_queue, (char *) &cqe, false, &controlqentry_serializer);
}

/* 		if (UseStaticPlan)						 */
/* 		{										 */
/* 			int driver = ((Fjord *)node)->driver;	 */
/* 		    Fjord *out; 						 */
												
/* 		    if (driver == - 1)					 */
/* 			{									 */
/* 				driver = StaticDriver;			 */
/* 			}									 */
												
/* 			out = ((Fjord *)node)->outputs[driver]; */
												
/* 			ENQUEUE_IHT(out->input,iht);			 */
/* 	    }										 */
/* 		else									 */
/* 		{										 */
/* 			FJORD_ENQ_NS_OUT(enqsucc,node,iht); */
/* 		}										 */


void
InsertRecursiveResults(ScanModule * node, HeapTuple inputTuple){
  result r;
  r.type = RESULT_HEAPTUPLE;
  r.data.tuple = inputTuple;
  if(enqueue(node->recurseResult, (char *) &r, false, &result_serializer) == false)
	  {
		  elog(NOTICE, "recursive result tuple discarded because enqueue failed. bytesfree=%d", node->recurseResult->bytesfree);
	  }
  /*  free tuple here.  add comment explaining why it is freed here and 
   *  not by the program calling this function
   */

  //heap_freetuple(inputTuple);
} 

bool
IsScanModuleDone(ScanModule * node, bool isCancel)
{
	bool recurseDone = true;
	bool scanDone=node->isDone;
	if(IsA(node->scan, StrmScan && isCancel))
	  {
	    scanDone = scanDone || (((StrmScan*)node->scan)->dependant_stream && (node->itersIdle > 500));
	  }

	if(node->recurseResult &&
	   node->recurseResult->numelts > 0)
		recurseDone=false;
	
	return (recurseDone && scanDone);
}

void resetIdleCounts(Eddy * e, List * used_by)
{
  int i=0;
  ScanModule *m=NULL;
  for(i=0; i <e->numSources; i++)
    {
      if(e->sources[i] == NULL ||
	 !IsA(e->sources[i], ScanModule))
	continue;
      else
	{
	  m = (ScanModule*)e->sources[i];
	  if(!nonoverlap_setsi(used_by,m->fjord.usedByQueries))
	    {
	      m->itersIdle=0;
	    }
	}
    }
}

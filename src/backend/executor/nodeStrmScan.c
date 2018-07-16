/*-------------------------------------------------------------------------
 *
 * nodeStrmScan.c
 *
 *	  Support routines for the stream scanner.
 *
 *	  Combines the functionality of the scanner that reads from a
 *	  queue and one that reads from on-disk relations. It is used to
 *	  handle incoming network based sources.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeStrmScan.c,v 1.30 2005/12/14 20:49:23 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "access/xact.h"
#include "access/tupdesc.h"
#include "executor/execdebug.h"
#include "executor/nodeSeqscan.h"
#include "executor/nodeScanSock.h"
#include "executor/nodeStrmScan.h"
#include "executor/nodeScanModule.h"
#include "executor/nodeEddy.h"
#include "parser/parsetree.h"
#include "parser/parse_wrapper.h"
#include "utils/lsyscache.h"
#include "catalog/namespace.h"
#include "rqueue/rqdest.h"
#include "rqueue/ingresq.h"
#include "rqueue/controlq.h"
#include "telegraphcq/wrapch.h"
#include "optimizer/bindings.h"

#include "telegraphcq/tcq_usleep.h"


/* Module-global variable that holds the number of stream scans currently
 * dumping data in the current process.  Every time this increases from zero to
 * 1, we start a transaction; every time it falls back to zero, we commit.
 *
 * NOTE: This mechanism only works if no one else is running update
 * transactions from this process!!!
 */
int g_num_streams_being_dumped = 0;


/* External function to fetch the OID of the scan's relation. */
Oid GetRelOidStrmScan(StrmScan *node) 
{
    return node->streamoid;
}

/* External function to return the index into the Eddy range table of the
 * underlying relation. */
Index GetRTIndexStrmScan(StrmScan *node) 
{
    return node->rtindex;
}

CommonScanState *GetScanStateStrmScan(StrmScan *node) 
{
    if (node->seqscan) {
        return node->seqscan->scanstate;
    } else {
        return node->scanstate; 
    }
}

/* ----------------------------------------------------------------
 *						Stream Scanner
 * ----------------------------------------------------------------
 */

#define SPIN_THR	  100
#define SPIN_DELAY	  2000

TupleTableSlot *
ExecStrmScan(StrmScan * node)
{
    TupleTableSlot *ret = NULL;

        if (IsHeapAccessedStream(node->streamtype)) {
		ret = ExecSeqScan(node->seqscan);
	}	
	else
	{
	  if (node->blocking) {
          elog(ERROR, "Blocking ScanSock not implemented.");
	    /*ret = ExecStrmScanBlocking(node);*/
	  } else {
            /* Otherwise read from socket. */
            ret = ExecScanSock((ScanSock *) node);
	  }
	}

    return ret;
}

#define VAL_SIZE 3

/* static HeapTuple a = NULL; */
/* static HeapTuple b = NULL; */

/* Gets the next tuple */
#if 0
TupleTableSlot *
ExecStrmScanBlocking(StrmScan * node)
{
	TupleTableSlot *result = NULL;
    ScanQueue * node_as_scan_q = (ScanQueue*)node;

	/* Just read from the ingres queue for now. */
	int			loop = 0;

	while (TupIsNull(result) && !(node_as_scan_q->scanqstate->end_of_stream))
	{

		if (loop == SPIN_THR)
		{
			/* elog(DEBUG, "StrmScan sleeping"); */
			tcq_usleep(SPIN_DELAY);
			loop = 0;
		}

		result = ExecScanQueue(node_as_scan_q);
		loop++;
	}

	/* elog(DEBUG, "Dequeued a tuple"); */
	/* print_slot(result); */

	return result;
}
#endif /* 0 */

/* Need slots for both the sequential scan and the queue scanner */

bool
ExecInitStrmScan(StrmScan * node, EState *estate, Plan *parent)
{
	CommonScanState *css;
	TupleDesc	scan_tup_desc;
	IngresQIdx	idx;
	List	   *sname = NIL;	/* Name of the stream */
	bool		res = true;

	MemoryContext oldcontext;

#ifdef NOT_USED
	/* XXX For generating values during testing */
	node->values = (Datum *) palloc(sizeof(Datum) * VAL_SIZE);
	node->seqnum = 0;
	node->round = 0;
	node->prng.left = -1;
    node->tuples_to_dump = NULL;

	/* Seed it based on the partition id */
	if (node->part_id)
		seedMT(&(node->prng), 4375U);
	else
		seedMT(&(node->prng), 9875U);
#endif

	/*
	 * First initialize the sequential scan, and get the tuple descriptor
	 * for the input tuples from there.
	 */

	ExecInitSeqScan(node->seqscan, estate, parent);

	if ((node->streamtype == RELKIND_UNARCHIVEDSTREAM_DROP) ||
		(node->streamtype == RELKIND_UNARCHIVEDSTREAM))
	{

		/* Get the target list and other info from the sequential scan */
		(node->scansock).plan = node->seqscan->plan;
		(node->scansock).plan.type = T_StrmScan;	/* Make sure its there */

		css = node->seqscan->scanstate;

		/*
		 * Change the context to shared memory, so the initialization
		 * state, if needed, can be shared with the WRAPCH
		 */

		oldcontext = MemoryContextSwitchTo(TopSharedMemoryContext);

		/* Set the tuple descriptor so the initscansock can use it */

		/*
		 *scan_tup_desc =
		 * CreateTupleDescCopy(css->css_ScanTupleSlot->ttc_tupleDescriptor);
		 */
		
		/* Copying constraints too... like TIMESTAMPCOLUMN !! - TCQ SR */

		scan_tup_desc =
			CreateTupleDescCopyConstr(css->css_ScanTupleSlot->ttc_tupleDescriptor);



		MemoryContextSwitchTo(oldcontext);

		(node->scansock).tupDesc = scan_tup_desc;
		/*(node->scansock).serializer = &result_serializer;*/
		(node->scansock).dontProject = true;

		/* Allocate an ingresq for the scan based on the relation name */
		sname = makedupNameList2(
								 get_namespace_name(css->css_currentRelation->rd_rel->relnamespace),
						 css->css_currentRelation->rd_rel->relname.data);

		idx = allocIngresQ(DEF_INGRES_QSIZE,
						   NameListToString(sname));

		freeNameList(sname);
		sname = NIL;
		/*(node->scansock).queue = getIngresQ(idx);*/
		/*(node->scansock).qno = idx;*/


		/*
		 * Remember to set the tuple descriptor, otherwise, we cannot
		 * create tuples!
		 */
		setTupleDesc(idx, scan_tup_desc);

		/*
		 * Note, the state and the targetlist have been already
		 * initialized from the sequential scan, when the streamer was
		 * setup
		 */

		/*
		 * Need to install some serialize and deserialize methods into the
		 * queues
		 */

		res = ExecInitScanSock((ScanSock *) node, estate, parent);
		(node->scansock).plan.type = T_StrmScan;
		/*
		 * 
		 * OC: the relation associated with the stream should be locked 
		 *  for the diration of its use so that DDL is blocked.  
		 * I.e. don't allow users to drop streams while in use.
		 * also, this is *not* the last use of the seqscan as
		 * ExecReScanStrmScan uses it for pull-wrapper related operations.
		 * 
		 *
		 *
		 * UnlockRelation(node->seqscan->scanstate->css_currentRelation,
		 *       AccessShareLock);
		 *  ExecEndSeqScan(node->seqscan);
		 *   node->seqscan = NULL;
		 */



	}
	else
	{
		AssertMacro(ScanDirectionIsForward(node->seqscan->plan.state->es_direction));

		
#ifdef MASHAH_BEHAVIOR
		node->seqscan->plan.state->es_direction = ForwardScanDirection;
#else

		/*
		 * Set the scan to be backwards, get the last tuple in the stream,
		 * and restore the direction to be forwards.
		 */
		node->seqscan->plan.state->es_direction = BackwardScanDirection;
		ExecSeqScan(node->seqscan);
		node->seqscan->plan.state->es_direction = ForwardScanDirection;
#endif
	}


	StrmScanInitBindingInfo(node);
	node->useindex = false;
	return res;

}


void StrmScanInitBindingInfo(StrmScan * node)
{


	if (node->bindings)
	{
		int			i;

		for (i = 0; i < node->numwrappers; i++)
		{
			if (!node->bindings[i])
				continue;
			node->bindings[i]->probecontext = TopSharedMemoryContext;
			node->bindings[i]->probequeue = *p_wrap_control_queue;
		}
	}
}
int
ExecCountSlotsStrmScan(StrmScan * node)
{
    if (node->seqscan) {
        return ExecCountSlotsNode(outerPlan(node)) +
            ExecCountSlotsNode(innerPlan(node)) +
            ExecCountSlotsSeqScan(node->seqscan) +
            ExecCountSlotsScanSock((ScanSock *) node);
    } else {
        return ExecCountSlotsNode(outerPlan(node)) +
            ExecCountSlotsNode(innerPlan(node)) +
            ExecCountSlotsScanSock((ScanSock *) node);
    }
}

void
ExecEndStrmScan(StrmScan * node)
{
	if (node->streamtype == RELKIND_UNARCHIVEDSTREAM_DROP)
	{
		ExecEndScanSock((ScanSock *) node);
	}

    if (node->seqscan) {
      /* OSC : pull wrappers need this
        ExecEndSeqScan(node->seqscan);
	     node->seqscan = NULL;
      */
    }

	if (node->bindings)
	{
		int			i;

		for (i = 0; i < node->numwrappers; i++)
		{
			if (node->bindings[i])
			{
				freeBindinginfo(node->bindings[i]);
				node->bindings[i] = NULL;

			}

		}
	}


}


void
ExecReScanStrmScan(StrmScan * node, ExprContext *exprCtxt, Plan *parent)
{

	int			i;
	HeapTuple	tuple;
	TupleDesc	tdesc;
	int		   *mapping = NULL;
	Fjord	   *f = (Fjord *) parent;
	HeapTuple	inputTuple;
	bool		isIntermediate;

    Assert(node->seqscan != NULL);

	/*
	 * 1. grab binding tlist for the tuple 2. set up some projection
	 * information, and project a probe tuple based on the input iht 3.
	 * send a probe tuple message to the wretch
	 *
	 */

	for (i = 0; i < node->numwrappers; i++)
	{


		MemoryContext octx = NULL;
		Controlqentry cqe;

		if (!node->bindings)
			continue;

		if (!node->bindings[i]->projinfo)
		{
			node->bindings[i]->projinfo =
				createProbeProjectionInfo(node->bindings[i]->targetlist);
		}

		if (!exprCtxt)

		{
			TCQHandleErrorMessageForQueries(f->usedByQueries,
					   "ExecReScanStrmScan got an invalid rescan tuple");
			return;
		}


		octx = MemoryContextSwitchTo(node->bindings[i]->probecontext);
		tdesc = node->scansock.tupDesc;
		mapping = palloc(sizeof(int) * tdesc->natts);
		memcpy(mapping, node->bindings[i]->mapping, sizeof(int) * tdesc->natts);
		if (exprCtxt->ecxt_outertuple)
		{
			isIntermediate = exprCtxt->ecxt_outertuple->ttc_isIntermediateTuple;
			inputTuple = (HeapTuple) exprCtxt->ecxt_outertuple->val;

		}
		else
		{
			isIntermediate = false;
			inputTuple = NULL;
		}

		tuple = ProjectTuple(node->bindings[i]->projinfo,
							 tdesc,
							 isIntermediate,
							 inputTuple);

		MemoryContextSwitchTo(octx);
		if (updatePendingProbes(parent, tuple, false))
		{

			cqe.command = STREAM_PROBE;
			cqe.probeinfo.wrapperoid = node->bindings[i]->wrapperoid;
			cqe.data.w.streamoid = node->seqscan->scanstate->css_currentRelation->rd_id;
			cqe.probeinfo.mapping = mapping;
			cqe.probeinfo.probetuple = tuple;
			if(enqueue(node->bindings[i]->probequeue, (char *) &cqe, false,
					&controlqentry_serializer) == false)
			  {
			
			    elog(NOTICE, "enqueue of probe tuple into wrap_control_queue failed");
			  }

		}
		else
			heap_freetuple(tuple);

	}



}

ProjectionInfo *
createProbeProjectionInfo(List *targetList)
{
	ProjectionInfo *projInfo = makeNode(ProjectionInfo);

	projInfo->pi_targetlist = targetList;
	projInfo->pi_len = ExecTargetListLength(targetList);
	projInfo->pi_tupValue = (projInfo->pi_len <= 0) ? NULL : (Datum *) palloc(sizeof(Datum) * projInfo->pi_len);
	memset(projInfo->pi_tupValue, sizeof(Datum)*projInfo->pi_len, 0);
	projInfo->pi_exprContext = MakeExprContext(MakeTupleTableSlot(),
											   CurrentMemoryContext);
	projInfo->pi_slot = MakeTupleTableSlot();
	return projInfo;
}


bool
strmscan_has_var_bindings(StrmScan * scan)
{
	List	   *iter = NULL;
	TargetEntry *tle = NULL;
	bool		allconst = true;


	int			i = 0;

	for (i = 0; i < scan->numwrappers; i++)
	{
		if (scan->bindings[i])
		{
			foreach(iter, scan->bindings[i]->targetlist)
			{
				tle = (TargetEntry *) lfirst(iter);
				if (!IsA(tle->expr, Const))
				{
					allconst = false;
					break;
				}

			}


		}
		if (allconst == false)
			break;
	}
	return (allconst == false);
}

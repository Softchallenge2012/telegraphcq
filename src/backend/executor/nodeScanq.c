/*-------------------------------------------------------------------------
 *
 * nodeScanq.c
 *	  Support routines for scanning from a queue of tuples
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeScanq.c,v 1.31 2006/02/02 01:46:40 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *		ExecScanQueue			scans from a queue.
 *		ExecInitScanQeueue		creates and initializes a ScanQeueue node.
 *		ExecEndScanQueue		shuts down a ScanQueue
 *
 */
#include "postgres.h"

#include "executor/executor.h"
#include "executor/nodeScanq.h"
#include "executor/nodeEddy.h"
#include "parser/parsetree.h"
#include "rqueue/rqdest.h"
#include "rqueue/ingresq.h"
#include "rqueue/controlq.h"
#include "miscadmin.h"
#include "utils/memutils.h"
#include "utils/builtins.h"
#include "access/printtup.h"
            /* For debugtup_lev() */

void		cancelQuery(int qno);

/* ----------------------------------------------------------------
 *		ExecScanQueue(node)
 *
 *		Scans the queue of tuples and returns the next qualifying
 *		tuple.
 *
 */

TupleTableSlot *
ExecScanQueue(ScanQueue * node)
{
	ScanQueueState *scanqstate;
	EState	   *estate;
	ExprContext *econtext;
	List	   *qual;
	TupleTableSlot *slot;
	bool		dontProject;
	Queue	   *q;
	q_serializer serializer = NULL;
	HeapTuple	t = NULL;
	result	   *res = NULL;
	text	   *error;
	char	   *msg;
	char	   *qelem = NULL;


	/*
	 * Fetch data from node
	 */
	estate = node->plan.state;
	scanqstate = node->scanqstate;
	econtext = scanqstate->cstate.cs_ExprContext;
	qual = node->plan.qual;
	dontProject = node->dontProject;
	q = node->queue;
	slot = node->scanqstate->sqs_ScanTupleSlot;
	serializer = node->serializer;

	/*
	 * Reset per-tuple memory context to free any expression evaluation
	 * storage allocated in the previous tuple cycle.  Note this can't
	 * happen until we're done projecting out tuples from a scan tuple.
	 */
	if (!dontProject)
		ResetExprContext(econtext);

	/*
	 * get a tuple from the access method loop until we obtain a tuple
	 * which passes the qualification.
	 */
	for (;;)
	{
		CHECK_FOR_INTERRUPTS();

		if (dequeue(q, &qelem, true, serializer))
/* 		if (dequeue(q, &qelem, false, serializer)) */
		{

			if (serializer == &result_serializer)
			{
				res = (result *) qelem;
				switch (res->type)
				{
					case RESULT_HEAPTUPLE:
                        /*elog(LOG, "ExecScanQueue: Got a tuple.");*/
						t = res->data.tuple;
						break;

					case RESULT_ERROR:
                        /*elog(LOG, "ExecScanQueue: Got an error.");*/
						error = res->data.error;
						msg = DatumGetCString(DirectFunctionCall1(textout,
												PointerGetDatum(error)));
						if (node->plan.parent &&
							IsA_Fjord(node->plan.parent))
						{
							Fjord	   *f = (Fjord *) node->plan.parent;
							Eddy * e = (Eddy *) node->plan.parent->parent;
					       List * qidlist = findOutputQueryList(e, f->usedByQueries);

							TCQHandleErrorMessageForQueries(qidlist,
															msg);
						}
						else
							TCQHandleErrorMessageForQuery(-1, msg);

						t = NULL;
						break;
					case RESULT_DONE:
                        elog(LOG, "ExecScanQueue: Got end-of-stream.");
			/* HACK OC: no end for now */
			/* scanqstate->end_of_stream = true; */
						t = NULL;
						break;
					default:
						elog(ERROR, "invalid result type");
				}
				pfree(res);
				res = NULL;
			}
			else
            {
				t = (HeapTuple) qelem;
            }

            if (t == NULL || t->t_len == 0) {
				(void) ExecClearTuple(slot);
            } else {
#if 0
                /* Punctuation ignorance code has been moved to ExecutePlan().
                 */
                 
                /* SPECIAL CASE: Got a punctuation tuple, and we're ignoring
                 * punctuation tuples. */
                if (node->ignorePunct && HeapTupleIsPunct(t)) {
                    
                    elog(DEBUG1, "Scanq %p ignoring a punctuation tuple.",
                            node);
                    (void) ExecClearTuple(slot);
                    return ExecScanQueue(node);
                        /* Return the next thing in the queue instead. */
                }
                /* END SPECIAL CASE */
                else {
                    elog(DEBUG1,"Scanq got a non-punct tuple ... "
						" or is not ignoring punctuation tuples ");
#endif /* 0 */
                    (void) ExecStoreTuple(t, slot, InvalidBuffer, true);
                /*}*/

               
                   
            }
		}
		else
		{
            /* Got no result when we dequeued.  This usually happens because
             * an eddy was polling a queue that happened to be empty at the
             * time. */
		  /*elog(LOG,"Dequeue on resultq got nothing.");*/
            /*tcq_usleep(100000);*/
			(void) ExecClearTuple(slot);
		}

		if (TupIsNull(slot))
		{
            /* If you get here, you've got a NULL tuple. */
            /*elog(LOG, "ExecScanQueue: NULL tuple.");*/

			if (dontProject)
				return NULL;
			return ExecStoreTuple(NULL,
								  scanqstate->cstate.cs_ProjInfo->pi_slot,
								  InvalidBuffer,
								  true);
		}
         

		/*
		 * place the current tuple into the expr context
		 */
		econtext->ecxt_scantuple = slot;

		/*
		 * check that the current tuple satisfies the qual-clause
		 *
		 * check for non-nil qual here to avoid a function call to ExecQual()
		 * when the qual is nil ... saves only a few cycles, but they add
		 * up ...
		 */
		if (!qual || ExecQual(qual, econtext, false))
		{
			TupleTableSlot *resultSlot;

			/*
			 * Found a satisfactory scan tuple.
			 *
			 * Form a projection tuple, store it in the result tuple slot and
			 * return it --- unless we find we can project no tuples from
			 * this scan tuple, in which case continue scan.
			 */
			if (!dontProject)
			{
				ExprDoneCond isDone;

				resultSlot = ExecProject(scanqstate->cstate.cs_ProjInfo,
										 &isDone);
				return resultSlot;
			}
			else
				resultSlot = slot;
			elog(DEBUG1,"Scanq: Fetched a tuple ... \n\n");
			debugtup_lev(resultSlot->val,resultSlot->ttc_tupleDescriptor,NULL,DEBUG1);
			return resultSlot;
		}

		/*
		 * Tuple fails qual, so free per-tuple memory and try again.
		 */
		ResetExprContext(econtext);
	}
	return NULL;
}

/* ----------------------------------------------------------------
 *		ExecInitScanQueue
 * ----------------------------------------------------------------
 */
bool
ExecInitScanQueue(ScanQueue * node, EState *estate, Plan *parent)
{


	ScanQueueState *scanqstate;
	bool		useNewSlots = ExecUseNewSlots((Plan *) node, parent);

	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);
	Assert(node->queue != NULL);

    elog(LOG, "ExecInitScanQueue");

	/*
	 * If we are running in backend don't ignore punctuation
	 */
    /*
	if (PROC_TCQ_BACKEND == MyProcessType) 
	{
		node->ignorePunct = false;
	}
    */
	/*
	 * assign the node's execution state
	 */
	node->plan.state = estate;

	/*
	 * create new ScanQueueState for node
	 */
	scanqstate = makeNode(ScanQueueState);
	node->scanqstate = scanqstate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &scanqstate->cstate);

#define SCANQ_NSLOTS 2

	/*
	 * tuple table initialization
	 */
	if (useNewSlots)
	{
		scanqstate->sqs_ScanTupleSlot = MakeTupleTableSlot();
		InitSlotForEddy(scanqstate->sqs_ScanTupleSlot);
	}
	else
	{
		scanqstate->sqs_ScanTupleSlot =
			ExecAllocTableSlot(estate->es_tupleTable);
	}


	ExecSetSlotDescriptor(scanqstate->sqs_ScanTupleSlot,
						  node->tupDesc, false);

	if (node->dontProject)
	{

		scanqstate->cstate.cs_ResultTupleSlot =
			scanqstate->sqs_ScanTupleSlot;

	}
	else
	{

		if (useNewSlots)
		{
			ExecInitNewResultTupleSlot(estate, &scanqstate->cstate);
			InitSlotForEddy(scanqstate->cstate.cs_ResultTupleSlot);
		}
		else
			ExecInitResultTupleSlot(estate, &scanqstate->cstate);

		/*
		 * initialize tuple type
		 */
		ExecAssignResultTypeFromTL((Plan *) node, &scanqstate->cstate);
		ExecAssignProjectionInfo((Plan *) node, &scanqstate->cstate);

	}

    scanqstate->end_of_stream = false;

	return TRUE;
}

int
ExecCountSlotsScanQueue(ScanQueue * node)
{
	return ExecCountSlotsNode(outerPlan(node)) +
		ExecCountSlotsNode(innerPlan(node)) +
		SCANQ_NSLOTS;
}

/* ----------------------------------------------------------------
 *		ExecEndScanQueue
 *
 *		frees any storage allocated through C routines.
 * ----------------------------------------------------------------
 */
void
ExecEndScanQueue(ScanQueue * node)
{
	ScanQueueState *scanqstate;


	/*
	 * get information from node
	 */
	scanqstate = node->scanqstate;

	/*
	 * Free the projection info and the scan attribute info
	 *
	 */
	ExecFreeProjectionInfo(&scanqstate->cstate);
	ExecFreeExprContext(&scanqstate->cstate);

	/*
	 * clean out the tuple table
	 */
	ExecClearTuple(scanqstate->cstate.cs_ResultTupleSlot);
	ExecClearTuple(scanqstate->sqs_ScanTupleSlot);
	ExecFreeSlotIfOwned(scanqstate->cstate.cs_ResultTupleSlot);
	if (scanqstate->cstate.cs_ResultTupleSlot !=
		scanqstate->sqs_ScanTupleSlot)
		ExecFreeSlotIfOwned(scanqstate->sqs_ScanTupleSlot);
	if (IsA(node, ScanQueue))
		cancelQuery(node->qno);
	node->queue = NULL;


}


void
cancelQuery(int qno)
{
	Controlqentry cqe;

	if (UseTelegraphCQ)
	{
		elog(DEBUG1, "scanq is sending a query removal"
			 " request for query id %d", qno);

		cqe.command = QUERY_REMOVE;
		cqe.data.queryid = qno;
		if (enqueue(*p_control_queue, (char *) &cqe, false, &controlqentry_serializer) == false) 
		{
			elog(DEBUG1, "control queue is full.... this shouldn't happen");
		}
		cqcancelid = -1;
	}
}

/*-------------------------------------------------------------------------
 *
 * nodeIndexSteM.c
 *	  routines to support index joins
 *
 *
 *-------------------------------------------------------------------------
 */

/*
 *	 INTERFACE ROUTINES
 *		ExecIndexSteM	  - process a nestloop join of two plans
 *		ExecInitIndexSteM - initialize the join
 *		ExecEndIndexSteM   - shut down the join
 */

#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/nodeIndexSteM.h"
#include "utils/memutils.h"
#include <assert.h>

/* ----------------------------------------------------------------
 *		ExecIndexSteM(node)
 *
 * old comments for nested loop code
 *		Returns the tuple joined from inner and outer tuples which
 *		satisfies the qualification clause.
 *
 *		It scans the inner relation to join with current outer tuple.
 *
 *		If none is found, next tuple from the outer relation is retrieved
 *		and the inner relation is scanned from the beginning again to join
 *		with the outer tuple.
 *
 *		NULL is returned if all the remaining outer tuples are tried and
 *		all fail to join with the inner tuples.
 *
 *		NULL is also returned if there is no tuple from inner relation.
 *
 *		Conditions:
 *		  -- outerTuple contains current tuple from outer relation and
 *			 the right son(inner relation) maintains "cursor" at the tuple
 *			 returned previously.
 *				This is achieved by maintaining a scan position on the outer
 *				relation.
 *
 *		Initial States:
 *		  -- the outer child and the inner child
 *			   are prepared to return the first tuple.
 *
 *
 *	New comments for index stems -- Amol Deshpande
 *
 *		The old nested loop join code also handles index joins.
 *		We will convert it to do SteM kind of index joins by utilizing
 *		almost the same interface.
 *		The main difference between the two will be the processing
 *		of outer tuples. These tuples will be passed on from the Eddy
 *		and not obtained from the outer relation directly.
 *		Minor differences include using intermediateHeapTuple data structures etc.
 *		This is similar in spirit to the way hashjoin code was converted
 *		to SteMs.
 * ----------------------------------------------------------------
 */
TupleTableSlot *
ExecIndexSteM(IndexSteM * node)
{
	IndexSteMState *isstate;
	Plan	   *innerPlan;

	TupleTableSlot *outerTupleSlot;
	TupleTableSlot *innerTupleSlot;

	TupleTableSlot *result;

	IntermediateHeapTuple returnval;

	List	   *joinqual;
	List	   *otherqual;

	ExprContext *econtext;

	/*
	 * get information from the node
	 */
	ENL1_printf("getting info from node");

	isstate = node->isstate;
	joinqual = node->join.joinqual;
	otherqual = node->join.plan.qual;
	innerPlan = innerPlan((Plan *) node);
	econtext = isstate->jstate.cs_ExprContext;

	/*
	 * get the current outer tuple
	 */
	outerTupleSlot = isstate->jstate.cs_OuterTupleSlot;
	econtext->ecxt_outertuple = outerTupleSlot;

	/** --- CODE TO CHECK PROJECTING OUT TUPLES FROM A PREVIOUS JOIN TUPLE DELETED -- **/

	/*
	 * Reset per-tuple memory context to free any expression evaluation
	 * storage allocated in the previous tuple cycle.  Note this can't
	 * happen until we're done projecting out tuples from a join tuple.
	 */
	ResetExprContext(econtext);

	/*
	 * Ok, everything is setup for the join so now loop until we return a
	 * qualifying join tuple.
	 */
	ENL1_printf("entering main loop");

	if (isstate->is_NeedNewOuter)
		return NULL;
	else
	{
		/*
		 * we have an outerTuple, try to get the next inner tuple.
		 */
		for (;;)
		{
			innerTupleSlot = ExecProcNode(innerPlan, (Plan *) node);

			econtext->ecxt_innertuple = innerTupleSlot;

			if (TupIsNull(innerTupleSlot))
			{
				isstate->is_NeedNewOuter = true;

				/** --- CODE TO DO OUTERJOINS DELETED -- **/

				return NULL;
			}

			/*
			 * at this point we have a new pair of inner and outer tuples
			 * so we test the inner and outer tuples to see if they
			 * satisfy the node's qualification.
			 *
			 * Only the joinquals determine MatchedOuter status, but all
			 * quals must pass to actually return the tuple.
			 */
			ENL1_printf("testing qualification");

			if (ExecQual(joinqual, econtext, false))
			{
				isstate->is_MatchedOuter = true;		/* this is for
														 * outerjoins which we
														 * don't really use */

				if (otherqual == NIL || ExecQual(otherqual, econtext, false))
				{
					/*
					 * qualification was satisfied so we project and
					 * return the slot containing the result tuple using
					 * ExecProject().
					 */
					returnval = &isstate->is_tempiHeapTupleData;
					copyIntermediateHeapTuple(returnval, (IntermediateHeapTuple) outerTupleSlot->val);

					/*
					 * printf("Printing inner tuple scanned from the
					 * index\n");
					 */

					/*
					 * debugtup(innerTupleSlot->val,
					 * innerTupleSlot->ttc_tupleDescriptor, 0);
					 */
					/* printf("Printing returnval\n"); */
					/* debugitup(returnval);  */

					/** Note that the innerTupleSlot contains a HeapTuple, not a IntermediateHeapTuple */
					AddBaseTuple(returnval, innerTupleSlot->val, innerTupleSlot->ttc_tupleDescriptor,
					  node->sourceBuiltOnID - 1, returnval->t_timestamp);

					result = ExecStoreTuple((HeapTuple) returnval,		/* tuple to store */
									  isstate->jstate.cs_ResultTupleSlot,		/* CommonState seems to
																				 * have a result tuple
																				 * slot for precisely
																				 * this purpose */
											InvalidBuffer,
											false);		/* do not pfree this
														 * tuple */

					result->ttc_isIntermediateTuple = true;		/* cs_ResultTupleSlot
																 * was never set to have
																 * intermediate tuple */

					return result;
				}
			}

			/*
			 * Tuple fails qual, so free per-tuple memory and try again.
			 */
			ResetExprContext(econtext);

			ENL1_printf("qualification failed, looping");
		}
	}
}


void
ExecIndexSteMSetNewOuter(IndexSteM * istem, ExprContext *econtext)
{
	IndexSteMState *isstate = istem->isstate;

	/** Get the inner index node */
	Plan	   *innerPlan = innerPlan((Plan *) istem);

	assert(isstate->is_NeedNewOuter);

	isstate->is_NeedNewOuter = false;

	ENL1_printf("saving new outer tuple information");

	isstate->jstate.cs_OuterTupleSlot = econtext->ecxt_scantuple;

	econtext->ecxt_outertuple = econtext->ecxt_scantuple;

	/*
	 * now rescan the inner plan
	 */
	ENL1_printf("rescanning inner plan");

	/*
	 * The scan key of the inner plan might depend on the current outer
	 * tuple (e.g. in index scans), that's why we pass our expr context.
	 */

	/*
	 * TODO: This is going to need to changed so that the IndexScan knows
	 * what to do
	 */

	/*
	 * with the outer tuple correctly since the outer tuple is going to be
	 * an
	 */
	/* intermediateHeapTuple */
	ExecReScan(innerPlan, econtext, (Plan *) istem);
}


/* ----------------------------------------------------------------
 *		ExecInitIndexSteM
 *
 *		Creates the run-time state information for the indexstem node
 *
 * ----------------------------------------------------------------
 */
bool
ExecInitIndexSteM(IndexSteM * node, EState *estate, Plan *parent)
{
	IndexSteMState *isstate;

	/*
	 * assign execution state to node
	 */
	node->join.plan.state = estate;

	/*
	 * create new index stem state
	 */
	isstate = makeNode(IndexSteMState);
	node->isstate = isstate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &isstate->jstate);

	/*
	 * now initialize children -- no outer
	 */
	ExecInitNode(innerPlan((Plan *) node), estate, (Plan *) node);

#define INDEXSTEM_NSLOTS 2

	/*
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &isstate->jstate);

	switch (node->join.jointype)
	{
		case JOIN_INNER:
			break;
		case JOIN_LEFT:
			assert(!"We don't support outer joins.");
			isstate->is_NullInnerTupleSlot = ExecInitNullTupleSlot(estate,
							   ExecGetTupType(innerPlan((Plan *) node)));
			break;
		default:
			elog(ERROR, "ExecInitNestLoop: unsupported join type %d",
				 (int) node->join.jointype);
	}

	/*
	 * initialize tuple type and projection info
	 */
	ExecAssignResultTypeFromTL((Plan *) node, &isstate->jstate);
	ExecAssignProjectionInfo((Plan *) node, &isstate->jstate);

	/*
	 * finally, wipe the current outer tuple clean.
	 */
	isstate->jstate.cs_OuterTupleSlot = NULL;
	isstate->jstate.cs_TupFromTlist = false;
	isstate->is_NeedNewOuter = true;
	isstate->is_MatchedOuter = false;

	NL1_printf("ExecInitNestLoop: %s\n",
			   "node initialized");
	return TRUE;
}

int
ExecCountSlotsIndexSteM(IndexSteM * node)
{
	return ExecCountSlotsNode(innerPlan(node)) + INDEXSTEM_NSLOTS;
}

/* ----------------------------------------------------------------
 *		ExecEndIndexSteM
 *
 *		closes down scans and frees allocated storage
 * ----------------------------------------------------------------
 */
void
ExecEndIndexSteM(IndexSteM * node)
{
	IndexSteMState *isstate;

	NL1_printf("ExecEndNestLoop: %s\n", "ending node processing");

	/*
	 * get info from the node
	 */
	isstate = node->isstate;

	/*
	 * Free the projection info
	 *
	 * Note: we don't ExecFreeResultType(isstate) because the rule manager
	 * depends on the tupType returned by ExecMain().  So for now, this is
	 * freed at end-transaction time.  -cim 6/2/91
	 */
	ExecFreeProjectionInfo(&isstate->jstate);
	ExecFreeExprContext(&isstate->jstate);

	/*
	 * close down subplans
	 */
	ExecEndNode(innerPlan((Plan *) node), (Plan *) node);

	/*
	 * clean out the tuple table
	 */
	ExecClearTuple(isstate->jstate.cs_ResultTupleSlot);

	NL1_printf("ExecEndNestLoop: %s\n",
			   "node processing ended");
}

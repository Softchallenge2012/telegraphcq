/*-------------------------------------------------------------------------
 *
 * nodeSteMHashjoin.c
 *	  Routines to handle SteM hash join nodes
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeSteMHashjoin.c,v 1.4 2004/03/24 08:10:55 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <sys/types.h>

#include "postgres.h"

#include "executor/executor.h"
#include "executor/nodeSteM.h"
#include "executor/nodeSteMHashjoin.h"
#include "optimizer/clauses.h"
#include "utils/memutils.h"
#include "assert.h"

static TupleTableSlot *ExecSteMHashJoinOuterGetTuple(Plan *node, Plan *parent, HashJoinState *hjstate);


/* ----------------------------------------------------------------
 *		ExecSteMHashJoin
 *
 *		This function implements the Hybrid Hashjoin algorithm.
 *		recursive partitioning remains to be added.
 *		Note: the relation we build hash table on is the inner
 *			  the other one is outer.
 * ----------------------------------------------------------------
 */
TupleTableSlot *				/* return: a tuple or NULL */
ExecSteMHashJoin(SteMHashJoin * node)
{
	HashJoinState *hjstate = node->hashjoinstate;
	bool		hashPhaseDone = hjstate->hj_hashdone;
	Node	   *outerNode = (Node *) outerPlan(node);
	SteM	   *stem = (SteM *) innerPlan(node);
	ExprContext *econtext = hjstate->jstate.cs_ExprContext;

	assert(IsA(stem, SteM));

	ResetExprContext(econtext);

	/*
	 * if this is the first call, build the hash table for inner relation
	 */
	if (!hashPhaseDone)
	{
		ExecSteMPopulateHashTable(stem);

		printf("Populatig the hash table\n");
		/* printSteMHashJoinTable(stem->stemstate->stemhashtable); */

		hjstate->hj_hashdone = true;
	}


	/*
	 * First, check if the SteM needs an outer tuple. If yes, fetch the
	 * outertuple, set it and then call execprocnode on the SteM.
	 */
	for (;;)
	{
		TupleTableSlot *result;

		if (stem->stemstate->sm_NeedNewOuter)
		{
			TupleTableSlot *outerTupleSlot = ExecSteMHashJoinOuterGetTuple((Plan *) outerNode,
														   (Plan *) node,
																hjstate);

			IntermediateHeapTuple iht = &node->tempiHeapTupleData;

			if (TupIsNull(outerTupleSlot))
			{
				/* We are done */
				return NULL;
			}

			/*
			 * If the outernode was a sequential scan, then we need to
			 * convert from the base tuple to intermediateHeapTuple.
			 */
			if (IsA(outerNode, SeqScan))
			{
				/*
				 * In that case, the tuple has not been converted into the
				 * intermediate tuple format. Do so.
				 */
				int			sourceID = ((SeqScan *) outerNode)->scanrelid - 1;

				initFromBaseTuple(iht, outerTupleSlot->val, outerTupleSlot->ttc_tupleDescriptor, sourceID);

				outerTupleSlot->val = (HeapTuple) iht;
				outerTupleSlot->ttc_isIntermediateTuple = true;
				outerTupleSlot->ttc_shouldFree = false;
			}

			econtext->ecxt_scantuple = outerTupleSlot;

			ExecSteMSetNewOuter(stem, econtext);
		}

		result = ExecProcNode((Plan *) stem, (Plan *) node);


		if (!TupIsNull(result))
		{
			ExprDoneCond isDone;

			/* debugitup((IntermediateHeapTuple) result->val); */

			result = ExecStoreTuple(result->val,
									hjstate->jstate.cs_ProjInfo->pi_slot,
									InvalidBuffer, false);

			/*
			 * at this time, we still have an intermediateheaptuple.
			 * project, later, will convert this to a heaptuple in some
			 * cases.
			 */
			result->ttc_isIntermediateTuple = true;

			/** Project at this point if we want to. */
			if (node->doProject)
			{
				/* WARNING !!! HACK */
				hjstate->jstate.cs_ProjInfo->pi_exprContext->ecxt_scantuple = result;

				result = ExecProject(hjstate->jstate.cs_ProjInfo, &isDone);

				result->ttc_isIntermediateTuple = false;
			}

			return result;
		}
	}

	assert(0);

	return NULL;
}

/* ----------------------------------------------------------------
 *		ExecInitSteMHashJoin
 *
 *		Init routine for HashJoin node.
 * ----------------------------------------------------------------
 */
bool							/* return: initialization status */
ExecInitSteMHashJoin(SteMHashJoin * node, EState *estate, Plan *parent)
{
	HashJoinState *hjstate;
	Plan	   *outerNode;
	SteM	   *stemNode;

	/*
	 * assign the node's execution state
	 */
	node->join.plan.state = estate;

	/*
	 * create state structure
	 */
	hjstate = makeNode(HashJoinState);
	node->hashjoinstate = hjstate;

	/*
	 * Miscellaneous initialization
	 *
	 * create expression context for node
	 */
	ExecAssignExprContext(estate, &hjstate->jstate);

	/*
	 * initializes child nodes
	 */
	outerNode = outerPlan((Plan *) node);
	stemNode = (SteM *) innerPlan((Plan *) node);

	ExecInitNode(outerNode, estate, (Plan *) node);
	ExecInitNode((Plan *) stemNode, estate, (Plan *) node);

#define HASHJOIN_NSLOTS 3

	/*
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &hjstate->jstate);
	hjstate->hj_OuterTupleSlot = ExecInitExtraTupleSlot(estate);

	switch (node->join.jointype)
	{
		case JOIN_INNER:
			break;
		case JOIN_LEFT:
			hjstate->hj_NullInnerTupleSlot =
				ExecInitNullTupleSlot(estate,
									  ExecGetTupType((Plan *) stemNode));
			break;
		default:
			elog(ERROR, "ExecInitHashJoin: unsupported join type %d",
				 (int) node->join.jointype);
	}

	/*
	 * now for some voodoo.  our temporary tuple slot is actually the
	 * result tuple slot of the Hash node (which is our inner plan).  we
	 * do this because Hash nodes don't return tuples via ExecProcNode()
	 * -- instead the hash join node uses ExecScanHashBucket() to get at
	 * the contents of the hash table.	-cim 6/9/91
	 */
	{
		SteMState  *stemstate = stemNode->stemstate;
		TupleTableSlot *slot = stemstate->cstate.cs_ResultTupleSlot;

		hjstate->hj_HashTupleSlot = slot;
	}

	/*
	 * initialize tuple type and projection info
	 */
	ExecAssignResultTypeFromTL((Plan *) node, &hjstate->jstate);
	ExecAssignProjectionInfo((Plan *) node, &hjstate->jstate);

	ExecSetSlotDescriptor(hjstate->hj_OuterTupleSlot,
						  ExecGetTupType(outerNode),
						  false);

	/*
	 * initialize hash-specific info
	 */

	hjstate->hj_hashdone = false;

	return TRUE;
}

int
ExecCountSlotsSteMHashJoin(SteMHashJoin * node)
{
	return ExecCountSlotsNode(outerPlan(node)) +
		ExecCountSlotsNode(innerPlan(node)) +
		HASHJOIN_NSLOTS;
}

/* ----------------------------------------------------------------
 *		ExecEndSteMHashJoin
 *
 *		clean up routine for HashJoin node
 * ----------------------------------------------------------------
 */
void
ExecEndSteMHashJoin(SteMHashJoin * node)
{
	/*
	 * clean up subtrees
	 */
	ExecEndNode(outerPlan((Plan *) node), (Plan *) node);
	ExecEndNode(innerPlan((Plan *) node), (Plan *) node);
}

/* ----------------------------------------------------------------
 *		ExecHashJoinOuterGetTuple
 *
 *		get the next outer tuple for hashjoin: either by
 *		executing a plan node as in the first pass, or from
 *		the tmp files for the hashjoin batches.
 * ----------------------------------------------------------------
 */

static TupleTableSlot *
ExecSteMHashJoinOuterGetTuple(Plan *node, Plan *parent, HashJoinState *hjstate)
{
	TupleTableSlot *slot;

	slot = ExecProcNode(node, parent);

	if (!TupIsNull(slot))
		return slot;
	else
		return NULL;
}

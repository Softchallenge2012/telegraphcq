/*-------------------------------------------------------------------------
 *
 * nodeSymHashJoin.c
 *	  Routines to handle symmetric hash joins.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *-------------------------------------------------------------------------
 */
#include <sys/types.h>

#include "postgres.h"

#include "executor/executor.h"
#include "executor/nodeSteM.h"
#include "executor/nodeASteM.h"
#include "executor/nodeSymHashJoin.h"
#include "utils/memutils.h"
#include "assert.h"


/* ----------------------------------------------------------------
 *		ExecSymHashJoin
 *
 * ----------------------------------------------------------------
 */
TupleTableSlot *				/* return: a tuple */
ExecSymHashJoin(SymHashJoin *node)
{
	SymHashJoinState *shjstate = node->shjstate;
	ExprContext *econtext = shjstate->jstate.cs_ExprContext;
	
	bool isExhausted = false;
	bool currentSteMHasProbe;
	ASteM *buildSteM;
	ASteM *probeSteM;
	TupleTableSlot *result;
	TupleTableSlot *probe;
	ExprDoneCond isDone;
			
	List *otherqual = node->join.plan.qual;
	List *joinqual = node->join.joinqual;
	bool inputsExhausted = 0;
		
	/* clear per-tuple memory context */
	/* necessary?? */
	ResetExprContext(econtext);

	/* reset attempts counter */
	node->attempts = 0;
		
	while (! isExhausted)
	{
		currentSteMHasProbe = shjstate->shj_ProbeSent;
		buildSteM = (ASteM*) shjstate->shj_BuildStem;
		probeSteM = (ASteM*) shjstate->shj_ProbeStem;
		result = shjstate->shj_ProbeResult;
		probe = shjstate->shj_CurrentProbe;


		/* just a bunch of assertions for now */
		Assert(IsA(buildSteM, ASteM));
		Assert(IsA(probeSteM, ASteM));
					
		if (! currentSteMHasProbe) 
		{
			/* if the probeStem doesn't have a probe, get one from the build stem */
			probe = ExecASteMBuildTupleAuto(buildSteM);

			if (TupIsNull(probe)) 
			{
				/* no data from build stem, so switch stems */
				inputsExhausted++;
				SymHashJoinReverseRoles(node, &isExhausted);
				currentSteMHasProbe = false;
				continue;
			}
			else 
			{
				/* we got a probe tuple from the build stem */
				ExecSymHashJoinSetNewProbe(node, probe);
				currentSteMHasProbe = true;
			}
		}
		
		/* by now, the probe stem must have a probe */
		result = ExecSymHashJoinGetProbeResult(node);
				
		if (TupIsNull(result)) 
		{
			/* no more probe results, switch roles, try again */
			SymHashJoinReverseRoles(node, &isExhausted);
			currentSteMHasProbe = false;
			continue;
			// return ExecSymHashJoin(node);
		}
		else 
		{
			/* store it as the current result */
			result = ExecStoreTuple(result->val,
									shjstate->shj_ProbeResult,
									InvalidBuffer, false);
			result->ttc_isIntermediateTuple = true;
			
			/*
			 * Ok, right now the build stem's tuple is in the context's outer
			 * slot and the probe stem's tuple is in the context's inner slot.
			 * Before we can call ExecQual on them, we need to make sure that
			 * they are in the correct order based on the physical positioning
			 * of the stems below the SymHashJoinNode (regardless of current
			 * build/probe state, the outer SteM's tuple always needs to be
			 * in the context's outer slot, etc).
			 *
			 * Typically, the build stem's tuple belongs in the outer and
			 * the probe stem's tuple belongs in the inner (thats how stems
			 * work).  So, if the outer is actually the probe stem, we need
			 * to switch the position of the tuples in the expression context.
			 *
			 */
			econtext->ecxt_scantuple = shjstate->shj_ProbeResult;
			
			if (((ASteM*)outerPlan(node)) == buildSteM) 
			{
				econtext->ecxt_innertuple = shjstate->shj_ProbeResult;
				econtext->ecxt_outertuple = shjstate->shj_CurrentProbe;
			}
			else 
			{
				econtext->ecxt_innertuple = shjstate->shj_CurrentProbe;
				econtext->ecxt_outertuple = shjstate->shj_ProbeResult;
			}
						
			/* now its safe to call ExecQual */
			if (ExecQual(joinqual, econtext, false))
			{
				if (otherqual == NIL || ExecQual(otherqual, econtext, false)) 
				{
					if (node->doProject) 
					{
						ProjectionInfo *pi = shjstate->jstate.cs_ProjInfo;
						return ExecProject(pi, &isDone);
					}
					else 
					{
						result = ExecStoreTuple(result->val,
												shjstate->jstate.cs_ResultTupleSlot,
												InvalidBuffer, false);
					
						/* we still have an iht */
						result->ttc_isIntermediateTuple = true;
						
						/* we got a valid result tuple, so return it */
						return result;
					}
				}
				
			}
			
			/* if either ExecQual failed, then we need to look for another match */
			continue;
			// return ExecSymHashJoin(node);
		}

		/* should never get here */
		Assert(0);
	}

	/* should only get here if we've exhausted the SteM's for this round */
	Assert(isExhausted);
	Assert(node->attempts == 0);
	
	/* return a null */
	return NULL;
}

/* ----------------------------------------------------------------
 *		ExecInitSymHashJoin
 *
 *		Init routine for SymHashJoin node.
 * ----------------------------------------------------------------
 */
bool							/* return: initialization status */
ExecInitSymHashJoin(SymHashJoin *node, EState *estate, Plan *parent)
{
	SymHashJoinState 	*shjstate;
	ASteM *innerSteMNode;
	ASteM *outerSteMNode;

	/* let the join/plan know about the state */
	node->join.plan.state = estate;

    /* initialize the SymHashJoinState */
	shjstate = makeNode(SymHashJoinState);
	node->shjstate = shjstate;

	ExecAssignExprContext(estate, &shjstate->jstate);
		
	/* deal with children nodes */
	innerSteMNode = (ASteM *) innerPlan((Plan *) node);
	outerSteMNode = (ASteM *) outerPlan((Plan *) node);

	ExecInitNode((Plan *)innerSteMNode, estate, (Plan *) node);
	ExecInitNode((Plan *)outerSteMNode, estate, (Plan *) node);

	/* initialize the result tuple slot */
	ExecInitResultTupleSlot(estate, &shjstate->jstate);

	/* initialize temporary holder tuple slots */
	shjstate->shj_ProbeResult = ExecInitExtraTupleSlot(estate);
	shjstate->shj_CurrentProbe = ExecInitExtraTupleSlot(estate);
		
		
	switch (node->join.jointype)
	{
		case JOIN_INNER:
			break;
		default:
			elog(ERROR, "ExecInitSymHashJoin: unsupported join type %d",
				 (int) node->join.jointype);
	}

	/*
	 * initialize tuple type and projection info
	 */
	ExecAssignResultTypeFromTL((Plan *) node, &shjstate->jstate);
	ExecAssignProjectionInfo((Plan *) node, &shjstate->jstate);

	/*
	 * initialize sym-hash-specific info
	 */
	shjstate->shj_ProbeSent = false;
	shjstate->shj_BuildStem = (Node*)innerSteMNode;
	shjstate->shj_ProbeStem = (Node*)outerSteMNode;
	node->attempts = 0;
	
	return TRUE;
}

/* ----------------------------------------------------------------
 *		ExecEndSymHashJoin
 *
 *		clean up routine for HashJoin node
 * ----------------------------------------------------------------
 */
void
ExecEndSymHashJoin(SymHashJoin *node)
{
	/* do i need to clear/de-alloc any memory here?? */
	
	/*
	 * clean up subtrees
	 */
	ExecEndNode(outerPlan((Plan *) node), (Plan *) node);
	ExecEndNode(innerPlan((Plan *) node), (Plan *) node);
}


TupleTableSlot *
ExecSymHashJoinGetProbeResult(SymHashJoin *node)
{
	ASteM *probeStem = (ASteM*) node->shjstate->shj_ProbeStem;
	
	assert(node->shjstate->shj_ProbeSent);
	
	return ExecASteMGetNextOutputTuple(probeStem);
}


void
SymHashJoinReverseRoles(SymHashJoin *node, bool *isExhausted) 
{
	SymHashJoinState 	*shjstate = node->shjstate;
	ASteM *futureProbeSteM = (ASteM*) shjstate->shj_BuildStem;
	ASteM *futureBuildSteM = (ASteM*) shjstate->shj_ProbeStem;
		
	/* assertions */
	assert(!ASteMHasProbe(futureProbeSteM));
	assert(!ASteMHasProbe(futureBuildSteM));
		
    /* reset probe status */
	shjstate->shj_ProbeSent = false;

	/* switch build/probe roles */
	shjstate->shj_BuildStem = (Node*)futureBuildSteM;
	shjstate->shj_ProbeStem = (Node*)futureProbeSteM;

	/* clear the tuple table slots */
	ExecClearTuple(shjstate->shj_CurrentProbe);
	ExecClearTuple(shjstate->shj_ProbeResult);
	
	/* set probe slot descriptor based on build stem's tuples */
	ExecSetSlotDescriptor(shjstate->shj_CurrentProbe,
						  ExecGetTupType((Plan*)futureBuildSteM),
						  false);

	/* check to see if this is the last switch before we return null */
	if (node->attempts == node->exhaust_limit) 
	{
		/* we're done so reset the counter */
		*isExhausted = true;
		node->attempts = 0;
	}
	else 
	{
		/* we're not done, increment the counter */
		*isExhausted = false;
		node->attempts++;
	}
}


void
ExecSymHashJoinSetNewProbe(SymHashJoin *node, TupleTableSlot *probe) 
{
	SymHashJoinState *shjstate = node->shjstate;
	ExprContext *econtext = shjstate->jstate.cs_ExprContext;
	ASteM *probeStem = (ASteM*) shjstate->shj_ProbeStem;

	/* store the tuple into the probe slot */
	probe = ExecStoreTuple(probe->val,
						   shjstate->shj_CurrentProbe,
						   InvalidBuffer, false);
	probe->ttc_isIntermediateTuple = true;
	
	/* assert things got moved into an iht */
	Assert(TupIsIHT(probe));
	
	econtext->ecxt_scantuple = shjstate->shj_CurrentProbe;

	ExecASteMSetNewProbe(probeStem, econtext);

	shjstate->shj_ProbeSent = true;	
}


#define SYMHJ_SLOTS 3

int
ExecCountSlotsSymHashJoin(SymHashJoin *node)
{
	Plan *leftTree = outerPlan((Plan*)node);
	Plan *rightTree = innerPlan((Plan*)node);

	assert(leftTree != NULL);
	assert(rightTree != NULL);
	
	return SYMHJ_SLOTS + ExecCountSlotsNode(leftTree) + ExecCountSlotsNode(leftTree);
	
}

/*-------------------------------------------------------------------------
 *
 * nodeFFilter.c
 *	  Routines to handle Filter operation
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFFilter.c,v 1.2 2004/02/13 01:44:00 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/nodeEddy.h"
#include "executor/nodeFjord.h"
#include "executor/nodeFFilter.h"
#include "miscadmin.h"
#include "utils/memutils.h"

bool
ProcessInitFFilter(FFilter * node, EState *estate, Plan *parent)
{
	FjordInitialize(&(node->fjord), estate);	   /* Common * initialization */

	Assert(outerPlan((Plan *) node) == NULL);
	Assert(innerPlan((Plan *) node) == NULL);

	node->total_entered = node->total_exited = 0;
	
	return TRUE;
}

bool
ProcessEndFFilter(FFilter * node, Plan *parent)
{
	
    FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
    FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

    FjordEnd(&node->fjord, eddy);

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(FFilter));
	
	return TRUE;
}

/* ----------------------------------------------------------------
 *		ProcessNextFilter
 * ----------------------------------------------------------------
 */
void
ProcessNextFFilter(FFilter * node, Plan * parent)
{
	bool enqsucc=false;
    FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	Datum		expr_value;
	bool		isNull;
	bool		result;
	bool deqsucc;
	

    FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

#ifdef AMOLDEBUGON
	elog(DEBUG1,"ProcessNextFFilter");
#endif

	/*
	 *  Fetch next IHT from  i/p queue
	 *
	 *  NOTE:
	 *
	 *       Here we just manipulate pointers and modify the actual physical
	 *       Input IHT as necessary and then send on a pointer to the same IHT
	 */
	FJORD_DEQ_INP_PTR(deqsucc,node,&piiht);
	if (! deqsucc)
	{
		return;										/* No input ==> no output */
	}
	node->total_entered++;

	/*
	 * place the current tuple into the expr context
	 */
	input->val = (HeapTuple)iiht;
	input->ttc_isIntermediateTuple=TRUE;
	econtext->ecxt_scantuple = input;
    ResetExprContext(econtext);    
	
	/*
	 * check that the current tuple satisfies the qual-clause
	 */
	expr_value = ExecEvalExpr(node->filterqual, econtext, &isNull, NULL);
	
	result = (!isNull) && (DatumGetBool(expr_value));

	if (result == false)
	{
		List *iter = NULL;
		int qid=-1;

		if (! UseTupleLineage) 
		{
			/* added by garrett to stop tcqbackend from SPINNING forever */
			fjord->doneInput = true;
			return;
		}
		foreach(iter, fjord->usedByQueries)
		{
			qid = lfirsti(iter);
			BitArraySetBit(iiht->t_queriesCompleted, qid);
		}
	}
	
	FJORD_ENQ_OUT_PTR(enqsucc, node, &iiht);
	fjord->doneInput = true;
	node->total_exited++;
}

bool  ProcessRemoveQueryFFilter(FFilter * node, int queryid, Plan *parent)
{
	/* no special state here, so dispatch to the fjord version 
	   directly.
	*/
	return FjordRemoveQuery(&(node->fjord), queryid);
}

static void
ProcessPrintStatsFFilter_with_level(FFilter * node, Plan *parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	elog(level, " FFilter (%s) ",getNodeInformation(eddy, (Node *)node->filterqual));
	
	elog(level, "FFilter: [Tuples entered: %d] [Tuples exited: %d]",
		 node->total_entered, node->total_exited);
}

void
ProcessPrintStatsFFilter(FFilter *node, Plan *parent)
{
	ProcessPrintStatsFFilter_with_level(node,parent,INFO);
}

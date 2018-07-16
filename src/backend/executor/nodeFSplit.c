/*-------------------------------------------------------------------------
 *
 * nodeFSplit.c
 *
 * This operator uses the new Fjord "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFSplit.c,v 1.2 2004/02/13 01:44:01 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeFjord.h"
#include "executor/nodeEddy.h"
#include "executor/nodeFSplit.h"
#include "miscadmin.h"
#include "nodes/nodeFuncs.h"
#include "rqueue/rqdest.h"
#include "utils/bit.h"
#include "utils/memutils.h"

/* #define AMOLDEBUGON 1 */

/* ----------------------------------------------------------------------------
 *	Dynamically-dispatched functions
 * ----------------------------------------------------------------------------
 */
/*
 * ProcessInitFSplit
 */
bool
ProcessInitFSplit(FSplit * node, EState *estate, Plan *parent)
{
	FjordInitialize(&(node->fjord), estate);	   /* Common initialization */

	node->total_entered = 0;
	
	return true;
}
    
/*
 * ProcessRemoveQueryFSplit
 */
bool
ProcessRemoveQueryFSplit(FSplit * node, int queryid, Plan *parent)
{
	return FjordRemoveQuery((Fjord *) node, queryid);
}

/* ----------------------------------------------------------------
 *		ProcessNextFSplit
 * ----------------------------------------------------------------
 */
void
ProcessNextFSplit(FSplit * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;					/* Common Fjord auto declarations */

	bool		enqsucc;
	bool 		deqsucc;
	int 		i;
	
	/*
	 * Fetch data from node
	 */
	FJORD_AUTOVAR_DEFNS;					  /* Common Fjordauto definitions */

#ifdef AMOLDEBUGON
	elog(DEBUG1,"ProcessNextFSplit");
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

	input->val = (HeapTuple) iiht;
	input->ttc_isIntermediateTuple = TRUE;

#ifdef AMOLDEBUGON
	elog(DEBUG1,"ProcessNextFSplit: Got a new tuple");
#endif

	for(i = 0; i < fjord->numOutputs; i++) 
	{
		fjord->driver = i;
		FJORD_ENQ_OUT_PTR(enqsucc, node, &iiht);		 /* Forward the input */
	}
	fjord->doneInput = true;
}

/* ----------------------------------------------------------------
 *		ProcessEndFSplit
 * ----------------------------------------------------------------
 */
bool
ProcessEndFSplit(FSplit * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	FjordEnd(&node->fjord, eddy);

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(FSplit));

	return true;
}

/* ----------------------------------------------------------------
 *		ProcessPrintStatsFSplit_with_level
 * ----------------------------------------------------------------
 */
static void
ProcessPrintStatsFSplit_with_level(FSplit * node, Plan *parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	elog(level, "FSplit: [Tuples entered: %d]", node->total_entered);
}

/* ----------------------------------------------------------------
 *		ProcessPrintStatsFSplit
 * ----------------------------------------------------------------
 */
void
ProcessPrintStatsFSplit(FSplit *node, Plan *parent)
{
	ProcessPrintStatsFSplit_with_level(node,parent,INFO);
}


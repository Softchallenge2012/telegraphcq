/*-------------------------------------------------------------------------
 *
 * nodeGSFilter.c
 *	  Routines to handle GSFilter operation
 *
 * This operator uses the new Fjord "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeGSFilter.c,v 1.27 2005/06/13 02:59:26 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "executor/execdebug.h"
#include "executor/intermediateTuple.h"
#include "executor/nodeFjord.h"
#include "executor/nodeEddy.h"
#include "executor/nodeGSFilter.h"
#include "optimizer/eddy.h"
#include "miscadmin.h"
#include "utils/bit.h"
#include "utils/memutils.h"
#include "nodes/nodeFuncs.h"

#include "telegraphcq/punct.h"

/*
 * Prototypes of static functions
 *
 * Local to this compilation unit and not part of the public interface
 */

static ConstTree *RemoveQueryConstTree(ConstTree * node, int queryid);
static ConstTree *FindAndRemoveLeftmostConstTree(ConstTree * node);
static ConstTree *RemoveNodeConstTree(ConstTree * node);
static List *RemoveQueryConstList(List *constList, int queryid);
static void freeConstTree(ConstTree * n);
static void freeConstList(List *l);

static void checkConstTree(Node *expr,
			   ExprContext *econtext,
			   ConstTree * ct,
			   FunctionCachePtr rangefcache,
			   FunctionCachePtr eqfcache,
			   BitArray failed);

static void checkConstList(Node *expr,
			   ExprContext *econtext,
			   List *ct,
			   FunctionCachePtr fcache,
			   BitArray failed);

/* ----------------------------------------------------------------------------
 *	Dynamically-dispatched functions
 * ----------------------------------------------------------------------------
 */
/*
 * ProcessInitGSFilter
 */
bool
ProcessInitGSFilter(GSFilter * node, EState *estate, Plan *parent)
{
	FjordInitialize(&(node->fjord), estate);	/* Common * initialization */
	node->all_exprs = NIL;
		
	node->tuplesEntered = node->tuplesExited = 0;

	return true;
}

bool
ProcessRemoveQueryGSF(GSFilter * node, int queryid, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */
	
	if (node->ltmap)
	{
		/* elog(DEBUG, "removing from ltmap before"); */
		/* pprint(node->ltmap); */
		node->ltmap = RemoveQueryConstTree(node->ltmap, queryid);
		Assert(verifyConstTree(econtext,
							   node->ltmap, node->ltoperinsert->op_fcache,
							   NULL,
							   false) == true);

		/* elog(DEBUG, "removing from ltmap after"); */
		/* pprint(node->ltmap); */
	}
	if (node->rtmap)
	{
		/* elog(DEBUG, "removing from rtmap before"); */
		/* pprint(node->rtmap); */
		node->rtmap = RemoveQueryConstTree(node->rtmap, queryid);
		Assert(verifyConstTree(econtext,
							   node->rtmap, node->gtoperinsert->op_fcache,
							   NULL,
							   false) == true);
		/* elog(DEBUG, "removing from rtmap after"); */
		/* pprint(node->rtmap); */
	}
	if (node->eqmap)
	{
		/* elog(DEBUG, "removing from eqmap before"); */
		/* pprint(node->eqmap); */
		node->eqmap = RemoveQueryConstList(node->eqmap, queryid);
		/* elog(DEBUG, "removing from eqmap after"); */
		/* pprint(node->eqmap); */
	}
	if (node->nemap)
	{
		/* elog(DEBUG, "removing from nemap before"); */
		/* pprint(node->nemap); */
		node->nemap = RemoveQueryConstList(node->nemap, queryid);
		/* elog(DEBUG, "removing from nemap after"); */
		/* pprint(node->nemap); */
	}
	return FjordRemoveQuery((Fjord *) node, queryid);
}

/* ----------------------------------------------------------------
 *		ProcessNextGSFilter
 * ----------------------------------------------------------------
 */
void
ProcessNextGSFilter(GSFilter * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */

	ConstTree  	*ltmap;
	ConstTree  	*rtmap;
	Node	   	*expr;
	bits8		 range_failed[BASIZE(TCQMAXQUERIES)];
	bits8		 eq_failed[BASIZE(TCQMAXQUERIES)];
	BitIndex	 idx;
	bool		 enqsucc;
	bool 		 deqsucc;
	bool 		 alldone;

	/*
	 * Fetch data from node
	 */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	expr = node->node;
	ltmap = node->ltmap;
	rtmap = node->rtmap;

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
	node->tuplesEntered++;
	input->val = (HeapTuple) iiht;
	input->ttc_isIntermediateTuple = TRUE;

	ResetExprContext(econtext);
	econtext->ecxt_scantuple = input;

	BitArrayClearAll(range_failed, TCQMAXQUERIES);
	BitArrayClearAll(eq_failed, TCQMAXQUERIES);

    /* SPECIAL CASE: Pass punctuation tuples through. */
    if (slotContainsPunct(econtext->ecxt_scantuple)) {
        FJORD_ENQ_OUT_PTR(enqsucc, node, &iiht);
        node->tuplesExited++; 
        return;
    }
    /* END SPECIAL CASE */
	
	if (ltmap != NULL)
	{
		checkConstTree(expr, econtext, ltmap, node->ltoper->op_fcache,
					   node->eqoper->op_fcache, range_failed);
	}
	if (rtmap != NULL)
	{
		checkConstTree(expr, econtext, rtmap, node->gtoper->op_fcache,
					   node->eqoper->op_fcache, range_failed);
	}

	if (node->eqmap != NULL)
	{
		checkConstList(expr, econtext, node->eqmap,
					   node->eqoper->op_fcache, eq_failed);
	}
	if (node->nemap != NULL)
	{
		checkConstList(expr, econtext, node->nemap,
					   node->neoper->op_fcache, eq_failed);
	}
	alldone = true;
	for (idx = 0; idx < TCQMAXQUERIES; idx++)
	{
		if (BitArrayBitIsSet(range_failed, idx) ||
			BitArrayBitIsSet(eq_failed, idx))
		{
			BitArraySetBit(iiht->t_queriesCompleted, idx);
		}
		if (! BitArrayBitIsSet(iiht->t_queriesCompleted, idx)) 
		{
			alldone = false;
		}
	}
	fjord->doneInput = true;
	if (alldone)
	{
		return;								   /* No more work to do in tuple */
	}
	FJORD_ENQ_OUT_PTR(enqsucc, node, &iiht);
	node->tuplesExited++; 
}

bool
ProcessEndGSFilter(GSFilter * node, Plan *parent)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */

	FjordEnd(&node->fjord, eddy);

	pfree(node->node);
	freeConstTree(node->ltmap);
	freeConstTree(node->rtmap);
	freeConstList(node->eqmap);
	freeConstList(node->nemap);

	/* Zero out the struct to help dangling pointer detection. */
	MemSet(node, 0x0, sizeof(GSFilter));

	return true;
}

static void
ProcessPrintStatsGSFilter_with_level(GSFilter * node, Plan *parent, int level)
{
	FJORD_AUTOVAR_DECLS;		/* Common Fjord auto declarations */
	FJORD_AUTOVAR_DEFNS;		/* Common Fjordauto definitions */


	
	elog(level, "GSFilter: [Tuples entered: %d] [Tuples exited: %d]",
		 node->tuplesEntered, node->tuplesExited);
}

void
ProcessPrintStatsGSFilter(GSFilter *node, Plan *parent)
{
	ProcessPrintStatsGSFilter_with_level(node,parent,INFO);
}

/* ----------------------------------------------------------------------------
 *	Static functions local to this compilation unit
 * ----------------------------------------------------------------------------
 */
List *
RemoveQueryConstList(List *list, int queryid)
{
	List	   *deleted = NIL;
	List	   *iterator = NIL;
	ConstTree  *elemBeforeDelete = NULL;
	ConstTree  *elemAfterDelete = NULL;

	foreach(iterator, list)
	{
		elemBeforeDelete = lfirst(iterator);
		elemAfterDelete = RemoveQueryConstTree(elemBeforeDelete, queryid);
		if (elemAfterDelete == NULL)
			deleted = lcons(elemBeforeDelete, deleted);


	}

	foreach(iterator, deleted)
		list = lremove(lfirst(iterator), list);
	return list;
}

void
freeConstList(List *l)
{
	List	   *iter = NIL;

	foreach(iter, l)
		freeConstTree(lfirst(iter));
	freeList(l);
}

ConstTree *
RemoveQueryConstTree(ConstTree * node, int queryid)
{
	ConstTree  *removed = NULL;

	if (node == NULL)
		return NULL;
	node->queries = lremovei(queryid, node->queries);
	node->eqqueries = lremovei(queryid, node->eqqueries);

	node->left = RemoveQueryConstTree(node->left, queryid);
	node->right = RemoveQueryConstTree(node->right, queryid);
	if (length(node->queries) == 0 &&
		length(node->eqqueries) == 0)
	{
		removed = node;
		node = RemoveNodeConstTree(node);
		removed->left = removed->right = NULL;
		freeConstTree(removed);
	}

	return node;
}


ConstTree *
RemoveNodeConstTree(ConstTree * node)
{
	/* returns the new replacement node, and also sets the children of	*/
	/* node to null. the removed node is not deallocated. */
	ConstTree  *replace = NULL;

	if (node == NULL || (node->left == NULL && node->right == NULL))
		return NULL;
	if (node->left == NULL)
		replace = node->right;
	if (node->right == NULL)
		replace = node->left;
	if (replace == NULL)
	{
		/* find a node to replace the one being removed. */
		replace = FindAndRemoveLeftmostConstTree(node->right);
		if (replace == node->right)
		{
			Assert(node->right->left == NULL);
			replace->left = node->left;
		}
		else
		{

			Assert(replace->left == NULL && replace->right == NULL);
			Assert(replace != node);

			replace->left = node->left;
			replace->right = node->right;
		}

	}

	node->left = node->right = NULL;
	return replace;
}

ConstTree *
FindAndRemoveLeftmostConstTree(ConstTree * node)
{
	ConstTree  *ret = NULL;

	if (node->left == NULL)
		return node;

	if (node->left->left == NULL)
	{
		ret = node->left;
		node->left = RemoveNodeConstTree(node->left);
		ret->right = NULL;
		ret->left = NULL;
		return ret;
	}
	else
		return FindAndRemoveLeftmostConstTree(node->left);

}

void
freeConstTree(ConstTree * n)
{
	if (n == NULL)
		return;
	freeConstTree(n->left);
	freeConstTree(n->right);
	pfree(n->c);
	n->left = n->right = NULL;
	freeList(n->queries);
	freeList(n->eqqueries);
	pfree(n);
}




void
checkConstList(Node *expr,
			   ExprContext *econtext,
			   List *l,
			   FunctionCachePtr fcache,
			   BitArray failed)
{
	Datum		result;
	List	   *args = makeList2(expr, NULL);
	bool		isNull;
	ExprDoneCond isDone;
	List	   *iterator;
	List	   *nodeIter = NULL;
	ConstTree  *ct = NULL;


	if (l == NULL)
		return;

	foreach(nodeIter, l)
	{
		ct = lfirst(nodeIter);
		set_nth(args, 1, ct->c);


		result = ExecMakeFunctionResultSwitchContext(fcache, args, econtext, &isNull, &isDone);



		if (DatumGetBool(result) == false)
		{
			foreach(iterator, ct->queries)
				BitArraySetBit(failed, lfirsti(iterator));
		}
	}
	freeList(args);

}

static void
checkConstTree(Node *expr,
			   ExprContext *econtext,
			   ConstTree * ct,
			   FunctionCachePtr rfcache,
			   FunctionCachePtr eqfcache,
			   BitArray failed)

{
	Datum		rangeresult;
	Datum		eqresult;
	bool		rr = false,
				eqr = false;

	List	   *args = NIL;
	List	   *iterator = NIL;
	bool		isNull = false;
	ExprDoneCond isDone;

	if (ct == NULL)
		return;
	args = makeList2(expr, ct->c);

	rangeresult = ExecMakeFunctionResultSwitchContext(rfcache, args, econtext, &isNull, &isDone);


	rr = DatumGetBool(rangeresult);

	if (rr)
		checkConstTree(expr, econtext, ct->left, rfcache, eqfcache, failed);
	else
	{
		/* Tuple failed all queries in this node */
		foreach(iterator, ct->queries)
			BitArraySetBit(failed, lfirsti(iterator));

		if (ct->eqqueries)
		{
			eqresult = ExecMakeFunctionResultSwitchContext(eqfcache, args, econtext,
													   &isNull, &isDone);
			eqr = DatumGetBool(eqresult);
			if (eqr == false)
			{
				foreach(iterator, ct->eqqueries)
					BitArraySetBit(failed, lfirsti(iterator));
			}
		}
		checkConstTree(expr, econtext, ct->right, rfcache, eqfcache, failed);
		checkConstTree(expr, econtext, ct->left, rfcache, eqfcache, failed);
	}
	freeList(args);
	args = NIL;
}

bool
verifyConstTree(ExprContext *econtext,
				ConstTree * ct,
				FunctionCachePtr fcn,
				Const *c,
				bool expectedReturn)
{
	bool		isNull;
	ExprDoneCond isDone;
	bool		funcret = true;
	Datum		ret;

	if (ct == NULL)
		return true;
	if (c != NULL)
	{
		List	   *args = makeList2(c, ct->c);

		ret = ExecMakeFunctionResultSwitchContext(fcn, args, econtext,
												  &isNull, &isDone);
		if (DatumGetBool(ret) != expectedReturn)
		{
			elog(DEBUG1, "const tree does not maintain bst property");
			pprint(c);
			pprint(ct->c);
			funcret = false;
		}
		freeList(args);
		args = NIL;

	}

	funcret = funcret &&
		verifyConstTree(econtext, ct->left, fcn, ct->c, false) &&
		verifyConstTree(econtext, ct->right, fcn, ct->c, true);


	return funcret;
}

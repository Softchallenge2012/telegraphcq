/*-------------------------------------------------------------------------
 *
 * nodeFuncs.c
 *	  All node routines more complicated than simple access/modification
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/nodes/nodeFuncs.c,v 1.13 2004/03/24 08:10:57 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */


#include "postgres.h"

#include "executor/executor.h"	/* @dcacqSK */
#include "nodes/execnodes.h"	/* @dcacqSK */
#include "nodes/makefuncs.h"	/* @dcacqSK */
#include "nodes/nodeFuncs.h"
#include "utils/lsyscache.h"

static bool var_is_inner(Var *var);

/*
 * single_node -
 *	  Returns t if node corresponds to a single-noded expression
 */
bool
single_node(Node *node)
{
	if (IsA(node, Const) ||
		IsA(node, Var) ||
		IsA(node, Param))
		return true;
	else
		return false;
}

/*****************************************************************************
 *		VAR nodes
 *****************************************************************************/

/*
 *		var_is_outer
 *		var_is_inner
 *		var_is_mat
 *		var_is_rel
 *
 *		Returns t iff the var node corresponds to (respectively):
 *		the outer relation in a join
 *		the inner relation of a join
 *		a materialized relation
 *		a base relation (i.e., not an attribute reference, a variable from
 *				some lower join level, or a sort result)
 *		var node is an array reference
 *
 */
bool
var_is_outer(Var *var)
{
	return (bool) (var->varno == OUTER);
}

static bool
var_is_inner(Var *var)
{
	return (bool) (var->varno == INNER);
}

bool
var_is_rel(Var *var)
{
	return (bool)
		!(var_is_inner(var) || var_is_outer(var));
}

/*****************************************************************************
 *		OPER nodes
 *****************************************************************************/

/*
 * replace_opid -
 *
 *		Given a oper node, resets the opfid field with the
 *		procedure OID (regproc id).
 *
 *		Returns the modified oper node.
 *
 */
Oper *
replace_opid(Oper *oper)
{
	oper->opid = get_opcode(oper->opno);
	oper->op_fcache = NULL;
	return oper;
}

/*****************************************************************************
 *		constant (CONST, PARAM) nodes
 *****************************************************************************/

#ifdef NOT_USED
/*
 * non_null -
 *		Returns t if the node is a non-null constant, e.g., if the node has a
 *		valid `constvalue' field.
 */
bool
non_null(Expr *c)
{

	if (IsA(c, Const) &&
		!((Const *) c)->constisnull)
		return true;
	else
		return false;
}
#endif


/*****************************************************************************
 *		CONSTTREE nodes
 *****************************************************************************/
/*
 * add_to_ConstTree
 *
 * This recursive function adds a specified constant "c" of a predicate of query
 * "queryid" to the root of the binary tree pointed to by "ct". If the constant
 * "c" already exists in the tree, the specified "queryid" is just added to the
 * list of queries of the appropriate node in the tree. If the constant does not
 * exist, then an appropriate node with the constant and the queryid is created
 * and inserted in the appropriate position in the tree.
 *
 * NOTES:
 *
 *		(1) This routine does not pay any regard to balancing the tree. However,
 *		this API supports future balancing support by making the addition to the
 *		tree destructive. For this reason, callers MUST make sure that the root
 *		pointer that is passed as a pointer to this argument is replaced with
 *		the value returned by this routine. As of now this will only be
 *		different if the root is NULL. Indeed, this convention is already
 *		followed in the two recursive calls in the body of this function
 *
 *		(2) When the specified constant is not found in the tree, it is cloned
 *		using the "copy" function.
 *
 *		(3) Notice the order in which arguments are passed to the associated
 *		function pointer. This is very crucial. The first argument is ALWAYS the
 *		constant in the ConstTree node. The second argument is ALWAYS the new
 *		constant value. If the operator tests true, then we ALWAYS follow the
 *		right subtree and if it tests false, we ALWAYS follow the left
 *		subtree. So if we have a ConstTree for the "<" operator, the tree might
 *		look like this:
 *
 *										   20
 *										  /  \
 *										 /	  \
 *										/	   \
 *									   10		30
 *									  /  \	   /  \
 *									 5	  15  25   35
 *
 *
 * ct	   (IN)  Root of the ConstTree which has to be modified
 * c	   (IN)  New constant from a predicate that has to be added to the tree
 * queryid (IN) Identifier of the new query associated with the constant
 * fcache  (IN) Function cache pointer of the assoc. operator (pre-initialized)
 *
 * RETURNS		Pointer to appropriately modified ConstTree
 *
 */
ConstTree *
add_to_ConstTree(ConstTree * ct,
				 Const *c,
				 int32 queryid,
				 FunctionCachePtr fcache,
				 bool isRangeEQ)
{
	Datum		result;
	List	   *args;
	bool		isNull;
	ExprDoneCond isDone;

	if (ct == NULL)
	{
		/*
		 * It is the responsibility of this invocation of add_to_ConstTree
		 * to create a new ConstTree structure as the specified constant
		 * does not exist in the tree.
		 */
		ct = makeConstTree(c);
		if (isRangeEQ)
			ct->eqqueries = makeListi1(queryid);
		else
			ct->queries = makeListi1(queryid);
		return ct;
	}

	/*
	 * Check if the specified constant matches the current node
	 */
	if (equal(c, ct->c))
	{
		/*
		 * Add the specified queryid to the list of queries that match
		 * this value
		 */
		if (isRangeEQ)
			ct->eqqueries = lconsi(queryid, ct->eqqueries);
		else
			ct->queries = lconsi(queryid, ct->queries);
		return ct;
	}

	/*
	 * Find the next node to traverse
	 */
	args = makeList2(ct->c, c);

	/*
	 * XXX: Passing econtext as NULL because args are only constants
	 *
	 * XXX: Ignoring result of isNull and isDone
	 */
	result = ExecMakeFunctionResultSwitchContext(fcache, args, NULL, &isNull, &isDone);

	freeList(args);

	/*
	 * Traverse the appropriate node
	 */
	if (DatumGetBool(result))
		ct->right = add_to_ConstTree(ct->right, c, queryid, fcache, isRangeEQ);
	else
		ct->left = add_to_ConstTree(ct->left, c, queryid, fcache, isRangeEQ);
	return ct;
}


List *
add_to_list(List *l,
			Const *c,
			int32 queryid,
			FunctionCachePtr fcache)
{
	Datum		result;
	List	   *args;
	bool		isNull;
	ExprDoneCond isDone;
	bool		found = false;
	ConstTree  *ct = NULL;
	List	   *tmp = NULL;



	foreach(tmp, l)
	{
		ct = lfirst(tmp);
		/* first see if the constant is in the list */
		args = makeList2(ct->c, c);

		/*
		 * XXX: Passing econtext as NULL because args are only constants
		 *
		 * XXX: Ignoring result of isNull and isDone
		 */
		result = ExecMakeFunctionResultSwitchContext(fcache, args, NULL, &isNull, &isDone);
		freeList(args);

		/*
		 * Traverse the appropriate node
		 */
		if (DatumGetBool(result))
		{

			ct->queries = lconsi(queryid, ct->queries);
			found = true;
			break;
		}

	}

	if (found == false)
	{
		ct = makeConstTree(c);
		ct->queries = makeListi1(queryid);
		l = lcons(ct, l);
	}
	return l;
}

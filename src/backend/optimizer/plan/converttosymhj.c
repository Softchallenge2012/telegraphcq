/*
 * convertosymhj.c
 *
 *	  Routines to convert Hash-based Hashjoins to Symmetric SteM-based Hashjoins
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif
#include <assert.h>				/* @deddySK */

#include "access/xlog.h"
#include "commands/async.h"
#include "commands/trigger.h"
#include "commands/variable.h"
#include "libpq/libpq.h"
#include "libpq/pqformat.h"
#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "nodes/print.h"
#include "optimizer/cost.h"
#include "optimizer/planner.h"
#include "optimizer/clauses.h"
#include "parser/analyze.h"
#include "parser/parse.h"
#include "parser/parser.h"
#include "rewrite/rewriteHandler.h"
#include "tcop/fastpath.h"
#include "tcop/pquery.h"
#include "tcop/tcopprot.h"
#include "tcop/utility.h"
#include "storage/proc.h"
#include "utils/guc.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "catalog/pg_class.h"	/* @dtcqpmMS */
#include "rqueue/ingresq.h"		/* @dtcqpmMS */
#include "telegraphcq/wrapch.h" /* @dtcqpmMS */
#include "parser/parsetree.h"	/* @dwchMS */
#include "utils/relcache.h"		/* @dwchMS */
#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif

#include "nodes/nodes.h"
#include "executor/nodeASteM.h"
#include "nodes/nodeFuncs.h"
#include "optimizer/eddy.h"

#include "pgstat.h"

#include "optimizer/var.h"



Plan *
convert_to_sym_hash_join(Plan *plan, Query *querytree)
{
	Plan *ret = NULL;
	WindowClause *wc;
    /*
	WindowExpr *we;
	List *wl;
	int src_id_left, src_id_right;
    */

	wc = querytree->windowClause;
	
	
	if(IsA(plan, SeqScan) || IsA(plan, StrmScan))
	{
		ret = plan;
	}
	else if (IsA(plan, HashJoin)) 
	{
		/* change it to a symmetric hash join */
		HashJoin *hj = (HashJoin *) plan;
		SymHashJoin *shj = makeNode(SymHashJoin);
		Hash *hash = (Hash *) innerPlan(hj);
		Plan *outerChild = outerPlan(hj);
		Var *right_hash_key, *left_hash_key;
		ASteM *stem_right, *stem_left;
		Expr *expr = (Expr *) lfirst(hj->hashclauses);
		Expr *flipped_expr = copyObject(expr);
		List *l;
		List *allvars, *iter;
		List *tl_list = pull_var_clause((Node*)shj->join.plan.targetlist, false);
				
		/* most nodes don't project */
		shj->doProject = false;

		/*
		 * set the 'exhaustion limit' - how many times it tries to
		 * get tuples from its children before it gives up and
		 * returns a null that means no data AT THE CURRENT TIME
		 */
		shj->exhaust_limit = 10;
						
		/* set up the symmetric hash join */
		shj->join = hj->join;
		shj->join.plan.type = T_SymHashJoin;
		shj->joinqual = hj->join.joinqual;
		shj->join.plan.qual = hj->join.plan.qual;

		/* copied from convert_to_stemhashjoins */
		foreach(l, tl_list) 
        {
            Var *var = (Var *) ((TargetEntry *) lfirst(l))->expr;
            if(!IsA(var, Var))
                elog(ERROR, "Only column references may appear in the select list for this query");
			
            var->varno = var->varnoold;
            var->varattno = var->varoattno;
        }

		/* get the left and right hash keys */
		right_hash_key = makeNode(Var);
		*right_hash_key = *(Var *)get_rightop(expr);
		right_hash_key->varno = right_hash_key->varnoold;
		right_hash_key->varattno = right_hash_key->varoattno;

		left_hash_key = makeNode(Var);
		*left_hash_key = *(Var *)get_leftop(expr);
		left_hash_key->varno = left_hash_key->varnoold;
		left_hash_key->varattno = left_hash_key->varoattno;

		/* set up the flipped expr for the outer stem */
		allvars = pull_var_clause((Node*)flipped_expr, false);
		foreach (iter, allvars) 
		{
			Var *v = (Var *) lfirst(iter);
			Assert(IsA(v, Var));
			v->varno = (v->varno == OUTER) ? INNER : OUTER;
		}
		freeList(allvars);
				
		/* set up the inner ASteM */
		stem_right = makeNode(ASteM);
		stem_right->innerhashkey = (Node *) right_hash_key;
		stem_right->outerhashkey = (Node *) left_hash_key;
		stem_right->stemclauses = makeList1(expr);
		//BitArraySetBit(stem_right->sourceBuiltOn,(right_hash_key->varno - 1));
				
		/* set up the outer SteM */
		stem_left = makeNode(ASteM);
		stem_left->innerhashkey = (Node *) left_hash_key;
		stem_left->outerhashkey = (Node *) right_hash_key;
		stem_left->stemclauses = makeList1(flipped_expr);
		//BitArraySetBit(stem_left->sourceBuiltOn,(left_hash_key->varno - 1));

		/* attach SteMs to SymHashJoin */
		outerPlan(shj) = (Plan*)stem_left;
		innerPlan(shj) = (Plan*)stem_right;

		/* Recurse to create children of SteMs (skipping over Hash) */
		outerPlan(stem_left) = convert_to_sym_hash_join(outerChild, querytree);
		outerPlan(stem_right) = convert_to_sym_hash_join(outerPlan(hash), querytree);


		/* deal with windowing */
		if (wc != NULL) 
		{
			stem_left->wc = copyObject(wc);
			stem_right->wc = copyObject(wc);
		}

		ret = (Plan *) shj;
	}
	else 
	{
		nodeDisplay(plan);
		elog(ERROR, "converttosymhj: Don't know how to handle this query plan %d", plan->type);
	}

	return ret;
}

Plan *
pg_convert_to_sym_hash_join(Plan *plan, Query *querytree)
{
	Plan *ret = NULL;
	WindowClause *wc = querytree->windowClause;
	List *wl;
	WindowExpr *we;
	char *minusopname = "-";
	char *le_opname = "<";
	Node *minusexpr, *le_expr;
	
	elog(DEBUG1, "Converting to symmetric hash joins");

	/*
	 * does this need to skip over any group, agg,
	 * etc nodes on the top of the plan?
	 */	
	ret = convert_to_sym_hash_join(plan, querytree);

	/*
	 * make sure the top SymHashJoin projects
	 */
	if (IsA(ret, SymHashJoin)) 
	{
		((SymHashJoin *)ret)->doProject = TRUE;

		if (wc != NULL) 
		{
			/* add a windowing predicate to the SymHashJoin */
			for (wl = (wc->windowExprs); wl != ((List*)0); wl = wl->next) 
			{
				we = (WindowExpr*) lfirst(wl);

				/*
				 * Construct an expression that evaluates:
				 *
				 *    (wc->max - we->tsvar) < we->rangeint
				 *
				 * Where: <wc->max> is a function that fetches the maximum timestamp
				 * from a tuple (e.g. highest timestamp of any of the base tuples
				 * that make up an intermediate tuple). <we->tsvar> is an
				 * expression that pulls a timestamp out of a tuple. <we->reltime>
				 * is the window size of the current window expression, expressed
				 * as a string (e.g. "1 second").
				 */
				
				minusexpr = (Node *) make_op(makeList1(makeString(minusopname)), wc->max, (Node *)we->tsvar);
				replace_opid((Oper*) ((Expr*) minusexpr)->oper);
				le_expr = (Node *) make_op(makeList1(makeString(le_opname)), minusexpr, (Node *)we->rangeint);
				replace_opid((Oper*) ((Expr*) le_expr)->oper);
				
				/* now add this qual to the joinqual list of the top sym hash join */
				lcons(le_expr, ((SymHashJoin*) ret)->joinqual);
			}			
		}		
	}
	
	return ret;
}

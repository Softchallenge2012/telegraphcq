/*-------------------------------------------------------------------------
 *
 * convertoeddy.c
 *
 *	  Routines to convert a "normal" PostgreSQL plan to an Eddy plan.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/convertoeddy.c,v 1.54 2006/02/13 00:45:30 phred Exp $
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
#include "optimizer/clauses.h"
#include "optimizer/cost.h"
#include "optimizer/eddy.h"
#include "optimizer/planner.h"
#include "optimizer/var.h"
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
#include "rewrite/rewriteManip.h"
#include "executor/nodeStrmScan.h"
#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif

#include "pgstat.h"
Plan	   *create_eddy_from_scan(Plan *scan);
Plan	   *convert_subplan_to_eddy(Plan *plan);
Plan	   *pg_convert_plan_to_eddy(Plan *plan, List *rangetable);
Plan	   *pg_convert_plan_to_stemhashjoins(Plan *plan);
Plan	   *convert_to_sym_hash_join(Plan *plan, Query *querytree);
Plan	   *pg_convert_to_sym_hash_join(Plan *plan, Query *querytree);
void		create_stems_from_hashjoin(HashJoin *plan, SteM ** stem_left, SteM ** stem_right);
void		create_stems_from_mergejoin(MergeJoin *plan, SteM ** stem_left, SteM ** stem_right);
void		create_stems_from_nestloopjoin(NestLoop *plan, SteM ** stem_left, SteM ** stem_right);
void		find_sources_and_hash_joins(Plan *plan, List **list_sources, List **list_hashjoins);
void		add_sources_modules_to_eddy_recursively(Eddy * eddy, Plan *plan, Plan *parent);
IndexSteM  *create_indexstem_from_indexscan(IndexScan *plan, NestLoop *parent);
void		readjust_query_rangetable(Query *querytree);
static bool readjust_range_table(List *old_rtable, List **new_rtable_ptr, int *new_indexes);
bool		readjusting_required(List *old_rtable);
void		create_stems_from_eq_expr(Expr *expr, SteM ** stem_left_ptr, SteM ** stem_right_ptr);
int         check_uniqueness_and_get_varno(Node *node);

void		munge_source_into_eddy(Eddy * e, Plan *plan);

void		ChangeVarNodes(Node *node, int rt_index, int new_index, int sublevels_up);
void		join_quals_to_filter(Eddy * eddy, List * p);
void use_old_varnos(Node * n);

void add_strmscans_into_eddy(Eddy *e, Plan *plan);

/*
 * makeFilter
 *
 * Makes a new Filter node with the supplied qualifier.
 *
 *
 */
static Filter *
makeFilter(Node *fqual)
{
	Filter	   *f = makeNode(Filter);

	f->filterqual = fqual;
	return f;
}

/*
 * Munge a source into an Eddy
 *
 * If the UseCQEddy global is set, this also extracts plan qualifiers into
 * separate Filter modules.
 *
 */
void
munge_source_into_eddy(Eddy * e, Plan *plan)
{
	List	   *l;
	Node	   *expr;
	Filter	   *f;
	List	   *quals = NULL;

	elog(DEBUG1, "Munging source. UseCQEddy is %s", UseCQEddy ? "true" : "false");

	if (UseCQEddy)				/* Extract filters */
	{
		if (IsA(plan, StrmScan))
		{
			StrmScan   *strmscan = (StrmScan *) plan;

			quals = strmscan->seqscan->plan.qual;
			strmscan->scansock.plan.qual = NULL;
			strmscan->seqscan->plan.qual = NIL;
		}
		else
		{
			quals = plan->qual;
			plan->qual = NIL;
		}

		foreach(l, quals)
		{
			expr = lfirst(l);
			if ((nodeTag(expr) == T_Expr))
			{
				Expr	   *exp = (Expr *) expr;

				if (exp->opType != OP_EXPR)
					elog(ERROR, "unsupported qualification type");
/* 				if ((length(exp->args) != 2) || */
/* 					(!(IsA((Node *) nth(0, exp->args), Const) || */
/* 					   IsA((Node *) nth(1, exp->args), Const)))) */
				if (length(exp->args) != 2) 
				{
					elog(ERROR, " filter qualifications must be of the form "
						 "[expr|const] OP [expr|const] ");
				}
			}
			f = makeFilter(expr);
			add_planitem_to_eddy(e, (Plan *) f, EDDY_MODULE);
		}


		freeList(quals);		/* Frees only list structure, not contents */
	}

	add_planitem_to_eddy(e, (Plan *) plan, EDDY_SOURCE);
}


Plan *
create_eddy_from_scan(Plan *scan)
{
	Plan	   *ret = NULL;

	if (IsA(scan, SeqScan) ||IsA(scan, StrmScan))
	{
		/* Munge the scan node to strip out quals and create a list */
		/* of Filter nodes for each qual */
		Eddy	   *e = makeNode(Eddy);

		e->cacq = false;

		e->plan.targetlist = scan->targetlist;

		munge_source_into_eddy(e, scan);

		ret = (Plan *) e;
	}
	return ret;
}

Plan *
convert_subplan_to_eddy(Plan *plan)
{
	Eddy	   *e;
	List	   *l;

	elog(DEBUG1, "frontend is converting the plan to an eddy");

	e = makeNode(Eddy);

	e->plan.targetlist = plan->targetlist;

	/*
	 * For each of the target list variables, we will replace varno and
	 * varattno with varnoold and varoattno. We should really figure out a
	 * more clean way to deal with this confusion.
	 */
	foreach(l, e->plan.targetlist)
	{
		use_old_varnos(lfirst(l));
	}


	add_sources_modules_to_eddy_recursively(e, plan, NULL);

	printf("Number of sources = %d and Number of modules = %d \n", e->numSources, e->numModules);

	freeList(plan->qual);

	plan->qual = NIL;
	return (Plan *) e;
}

void
add_sources_modules_to_eddy_recursively(Eddy * eddy, Plan *plan, Plan *parent)
{
	if (IsA(plan, SeqScan) ||IsA(plan, StrmScan))
		munge_source_into_eddy(eddy, plan);
	else if (IsA(plan, Hash))
		add_sources_modules_to_eddy_recursively(eddy, outerPlan(plan), plan);
	else if (IsA(plan, HashJoin))
	{
		SteM	   *stem1,
				   *stem2;

		/** Recurse first */
		add_sources_modules_to_eddy_recursively(eddy, outerPlan(plan), plan);
		add_sources_modules_to_eddy_recursively(eddy, innerPlan(plan), plan);

		join_quals_to_filter(eddy, ((HashJoin*)plan)->join.joinqual);
		((HashJoin*)plan)->join.joinqual=NULL;
		create_stems_from_hashjoin((HashJoin *) plan, &stem1, &stem2);

		add_planitem_to_eddy(eddy, (Plan *) stem1, EDDY_MODULE);
		add_planitem_to_eddy(eddy, (Plan *) stem2, EDDY_MODULE);
	}
	else if (IsA(plan, Sort))
		add_sources_modules_to_eddy_recursively(eddy, outerPlan(plan), plan);
	else if (IsA(plan, MergeJoin))
	{
		SteM	   *stem1,
				   *stem2;

		/** Recurse first */
		add_sources_modules_to_eddy_recursively(eddy, outerPlan(plan), plan);
		add_sources_modules_to_eddy_recursively(eddy, innerPlan(plan), plan);

		join_quals_to_filter(eddy, ((MergeJoin*)plan)->join.joinqual);
		((MergeJoin*)plan)->join.joinqual=NULL;
		create_stems_from_mergejoin((MergeJoin *) plan, &stem1, &stem2);

		add_planitem_to_eddy(eddy, (Plan *) stem1, EDDY_MODULE);
		add_planitem_to_eddy(eddy, (Plan *) stem2, EDDY_MODULE);
	}
	else if (IsA(plan, NestLoop))
	{
		/** Nested loop has some special cases because Indexes also use NestLoop nodes.
		 *	XXX: It is not clear if this is correct in all situations, but right now
		 *	we will create stems for a nested loop only if neither of the children are
		 *	index scans. */

		Plan	   *inner = innerPlan(plan);

        elog(LOG, "NestLoop");

		if (IsA(inner, Material))		/* Watch out for materialization points! */
		{
			inner = outerPlan(inner);	/* Complicated inners have
										 * Materials */
		}

		/* Recurse in any case. */
		add_sources_modules_to_eddy_recursively(eddy, inner, plan);
		add_sources_modules_to_eddy_recursively(eddy, outerPlan(plan), plan);

		if ((!IsA(inner, IndexScan)) && (!IsA(outerPlan(plan), IndexScan)))
		{
			SteM	   *stem1,
					   *stem2;
			NestLoop * nl = (NestLoop*)plan;

			create_stems_from_nestloopjoin(nl, &stem1, &stem2);

            /* create_stems_from_nestloopjoin() has removed the equality
             * predicate from the join.  Add any remaining predicates as
             * filters. */
            if (NULL != nl->join.joinqual) {
                join_quals_to_filter(eddy, nl->join.joinqual);
            }

			add_planitem_to_eddy(eddy, (Plan *) stem1, EDDY_MODULE);
			add_planitem_to_eddy(eddy, (Plan *) stem2, EDDY_MODULE);
		}
	}
	else if (IsA(plan, IndexScan))
	{
		IndexSteM  *istem;

		assert(IsA(parent, NestLoop));

		istem = create_indexstem_from_indexscan((IndexScan *) plan, (NestLoop *) parent);
		add_planitem_to_eddy(eddy, (Plan *) istem, EDDY_MODULE);
	}
	else
	{
		nodeDisplay(plan);
		elog(ERROR, "convertoeddy1: Don't know how to handle this query plan %d", plan->type);
		printf("convertoeddy1: Don't know how to handle this query plan %d\n", plan->type);
	}
}

/** Given an IndexScan node, create a IndexSteM from it...
 *	  The things to do here are :
 *		create the new indexstem node with appropriate pointer to the indexscan
 *		change the indexscan->indxqual so that indxqual knows what to do with
 *		  intermediateheaptuple outers that it is passed.
 */
IndexSteM *
create_indexstem_from_indexscan(IndexScan *plan, NestLoop *parent)
{
	IndexSteM  *ret = makeNode(IndexSteM);

	List	   *list_qual;
	Expr	   *index_clause;
	Node	   *rightexpr,
			   *leftexpr;

	/** Create and initialize the indexstem structures */

	/*
	 * we can copy pretty much all the state from the parent nested loops,
	 * except for the nodetag
	 */
	ret->join = parent->join;
	NodeSetTag(ret, T_IndexSteM);

	/*
	 * XXX == should delete this from here... we don't initialize state
	 * here
	 */
	ret->isstate = makeNode(IndexSteMState);

	/** Make appropriate changes to the IndexScan itself.
	 *
	 * This code is modelled on the way IndexReScan function in IndexScan finds the correct
	 * expr to call ExecEval on it, except that this code is specialcased to allow exactly one
	 * index.
	 */
	assert(length(plan->indxqual) == 1);
	list_qual = lfirst(plan->indxqual);

	assert(length(list_qual) == 1);
	index_clause = lfirst(list_qual);

	rightexpr = (Node *) get_rightop(index_clause);
	leftexpr = (Node *) get_leftop(index_clause);
	assert(IsA(rightexpr, Var) &&IsA(leftexpr, Var));

	/** plan->indxqual seems to have guaranteed that the right side is the one that contains the outer.
	 *	So switch the variable numbers in this clause appropriately. */
	assert(((Var *) rightexpr)->varnoold != plan->scan.scanrelid || !printf("%d %d\n", ((Var *) rightexpr)->varnoold, plan->scan.scanrelid));
	/* ((Var *) rightexpr)->varno = ((Var *) rightexpr)->varnoold; */
	/* ((Var *) rightexpr)->varattno = ((Var *) rightexpr)->varoattno; */

	/** Finally, use the indexclause to set the sourceBuildOn and probeSource */
	ret->sourceBuiltOnID = plan->scan.scanrelid;		/* index into the range
														 * table */

	BitArrayClearAll(ret->sourceBuiltOn, TCQMAXSOURCES);
	BitArraySetBit(ret->sourceBuiltOn, ((Var *) leftexpr)->varnoold - 1);

	BitArrayClearAll(ret->probeSource, TCQMAXSOURCES);
	BitArraySetBit(ret->probeSource, ((Var *) rightexpr)->varnoold - 1);

	return ret;
}


/**
 *	Take a plan, and the rangetable for the query, and construct an Eddy using them.
 *
 *	With the new postgres structure for rangetables, we must first rearrange the
 *	rangetable so that all the base relations are in the beginning.
 */
Plan *
pg_convert_plan_to_eddy(Plan *plan, List *rangetable)
{
	Plan	   *ret = NULL;


	Assert(!readjusting_required(rangetable));

	/* For performing Eddy-conversion of event processing queries, 
	 * we observe that after coming out of the planner, they will be
	 * either a stream scan (if it was on one stream) or a nested-loop join
	 * (if it was on multiple relations, as there is no join predicate)	  
	 * 
	 * - TCQ SR
	*/

	if (IsA(plan, SeqScan) ||IsA(plan, StrmScan)) {
		ret = create_eddy_from_scan(plan); 


		if(plan->eventClause) { /* We are handling an event query, so we need to do more 
					   than just adding the StrmScan into the eddy's sources array */
		  FEventAgg *eventAgg = makeNode(FEventAgg);
		  eventAgg->eventExpr = ret->eventClause = plan->eventClause;
		  add_planitem_to_eddy((Eddy *)ret, (Plan *)eventAgg, EDDY_MODULE);
		  
		  elog(LOG, "Frontend: Converted a single-stream event query into an Eddy");
  
		}


		return (Plan *) ret;

	}

	if (IsA(plan, NestLoop)) {


	  if(plan->eventClause) {
	    List *l;
	    FEventAgg *eventAgg;
	    
	    /* The reason we are here is because we have seen an event query
	     * which has multiple streams in the FROM clause. The planner felt that 
	     * we are looking for a cross-product (as we have no join predicates)
	     * and hence generated a tree of NestLoops. 
	     * If we don't intercept the control here, the TCQ eddy conversion 
	     * routines will raise an ERROR for trying to do  a cross-product 
	     * between streams. 
	     * - TCQ SR 
	     */
	    
	    ret = (Plan *)makeNode(Eddy);
	    ((Eddy *)ret)->plan.targetlist = plan->targetlist;

	    foreach(l, ((Eddy *)ret)->plan.targetlist)
	      {
		use_old_varnos(lfirst(l));
	      }
	    
	    add_strmscans_into_eddy((Eddy *)ret, plan);
	    /* Now all sources have been taken care of. We only have the EventAggregator to put in */
	    
	    eventAgg = makeNode(FEventAgg);
	    eventAgg->eventExpr = ret->eventClause = plan->eventClause;
	    add_planitem_to_eddy((Eddy *)ret, (Plan *)eventAgg, EDDY_MODULE);

	    /* Not sure if I need to switch all Vars in the Eddy to their "old" varno/varattno's 
	    * The routines for eddy-conversion of normal plans seem to be doing this */

	    elog(LOG, "Frontend: Converted a multi-stream event query into an Eddy");

	  }
	  else
	    ret = convert_subplan_to_eddy(plan); /* The non-event query case */	


	  return (Plan *) ret;
	}


	/* If control comes here, the query should not be an event query...
	 * As no guy in the following code knows how to handle it! 
	 */
	
	Assert(!plan->eventClause);


	if (IsA(plan, HashJoin))
	{

		elog(DEBUG1, "frontend is converting a HashJoin to an eddy");
		ret = convert_subplan_to_eddy(plan);
		return (Plan *) ret;

	}
	if (IsA(plan, MergeJoin))
	{

		elog(DEBUG1, "frontend is converting a MergeJoin to an eddy");
		ret = convert_subplan_to_eddy(plan);
		return (Plan *) ret;

	}
    /* GROUP BY, ORDER BY, and LIMIT nodes get passed through unchanged.
     * When the converted plan tree is added to the eddy, these nodes will
     * become the per-window aggregation operator tree. */
    if (IsA(plan, Agg) || IsA(plan, Group) || IsA(plan, Sort) || IsA(plan, Limit))
    {
        /* Recursively convert everything below the node we're passing through
         */
        outerPlan(plan)  
			= pg_convert_plan_to_eddy(outerPlan(plan), rangetable);

        return plan;
    }
#if 0
	if (IsA(plan, Agg))
	{

		/* @dwndaggWH */
		/* there can be a Group node right below Agg */
		/* must skip the Group node too */
		Plan	   *p = plan;

		if (IsA(outerPlan(plan), Group))
		{
			p = outerPlan(plan);
			if (IsA(outerPlan(p), Sort))
				p = outerPlan(p);
		}

		/*
		 * If the outerplan is a hashjoin, we can call convert_subplan on
		 * it.
		 */
		ret = plan;
		if (IsA(outerPlan(p), HashJoin))
			outerPlan(p) = convert_subplan_to_eddy(outerPlan(p));
		else if (IsA(outerPlan(p), NestLoop))
			outerPlan(p) = convert_subplan_to_eddy(outerPlan(p));
		if (IsA_Scan(outerPlan(p)))
			outerPlan(p) = create_eddy_from_scan(outerPlan(p));

		return (Plan *) ret;
	}
	if (IsA(plan, Sort))
	{
		Plan	   *p = plan;

		ret = plan;

		if (IsA(outerPlan(p), HashJoin))
			outerPlan(p) = convert_subplan_to_eddy(outerPlan(p));
		else if (IsA(outerPlan(p), NestLoop))
			outerPlan(p) = convert_subplan_to_eddy(outerPlan(p));
		else if (IsA_Scan(outerPlan(p)))
			outerPlan(p) = create_eddy_from_scan(outerPlan(p));
		else if (IsA(outerPlan(p), Agg))		/* This seems to be
												 * happening for order by
												 * desc - Amol */
			outerPlan(p) = pg_convert_plan_to_eddy(outerPlan(p), rangetable);
		return (Plan *) ret;
	}
#endif /* 0 */

	if (IsA(plan, Eddy))
	{
		/** God has already created an Eddy. Who am I to argue with him ? */
		return plan;
	}
	
	nodeDisplay(plan);
	elog(ERROR, "convertoeddy2: Don't know how to handle this query plan %d", plan->type);
	
	return NULL; /* Keep compiler quiet */
}



/*
 * Given a plan, which is only a tree of NestLoops with StrmScans at the bottom,
 * pull out the scans and make them the sources of an Eddy. Makes sure that multiple scans
 * on the same stream are not munged into the eddy.
 *
 * A utility function for eddy-conversion of event queries, use it with REAL care if 
 * you have to, for other purposes.
 *
 *   - TCQ SR
 */
void
add_strmscans_into_eddy(Eddy *e, Plan *plan)
{
  
  if(IsA(plan, StrmScan)) {
    /* We have already spotted in convert_seqscan_to_strmscan() any presence of 
       mutliple scans on the same stream. Such StrScans have been "tainted" to have 
       rtindex = 0... We need to throw them out here. 
   
       - OLD!
    */
    /*
     * Find out if a similar scan has already been added
     */

    bool redundant = FALSE;
    int i;
    for(i = 0 ; i < e->numSources; i++) {
      StrmScan *thisscan = (StrmScan *)e->sources[i];
      if(thisscan->streamoid == ((StrmScan *)plan)->streamoid)
	redundant = TRUE;
    }
    if(redundant)
      elog(LOG, "Found a redundant stream scan. Not unexpected, as this its for an event query. Just dropping it");
    else
      munge_source_into_eddy(e, plan);
  }

  else if(!outerPlan(plan) || !innerPlan(plan)) 
    elog(ERROR, "add_strmscans_into_eddy: Unexpected plan as input, atleast one child missing, can't recurse down");
  else {
    
    add_strmscans_into_eddy(e, outerPlan(plan));
    add_strmscans_into_eddy(e, innerPlan(plan));

  }
}



/*
 * Given a plan node, traverse it and find all the sources and hash joins
 */
void
find_sources_and_hash_joins(Plan *plan, List **list_sources, List **list_hashjoins)
{

	if (IsA(plan, SeqScan) ||IsA(plan, StrmScan))
	{

		*list_sources = makeList1(plan);
		*list_hashjoins = NIL;

	}
	else if (IsA(plan, Hash))
	{

		find_sources_and_hash_joins(outerPlan(plan), list_sources, list_hashjoins);

	}
	else if (IsA(plan, HashJoin))
	{

		List	   *list_sources1,
				   *list_sources2;
		List	   *list_hashjoins1,
				   *list_hashjoins2;

		find_sources_and_hash_joins(outerPlan(plan), &list_sources1, &list_hashjoins1);
		find_sources_and_hash_joins(innerPlan(plan), &list_sources2, &list_hashjoins2);

		*list_sources = nconc(list_sources1, list_sources2);
		*list_hashjoins = nconc(list_hashjoins1, list_hashjoins2);

		*list_hashjoins = lappend(*list_hashjoins, plan);

	}
	else
	{
		nodeDisplay(plan);
		elog(ERROR, "convertoeddy3: Don't know how to handle this query plan %d", plan->type);
	}
}

/*
 *	Given a Node *, switch all teh INNER and OUTER in the Var's it contains.
 *	Note that, this relies on pull_var_clause returning pointers to actual
 *	Var's in the expression.
 */
/*
static Node *
switch_inner_and_outer(Node *node)
{
	List *allvars = pull_var_clause(node, false);
	List *iter;

	foreach (iter, allvars)
	{
		Var *v = (Var *) lfirst(iter);
		Assert(IsA(v, Var));
		v->varno = (v->varno == OUTER) ? INNER : OUTER;
	}

	freeList(allvars);

	return node;
}
*/

/*
 *	Given a Node *, switch all the varno's to varnoold's.
 */
static Node *
switch_varnos_and_attrs_to_old(Node *node)
{
	List	   *allvars = pull_var_clause(node, false);
	List	   *iter;

	foreach(iter, allvars)
	{
		Var		   *v = (Var *) lfirst(iter);

		Assert(IsA(v, Var));
		v->varno = v->varnoold;
		v->varattno = v->varoattno;
	}

	freeList(allvars);

	return node;
}

/** Check that all the Var's in the Node * refer to the same relation. Return
 *  that varno. */
int
check_uniqueness_and_get_varno(Node *node)
{
	List	   *allvars = pull_var_clause(node, false);
	List	   *iter;
	int			varno = -1;

    /* Assert( (length(allvars) == 1) || !"Not allowed to have more than one Var's in the hash key"); */

	foreach(iter, allvars)
	{
		Var		   *v = (Var *) lfirst(iter);

		Assert(IsA(v, Var));
		Assert((v->varno != INNER) || (v->varno != OUTER));
		if (varno == -1)
			varno = v->varno;
		else
		{
			if (varno != v->varno)
			{
				elog(ERROR, "Can't build a SteM on a hash key that refers to multiple relations.");
				Assert(varno == v->varnoold);
			}
		}
	}

	freeList(allvars);

	if (varno == -1)
		elog(ERROR, "Didn't find any Var's in the hash key");

	/*
	 * nodeDisplay(node); printf("The varno for this expression is %d\n",
	 * varno);
	 */

	return varno;
}

/** source_built_on is a rangetable index. */
static SteM *
create_stem(Node *innerhashkey, Node *outerhashkey, Expr *joinclause, List *otherjoinquals, int source_built_on)
{
	List	   *allvars,
			   *iter;
	SteM	   *ret = makeNode(SteM);

	ret->innerhashkey = (Node *) (copyObject(innerhashkey));
	ret->outerhashkey = (Node *) (copyObject(outerhashkey));
	ret->stemclauses = makeList1(copyObject(joinclause));
	BitArrayClearAll(ret->sourceBuiltOn, __MAX_RELATIONS__);
	BitArraySetBit(ret->sourceBuiltOn, source_built_on - 1);
	ret->joinquals = copyObject(otherjoinquals);
	ret->innerNode = 0;

	/** At this point, we need to make sure that the INNER and OUTERS are referred to correctly in
	 * the ret->stemclauses, and ret->joinquals. Basically, the INNER should have varnoold set
	 * to source_built_on as that will be the inner at that time.
	 * We will simply go over all the Var's in that to make sure this happens. */
	allvars = pull_var_clause((Node *) ret->stemclauses, false);
	foreach(iter, allvars)
	{
		Var		   *v = (Var *) lfirst(iter);

		Assert(IsA(v, Var));
		v->varno = (v->varnoold == source_built_on) ? INNER : OUTER;
		/** XXX: Shouldn't we be setting the varattno to old too ? */
	}
	allvars = pull_var_clause((Node *) ret->joinquals, false);
	foreach(iter, allvars)
	{
		Var		   *v = (Var *) lfirst(iter);

		Assert(IsA(v, Var));
		v->varno = (v->varnoold == source_built_on) ? INNER : OUTER;
		/** XXX: Shouldn't we be setting the varattno to old too ? */
	}

	/*** DEBUGGING INFORMATION
	printf("Creating SteM on source %d\n", source_built_on);
	printf("Innerhashkey\n");
	nodeDisplay(innerhashkey);
	printf("Outerhashkey\n");
	nodeDisplay(outerhashkey);
	printf("Clause\n");
	nodeDisplay(ret->stemclauses);
	********/

	return ret;
}

/** An attempt to modularize the functionality in previous 
 *  create_stems_from_hashjoin
 *	code so that it can be used for other join methods etc.
 *
 *	expr is the main join expression used for the join. It must be an equality.
 */
void
create_stems_from_eq_expr(Expr *expr, SteM ** stem_left_ptr, SteM ** stem_right_ptr)
{
	Expr	   *left_hash_key,
			   *right_hash_key;
	SteM	   *stem_left,
			   *stem_right;

	int			left_source_built_on,
				right_source_built_on;

	/***
	printf("HERE IT IS\n");
	nodeDisplay(expr);
	printf("HERE IT IS - 2\n");
	nodeDisplay(joinquals);
	***/

	if (!IsA(expr, Expr))
		elog(ERROR, "hashclause is not a T_Expr");

	/*
	 * Create flipped expr and joinquals to use in the second SteM.
	 *
	 * We won't use this any more. Instead, we will set the INNER and OUTER
	 * correctly based on whether the varnoold is the same as
	 * source_built_on. expr_flipped = copyObject(expr); joinquals_flipped
	 * = copyObject(joinquals);
	 *
	 * switch_inner_and_outer((Node *) expr_flipped);
	 * switch_inner_and_outer((Node *) joinquals_flipped);
	 */

	/*
	 * We will use the "old" attribute and relation numbers for evaluating
	 * the attribute values for hashing.
	 *
	 * We are going to clone the hash keys before switching the varno and
	 * varattno. Later, some of the expression evaluations still require
	 * inner and outer, and we will still be using above expressions for
	 * evaluating such expressions. So they need to remain in the
	 * inner/outer form.
	 *
	 */
	left_hash_key = copyObject(get_leftop(expr));
	right_hash_key = copyObject(get_rightop(expr));

	/** Is there any real point to this ? XXX */
	/** If we are coming from eddy_subplanner, then doing this is
	 * problematic. */
	if (!UseFastPathEddyCreate)
	{
		switch_varnos_and_attrs_to_old((Node *) left_hash_key);
		switch_varnos_and_attrs_to_old((Node *) right_hash_key);
	}

	/** Make sure that the left_hash_key and right_hash_key are in the correct
	 *	format. Currently, we require that each of these consist of exactly one
	 *	varno each. */
	left_source_built_on = check_uniqueness_and_get_varno((Node *) left_hash_key);
	right_source_built_on = check_uniqueness_and_get_varno((Node *) right_hash_key);

	/*
	 * One thing to notice here is how to match expr and expr_flipped to
	 * the SteMs.
	 *
	 * For a SteM, the inner is the relation on which we build. The original
	 * expr uses right as the inner and left as the outer.
	 */
	stem_left = create_stem((Node *) left_hash_key, (Node *) right_hash_key, expr, NULL, left_source_built_on);
	stem_right = create_stem((Node *) right_hash_key, (Node *) left_hash_key, expr, NULL, right_source_built_on);

    stem_left->innerhashkey_var = lfirst(pull_var_clause(stem_left->innerhashkey, false));
    stem_left->outerhashkey_var = lfirst(pull_var_clause(stem_left->outerhashkey, false));
    stem_right->innerhashkey_var = lfirst(pull_var_clause(stem_right->innerhashkey, false));
    stem_right->outerhashkey_var = lfirst(pull_var_clause(stem_right->outerhashkey, false));

	stem_left->dualSteM = stem_right;
	stem_right->dualSteM = stem_left;

	*stem_left_ptr = stem_left;
	*stem_right_ptr = stem_right;
}




void
create_stems_from_hashjoin(HashJoin *plan, SteM ** stem_left_ptr, SteM ** stem_right_ptr)
{
	printf("The length of hash clauses equals %d\n", length(plan->hashclauses));
	printf("The length of other clauses equals %d\n", length(plan->join.joinqual));

	Assert(length(plan->hashclauses) == 1);

	create_stems_from_eq_expr(lfirst(plan->hashclauses), stem_left_ptr, stem_right_ptr);
}

void
create_stems_from_mergejoin(MergeJoin *plan, SteM ** stem_left_ptr, SteM ** stem_right_ptr)
{
	printf("The length of merge clauses equals %d\n", length(plan->mergeclauses));
	printf("The length of other clauses equals %d\n", length(plan->join.joinqual));

	Assert(length(plan->mergeclauses) == 1);

	create_stems_from_eq_expr(lfirst(plan->mergeclauses), stem_left_ptr, stem_right_ptr);
}

bool is_simple_equality(Expr *expr);
 
/**
 * Returns TRUE if the indicated expression is a simple equality expression. 
 */
bool
is_simple_equality(Expr *expr) {
    int i = -1;
    Oper *op = NULL;

    /** OIDs of operators that represent equality.   
     * Terminated with InvalidOid.
     *
     * Generated by running: 
     *      select oid from pg_operator where opername = '=';
     *
     * TODO: Generate this list dynamically!
     */
    const Oid equality_oids[] = 
    { 15, 91, 92, 93, 94, 96, 98, 329, 349, 374, 375, 377, 378, 379, 380, 381,
        382, 383, 384, 385, 386, 390, 391, 392, 393, 394, 395, 396, 397, 398,
        400, 401, 352, 353, 387, 410, 416, 503, 532, 533, 560, 566, 607, 649,
        620, 670, 792, 811, 900, 1054, 1062, 1093, 1108, 1550, 1120, 1130,
        1320, 1330, 1500, 1535, 1616, 1220, 1201, 820, 1656, 1752, 1784, 1804,
        1862, 1868, 1955, 2060, 2222, 2223, 2224, 2225, 2226, 2227, 2228, 2229,
        2230, 2231, 2232, 2233, 2234, 2235, 2236, 2237, 2238, 2239, 2240, 2241,
        2242, 2243, 2244, 2245, InvalidOid };

    /* We're looking for an equality expression whose arguments are fields of
     * tuples. */

    if (OP_EXPR != expr->opType) {
        elog(LOG, "Got a non-EXPR node.");
        return false;
    }

    /* Check number of arguments. */
    if (2 != length(expr->args)) {
        return false;
    }

    /* TODO: Check types of arguments. */

    /* Check the operator type. */
    op = (Oper*)(expr->oper);
    for (i = 0; equality_oids[i] != InvalidOid; i++) {
        if (op->opno == equality_oids[i]) {
            return true;
        }
    }

    return false;
}

/** 
 * Creates SteM modules that will execute the indicated join; reuses existing
 * SteMs if possible.
 *
 * Attempts to find an equality predicate for the SteMs.  We currently don't do
 * non-equijoins.
 *
 * Removes the equality predicate from the joinquals list of the NestLoop
 * operator.  The caller is responsible for adding these quals to the eddy as
 * filters.
 */
void
create_stems_from_nestloopjoin(NestLoop *plan, SteM ** stem_left_ptr, SteM ** stem_right_ptr)
{

	Expr	    *eq_expr = NULL;
    List        *itr = NULL;

    elog(LOG, "Creating SteMs from a nested loop join node.");

    /* Find the equality expression in the join predicate, so that we can use
     * SteM hash tables to do the join. */
    foreach(itr, plan->join.joinqual) {
        Expr *cur = lfirst(itr);

        elog(LOG, "Checking Expr %p", cur);

        if (is_simple_equality(cur)) {
            elog(LOG, "Found equality expression.");
            eq_expr = cur;
            break;
        }
    }

    if (NULL == eq_expr) 
    {
	    elog(ERROR, "Sorry, need at least one equality predicate in each join");
    }

    /* If we get here, we've found the equality predicate (eq_expr).  Remove it
     * from the list of predicates. */
	plan->join.joinqual = lremove(eq_expr, plan->join.joinqual);

	create_stems_from_eq_expr(eq_expr, stem_left_ptr, stem_right_ptr);
}

static Plan *
convert_plan_to_stemhashjoins(Plan *plan)
{
	Plan	   *ret = NULL;

	if (IsA(plan, SeqScan) ||IsA(plan, StrmScan))
		ret = plan;
	else if (IsA(plan, HashJoin))
	{
		/* Change it to a SteM Hash Join */
		HashJoin   *hj = (HashJoin *) plan;
		SteMHashJoin *shj = makeNode(SteMHashJoin);
		Hash	   *hash = (Hash *) innerPlan(hj);
		Var		   *right_hash_key,
				   *left_hash_key;
		SteM	   *stem_right;
		Expr	   *expr = (Expr *) lfirst(hj->hashclauses);
		List	   *l;
        int			right_source_built_on;

		shj->join = hj->join;
		shj->join.plan.type = T_SteMHashJoin;
		shj->hashclauses = hj->hashclauses;
		shj->hashjoinop = hj->hashjoinop;
		shj->hashjoinstate = hj->hashjoinstate;

		shj->doProject = false; /* In most cases, we don't want to project */

		/*
		 * For each of the target list variables, we will replace varno
		 * and varattno with varnoold and varoattno. We should really
		 * figure out a more clean way to deal with this confusion.
		 */
		foreach(l, shj->join.plan.targetlist)
		{
			Var		   *var = (Var *) ((TargetEntry *) lfirst(l))->expr;

			if (!IsA(var, Var))
				elog(ERROR, "Only column references may appear in the select list for this query");

			var->varno = var->varnoold;
			var->varattno = var->varoattno;
		}

        /* Code copied from somewhere above where multiple stems are created... i think from 
         * create_stems_from_eq_expr */
        right_hash_key = copyObject(get_rightop(expr));
        left_hash_key = copyObject(get_leftop(expr));

        if (!UseFastPathEddyCreate)
        {
            switch_varnos_and_attrs_to_old((Node *) left_hash_key);
            switch_varnos_and_attrs_to_old((Node *) right_hash_key);
        }

        right_source_built_on = check_uniqueness_and_get_varno((Node *) right_hash_key);

        stem_right = create_stem((Node *) right_hash_key, (Node *) left_hash_key, expr, NIL, right_source_built_on);

        stem_right->innerhashkey_var = lfirst(pull_var_clause(stem_right->innerhashkey, false));
        stem_right->outerhashkey_var = lfirst(pull_var_clause(stem_right->outerhashkey, false));
        stem_right->innerNode = 0;

		/*
		 * Recurse on one side, and on the other side through Hash. Can't
		 * recurse on Hash itself, as the Hash node does not contain
		 * enough information to create the corresponding SteM node.
		 */
		outerPlan(shj) = convert_plan_to_stemhashjoins(outerPlan(shj));
		innerPlan(shj) = (Plan *) stem_right;
		stem_right->innerNode = convert_plan_to_stemhashjoins(outerPlan(hash));

		ret = (Plan *) shj;
	}
	else
	{
		nodeDisplay(plan);
		elog(ERROR, "convertoeddy4: Don't know how to handle this query plan %d", plan->type);
	}

	return ret;
}

Plan *
pg_convert_plan_to_stemhashjoins(Plan *plan)
{
	Plan	   *ret = convert_plan_to_stemhashjoins(plan);

	assert(IsA(ret, SteMHashJoin));

	/** Top level SteMHashJoin must project */
	((SteMHashJoin *) ret)->doProject = true;

	return ret;
}


/** Check if readjusting is required */
bool
readjusting_required(List *rtable)
{
	bool		rearranging_required = false;
	List	   *iter;

	/*
	 * If at any time, we have a Entry that is RTE_RELATION that is
	 * preceded by a non RTE_RELATION, we need to rearrange.
	 */
	iter = rtable;
	while ((iter != NULL) && (lnext(iter) != NULL))
	{
		if (((RangeTblEntry *) lsecond(iter))->rtekind == RTE_RELATION)
		{
			if (((RangeTblEntry *) lfirst(iter))->rtekind != RTE_RELATION)
			{
				rearranging_required = true;
				break;
			}
		}
		iter = lnext(iter);
	}
	return rearranging_required;
}

/** Checks if a rearranging is required (the return value), and if yes, creates
 *	a new rangetable and sets the new_indexes appropriately. */
static bool
readjust_range_table(List *old_rtable, List **new_rtable_ptr, int *new_indexes)
{
	int			index1,
				index2;
	List	   *iter;

	/** We might just get lucky. */
	if (!readjusting_required(old_rtable))
		return false;

	/** oh well ! */
	*new_rtable_ptr = NIL;
	index1 = 0;
	index2 = 0;
	foreach(iter, old_rtable)
	{
		if (((RangeTblEntry *) lfirst(iter))->rtekind == RTE_RELATION)
		{
			*new_rtable_ptr = lappend(*new_rtable_ptr, lfirst(iter));
			new_indexes[index1] = index2;
			index2++;
		}

		index1++;
	}

	index1 = 0;
	foreach(iter, old_rtable)
	{
		if (((RangeTblEntry *) lfirst(iter))->rtekind != RTE_RELATION)
		{
			*new_rtable_ptr = lappend(*new_rtable_ptr, lfirst(iter));
			new_indexes[index1] = index2;
			index2++;
		}

		index1++;
	}
	return true;
}

/** Readjust query tree */
void
readjust_query_rangetable(Query *querytree)
{
	int			num_rtable_entries = length(querytree->rtable);
	List	   *new_rangetable;
	int			newindexes[num_rtable_entries];

	printf("The number of entries here is %d\n", num_rtable_entries);

	if (readjust_range_table(querytree->rtable, &new_rangetable, newindexes))
	{
		int			i;

		querytree->rtable = new_rangetable;

		for (i = 0; i < num_rtable_entries; i++)
			printf("%d -> %d\n", i, newindexes[i]);

		/** Do the changes. */
		for (i = 0; i < num_rtable_entries; i++)
		{
			if (i != newindexes[i])
			{
				int			tmp_index = newindexes[i] + num_rtable_entries;

				ChangeVarNodes((Node *) querytree, i + 1, tmp_index + 1, 0);
			}
		}

		for (i = 0; i < num_rtable_entries; i++)
		{
			if (i != newindexes[i])
			{
				int			tmp_index = newindexes[i] + num_rtable_entries;

				ChangeVarNodes((Node *) querytree, tmp_index + 1, newindexes[i] + 1, 0);
			}
		}
	}
}

IndexSteM *
strmscan_to_indexstem(StrmScan * scan)
{
	bool		allconst = true;
	IndexSteM  *istem = NULL;


	allconst = !strmscan_has_var_bindings(scan);

	if (allconst == false)
	{
		istem = makeNode(IndexSteM);
		istem->join.jointype = JOIN_INNER;
		istem->join.joinqual = NIL;
		istem->sourceBuiltOnID = sourceGetVarNo((Plan *) scan);
		innerPlan(&(istem->join.plan)) = (Plan *) scan;
		return istem;
	}
	return NULL;
}

void join_quals_to_filter(Eddy * eddy, List * joinquals)
{
  List * iter = NIL;
  foreach(iter, joinquals)
    {
      Node * n = (Node*)lfirst(iter);
      Filter * f = makeFilter(n);
      use_old_varnos(f->filterqual);
      elog(NOTICE, "converting joinqual to a filter: %s", nodeToString(f));
      add_planitem_to_eddy(eddy, (Plan *) f, EDDY_MODULE);
    }
}

void use_old_varnos(Node * n)
{
  List	   *vars = pull_var_clause(n, false);
  List     *iter = NIL;
  Var      *var=NULL;
  foreach(iter, vars)
    {
      var = lfirst(iter);
      var->varno = var->varnoold;
      var->varattno = var->varoattno;
    }
  freeList(vars);
  vars = NIL;
}

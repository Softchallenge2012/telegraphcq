/*
 * convertohashgroup.c
 *
 *	  Routines to convert sorted group-by to hash-based group-by
 *
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/converttohashgroup.c,v 1.14 2004/03/05 21:10:16 mashah Exp $
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

#include "pgstat.h"

Plan	   *convert_to_hash_group(Plan *plan, Query *querytree, Sort *sort);
Plan	   *pg_convert_to_hash_group(Plan *plan, Query *querytree);
Plan       *pg_convert_all_to_hash_group(Plan *plan, Query *querytree);
void		adjust_varnos(List *tl);

void       remove_seqnums_from_group_cols(Plan *plan);

/*
 * Stolen from nodeSort.c, but since I do not do projections,
 * I use varnos instead.
 */

static void
ExtractSortKeys(Sort *sortnode,
				int *p_nkeys,
				AttrNumber **attNums,
				bool dontProject)
{
	List	   *targetList;
	int			keycount;
	AttrNumber *attNos;
	List	   *tl;

	/*
	 * get information from the node
	 */
	targetList = sortnode->plan.targetlist;
	keycount = sortnode->keycount;
	*p_nkeys = keycount;

	/*
	 * first allocate space for results
	 */
	if (keycount <= 0)
		elog(ERROR, "ExtractSortKeys: keycount <= 0");
	attNos = (AttrNumber *) palloc(keycount * sizeof(AttrNumber));
	MemSet(attNos, 0, keycount * sizeof(AttrNumber));
	*attNums = attNos;

	/*
	 * extract info from the resdom nodes in the target list
	 */
	foreach(tl, targetList)
	{
		TargetEntry *target = (TargetEntry *) lfirst(tl);
		Resdom	   *resdom = target->resdom;
		Index		reskey = resdom->reskey;
		Var		   *var = (Var *) target->expr;

		if (!IsA(var, Var))
			elog(ERROR, "Cannot deal with non-var expressions");

		if (reskey > 0)			/* ignore TLEs that are not sort keys */
		{
			Assert(reskey <= keycount);

			/*
			 * We want the old attribute number, if no projection was
			 * performed
			 */
			attNos[reskey - 1] =
/* 				(dontProject) ? (var->varoattno) : (var->varattno); */
				(dontProject) ? (var->varoattno) : (resdom->resno);
		}
	}


}

/* XXX In future, provide two arguments, one from below and one new, and
   using the varnoold's perform the mapping */
void
adjust_varnos(List *tl)
{

	List	   *l;

	/*
	 * For each of the target list variables, we will replace varno and
	 * varattno with varnoold and varoattno. We should really figure out a
	 * more clean way to deal with this confusion.
	 */
	foreach(l, tl)
	{
		Var		   *var = (Var *) ((TargetEntry *) lfirst(l))->expr;

		if (IsA(var, Var))
		{
			var->varno = var->varnoold;
			var->varattno = var->varoattno;
		}
		else
			elog(DEBUG1, "Found non-column reference");
	}

}

#define FUDGE 1.5				/* Fudge factor for number of buckets */

Plan *
convert_to_hash_group(Plan *plan, Query *querytree, Sort *sort)
{

	HashGroup  *hashgroup;

	if (sort == NULL)
		return NULL;

	hashgroup = makeNode(HashGroup);

	/* We made sure it was an Agg in pg_convert_to_hash_group. */

	/*
	 * We are copying the plan information. ExecInitHashAgg should do the
	 * rest.
	 */
	hashgroup->agg.plan = *plan;
	hashgroup->agg.plan.type = T_HashGroup;		/* Set tag to HashGroup */
	hashgroup->agg.plan.pushPlan = fdataflow->pushPlan; /* Is it a push? */

	if (!IsA(outerPlan(sort), StrmScan) || 
		(((StrmScan *) outerPlan(sort))->seqscan->dontProject))
	{
		/*
		 * Replace the varnos with the varnos from the non-projected tuples.
		 * Its because we avoid projections. If we were projecting in between,
		 * we would have to change this.
		 */
		adjust_varnos(hashgroup->agg.plan.targetlist);

		/*
		 * Set the grouping columns properly, they must come from the sort's
		 * targetlist
		 */

		/*
		 * Note, the last "true" indicates we are using the old varnos, and
		 * not the projected ones. If there is a projection before the sort,
		 * we should pass false.
		 */
		ExtractSortKeys(sort, &(hashgroup->agg.numCols),
						&(hashgroup->agg.grpColIdx), true);
	}
	else
	{
		/* See comment above */
		ExtractSortKeys(sort, &(hashgroup->agg.numCols),
						&(hashgroup->agg.grpColIdx), false);
	}
	
	/*
	 * This must happen after the assignment of the group otherwise the
	 * group assignment clobbers the lefttree value
	 */
	outerPlan(hashgroup) = outerPlan(sort);
	hashgroup->nparts = 10000;	/* Whatever, number of parts is not used */

	hashgroup->numaggs = 1;
	hashgroup->wndSpecs = palloc(sizeof(AggWindowSpec) * (hashgroup->numaggs));

	if (querytree->aggWinClause)
		hashgroup->wndSpecs[0] = *querytree->aggWinClause;
	else
	{
		elog(DEBUG1, "Setting window to tumble 1000");
		hashgroup->wndSpecs[0].window_interval = 1000;
		hashgroup->wndSpecs[0].hop_size = /* 1000 */ 2;
	}

	hashgroup->agg.aggstrategy = AGG_HASHED_WINDOW;

	/* Estimate of number of groups */
	hashgroup->agg.numGroups = (int) ((float) NUM_HASH_GROUPS * (FUDGE));

	return (Plan *) hashgroup;
}

/* Coverts the first sequence of Agg-Group-Sort into HashGroup */
Plan *
pg_convert_to_hash_group(Plan *plan, Query *querytree)
{
	Sort	   *asort;

	elog(DEBUG1, "Converting to hash group-bys");

	if ((plan != NULL) &&
		IsA(plan, Agg))
	{							/* First level must be agg */
		if ((outerPlan(plan) != NULL) &&
			IsA(outerPlan(plan), Group))
		{						/* Second must be group */

			asort = (Sort *) outerPlan(outerPlan(plan));
			if ((asort != NULL) &&
				IsA(asort, Sort))
			{					/* Last sort */

				return convert_to_hash_group(plan, querytree, asort);
			}
		}
	}

	return plan;
}

/* Converts all sequences of Agg-Group-Sorts into HashGroup */

Plan *
pg_convert_all_to_hash_group(Plan *plan, Query *querytree)
{
	Plan *result;
	
	if (IsA(plan, Agg)) 
	{
		plan = pg_convert_to_hash_group(plan, querytree);
			
		if (IsA(plan, HashGroup))
		{
			result = pg_convert_all_to_hash_group(outerPlan(plan), querytree);
			outerPlan(plan) = result;
		}
	}
	else
	{
		if (IsA(plan, SubqueryScan))
		{
			return pg_convert_all_to_hash_group(((SubqueryScan *) plan)->subplan, querytree);
		}
	}
	
	return plan;
}

static void
drop_first_k_group_col(HashGroup *grp, int k)
{
	int i;

	for(i = 0; i < grp->agg.numCols - k; i++)
	{
		grp->agg.grpColIdx[i] = grp->agg.grpColIdx[i+k];
	}

	grp->agg.numCols-=k;
}

/* The seqnums are added in the agg columns because they are needed
   to create the proper target lists. */

void 
remove_seqnums_from_group_cols(Plan *plan)
{
	if (!plan) return;
		
	/* Just walk down the lefttree */
	if (IsA(plan, HashGroup)) 
	{
		drop_first_k_group_col((HashGroup *) plan, 1);
	}

	remove_seqnums_from_group_cols(outerPlan(plan));
}

void
remove_seqnums_ts_from_group_cols(Plan *plan)
{
	if (!plan) return;
		
	/* Just walk down the lefttree */
	if (IsA(plan, HashGroup)) 
	{
		drop_first_k_group_col((HashGroup *) plan, 3);
	}

	remove_seqnums_ts_from_group_cols(outerPlan(plan));
}


/* Traverses down the plan structure connecting up the parents */
void
connect_parents(Plan *plan)
{
	if (!plan) return;

	/* parent will set this if one exists. */
	plan->parent = NULL;
	
	if (outerPlan(plan)) 
	{
		connect_parents(outerPlan(plan));
		(outerPlan(plan))->parent = plan;
	}

	if (innerPlan(plan))
	{
		connect_parents(innerPlan(plan));
		(innerPlan(plan))->parent = plan;
	}

	return;
}

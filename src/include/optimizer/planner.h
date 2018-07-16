/*-------------------------------------------------------------------------
 *
 * planner.h
 *	  prototypes for planner.c.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: planner.h,v 1.18 2004/07/30 20:54:37 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PLANNER_H
#define PLANNER_H

#include "nodes/parsenodes.h"
#include "nodes/plannodes.h"


extern Plan *planner(Query *parse);
extern Plan *subquery_planner(Query *parse, double tuple_fraction);

extern Plan *make_sortplan(Query *parse, List *tlist,
			  Plan *plannode, List *sortcls);

extern Plan *pg_convert_plan_to_eddy(Plan *plan, List *rangetable);		/* @dstemsSK */
Plan	   *convert_to_sym_hash_join(Plan *plan, Query *querytree);
Plan	   *pg_convert_to_sym_hash_join(Plan *plan, Query *querytree);
extern Plan *
pg_convert_seqscan_to_strmscan(Plan *plan,
							   Query *querytree,
							   bool blocking);	/* @dtcqpmMS */

extern Plan *
pg_convert_to_hash_group(Plan *plan,
						 Query *querytree);		/* @dfluxMS */

extern Plan *
pg_convert_all_to_hash_group(Plan *plan,
							 Query *querytree); /* @dfluxMS */

extern Plan *
pg_convert_to_fsched(Plan *plan,
					 Query *querytree); /* @dfluxMS */
extern void 
remove_seqnums_from_group_cols(Plan *plan); /* @dfluxMS */

extern void 
remove_seqnums_ts_from_group_cols(Plan *plan); /* @dfluxMS */

extern Plan *
pg_convert_to_fsched_dup(Plan *plan,
						 Query *querytree);		/* @dfluxMS */

extern void
connect_parents(Plan *plan); /* @dfluxMS */

extern Plan *pg_convert_plan_to_stemhashjoins(Plan *plan);
extern void readjust_query_rangetable(Query *querytree);

#endif   /* PLANNER_H */

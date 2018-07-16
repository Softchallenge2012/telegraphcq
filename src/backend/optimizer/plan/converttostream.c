/*-------------------------------------------------------------------------
 *
 * convertostrm.c
 *
 *	  Routines to convert scanners to streamers
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/optimizer/plan/converttostream.c,v 1.23 2005/12/14 20:49:23 phred Exp $
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
#include "nodes/nodes.h"
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

#include "optimizer/bindings.h"
    /* For load_bindings_for_stream() */

#include "pgstat.h"

Plan	   *subplan_seqscan_to_strmscan(Plan *plan, Query *querytree, bool blocking);
Plan	   *pg_convert_seqscan_to_strmscan(Plan *plan, Query *querytree, bool blocking);
Plan	   *convert_seqscan_to_strmscan(SeqScan *scan, Query *querytree, bool blocking);

/* If the scan is over a streamer, then it converts is to
   a stream scan, otherwise it returns the original object */

Plan *
convert_seqscan_to_strmscan(SeqScan *scan, Query *querytree, bool blocking)
{

	StrmScan   *sscan;

	/* It this is over a streamer relation */

	/*
	 * We need to do a lookup in the catalogs to get the relation tuple
	 * associated with the relation, since the scannners have yet to be
	 * initialized.
	 */

	Index		relid;
	List	   *rangeTable;
	RangeTblEntry *rtentry;
	Oid			reloid;
	Relation	reldesc;
	Plan	   *ret;

	/*
	 * We need the relation oid to access the relation tuple that is
	 * obtained from the range table entry
	 */

	relid = scan->scanrelid;
	rangeTable = querytree->rtable;
	rtentry = rt_fetch(relid, rangeTable);
	reloid = rtentry->relid;

	reldesc = RelationIdGetRelation(reloid);

	if (IsAStream(reldesc->rd_rel->relkind))
	{

		sscan = makeNode(StrmScan);
		sscan->seqscan = scan;

		/* @BfluxMS */
		/* dontProject should be false, if unarchived, I want it to be true. */
		if (IsUnarchivedStream(reldesc->rd_rel->relkind))
		{
			sscan->seqscan->dontProject = true;
		}
		/* @EdluxMS */
		
		sscan->streamoid = reloid;
		sscan->rtindex = scan->scanrelid;
	        sscan->scanstate = (CommonScanState*)copyObject(scan->scanstate);
		(sscan->scansock).plan = scan->plan;		/* Copy this stuff */
		(sscan->scansock).plan.type = T_StrmScan;
		(sscan->scansock).tupDesc =
			CreateTupleDescCopy(reldesc->rd_att);		/* Tuple descriptor */
		/* @dfluxMS, XXX please */
		/* do not remove.		*/

		sscan->blocking = blocking;
		sscan->streamtype = reldesc->rd_rel->relkind;
		elog(DEBUG1, "Converted scan over %s to stream",
			 reldesc->rd_rel->relname.data);

		load_bindings_for_stream(reloid, sscan);


		/* Set it to be a continuous query since isContinuousQuery
		   fails to recognize subqueries and view that use streams */
		
		querytree->isContinuousQuery = true; 		/* @dfluxMS */

		ret = (Plan *) sscan;	/* Changed it to a streamer */
	}
	else 
	{
		ret = (Plan *) scan;	/* Returning the same scan */
	}
	RelationClose(reldesc);		/* Decrement the refcount in the relcache
								 * ! */
	return ret;
}

Plan *
subplan_seqscan_to_strmscan(Plan *plan, Query *querytree, bool blocking)
{
	if (plan == NULL)
	{							/* Got a null plan */
		return plan;
	}
	if (IsA(plan, SeqScan))
		return convert_seqscan_to_strmscan((SeqScan *) plan, querytree, blocking);
	else
	{

		if (IsA(plan, SubqueryScan))
		{
			/* Recurse on the subplan */
			SubqueryScan *subs = (SubqueryScan *) plan;
			List *rangeTable;
			RangeTblEntry *rte;
				
			rangeTable = querytree->rtable;
			rte = rt_fetch(subs->scan.scanrelid, rangeTable);

			if (rte->rtekind != RTE_SUBQUERY)
			{
				elog(DEBUG1, "Found a subquery but wrong RTE.");
				return plan;
			}
			
			
			subs->subplan = subplan_seqscan_to_strmscan(subs->subplan,
														rte->subquery, blocking);

			/* Set it to be a continuous query since isContinuousQuery
			   fails to recognize subqueries and view that use streams */

			querytree->isContinuousQuery = (rte->subquery->isContinuousQuery);
		}
		else
		{
			/* Recurse and do the outer plan */
			outerPlan(plan) = subplan_seqscan_to_strmscan(outerPlan(plan),
														  querytree, blocking);
			innerPlan(plan) = subplan_seqscan_to_strmscan(innerPlan(plan),
														  querytree, blocking);
		}
		
		return plan;
	}

}

/*
 *
 * blocking indicates wether all the streamers will block on the queue or
 * not. If they block, then basically the steamer will spin for a while and
 * then sleep.
 *
 */

Plan *
pg_convert_seqscan_to_strmscan(Plan *plan, Query *querytree, bool blocking)
{
	elog(DEBUG1, "Converting seqscans to streamscans");
	return subplan_seqscan_to_strmscan(plan, querytree, blocking);
}

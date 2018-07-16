/*-------------------------------------------------------------------------
 *
 * nodeFluxEgress.c
 *	  The consumer side of the Flux operator.
 *	  Currently only has exchange functionality.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Window
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * DESCRIPTION
 *	  You know, the usual.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxEgress.c,v 1.2 2004/07/15 20:39:54 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeFluxCons.h"
#include "executor/nodeFluxCons_FT.h"
#include "nodes/print.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"
#include "rqueue/resultq.h"
#include "rqueue/rqdest.h"
#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>				/* These are included for the networking */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/poll.h>

#include "executor/instrument.h"
#include "executor/execState.h"
#include "storage/buffile.h"

#include "executor/nodeFluxEgress.h"
#include "executor/nodeFSched.h"
#include "telegraphcq/fluxroutines.h"

#include "executor/nodeFluxProd_FT.h"

TupleTableSlot *
ExecFluxEgress(FluxEgress * node)
{
	TupleTableSlot *outer;
	struct timeval st_time, curr_time, dur_time;
	bool isNull;
	Instrumentation *instr = node->fcons.instrument;
	
	if (!node->use_ts)
	{
		return ExecFluxCons_FT((FluxCons_FT *) node);
	}

	outer = ExecFluxCons_FT((FluxCons_FT *) node);

	if (TupIsNull(outer)) return NULL;
	
	st_time.tv_sec = heap_getattr(outer->val, node->ts_sec_col,
								  outer->ttc_tupleDescriptor,
								  &isNull);

	if (st_time.tv_sec != 0)
	{
		st_time.tv_usec = heap_getattr(outer->val, node->ts_usec_col,
									   outer->ttc_tupleDescriptor,
									   &isNull);

		gettimeofday(&curr_time, NULL);
		timersub(&curr_time, &st_time, &dur_time);
		
		instr->total_resp_time += TV_IN_MSEC_DOUBLE(dur_time);
		instr->resp_tuples++;
	}

	return outer;
}

bool
ExecInitFluxEgress(FluxEgress * node, EState *estate, Plan *parent)
{
	bool rval;
	
	rval = ExecInitFluxCons_FT((FluxCons_FT *) node, estate, parent);

	node->fcons.instrument->total_resp_time = 0;
	node->fcons.instrument->resp_tuples = 0;

	return rval;
}

int
ExecCountSlotsFluxEgress(FluxEgress * node)
{
	return ExecCountSlotsFluxCons_FT((FluxCons_FT *) node);
}

void
ExecEndFluxEgress(FluxEgress * node)
{
	return ExecEndFluxCons_FT((FluxCons_FT *) node);
}

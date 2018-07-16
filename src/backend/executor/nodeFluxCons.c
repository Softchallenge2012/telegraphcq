/*-------------------------------------------------------------------------
 *
 * nodeFluxCons.c
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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxCons.c,v 1.11 2004/07/15 20:39:54 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeFluxCons.h"
#include "nodes/print.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"
#include "rqueue/resultq.h"
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

#include "rqueue/rqdest.h"		/* result */
#include "telegraphcq/fluxroutines.h"
#include "executor/nodeSegCons.h" /* getResultFromConn */

static bool getNextTuple(FluxCons * node, int conn_id, TupleTableSlot *rslot);

/* Returns the next tuple from the socket */
static bool
getNextTuple(FluxCons * node, int conn_idx, TupleTableSlot *rslot)
{
	result 				 res;
	FluxConnInfo 		*fcinfo = &(node->fcstate->conn_info[conn_idx]);

	if (!getResultFromConn((Plan *) node, fcinfo, &res, conn_idx))
	{
		return false;
	}

	/* We have a tuple to return */
	ExecStoreTuple(fcinfo->p_htup, rslot, InvalidBuffer, true);
	fcinfo->p_htup = NULL;

	return true;
}

void
ExecFluxConsAddConn(FluxCons * node,
					int sockfd, struct sockaddr_in *addr,
					int node_id, int fprod_idx)
{
	int					 cidx;
	FluxConnInfo 	    *fcinfo;
	FluxConsState 		*lfcstate;

	cidx = index_of_mach(node->map_c2m, node->nProd, node_id);

	if (cidx == node->nProd)
	{
		elog(ERROR, "This FluxCons does not know about machine %d",
			 node_id);
	}

	lfcstate = node->fcstate;

	lfcstate->num_conn++;

	elog(DEBUG1, "Adding remote forward queue. From %d, from_idx %d",
		 node_id, fprod_idx);

	fcinfo = &(lfcstate->conn_info)[cidx];

	if (node_id != node->node_id) /* remote connection */
	{
		/*
		  init_conn_info(fcinfo, sockfd,
			   		     (Queue *) NULL, flux_queue_size,
					     (Queue *) NULL, 0);
		*/

		fcinfo->trans = newSocketTrans(sockfd, addr);
	}
	else
	{
		/*
		  init_conn_info(fcinfo, (-1),
					     node->local_q, 0,
					     (Queue *) NULL, 0);
		*/

		fcinfo->trans = newLocalTrans_q(node->local_q, (Queue *) NULL);
	}

	node->all_done = (lfcstate->num_conn == node->nProd);
}

/* ---------------------------------------
 *	 ExecFluxCons
 *
 *		Does the consuming from the sockets.
 * ------------------------------------------
 */

TupleTableSlot *
ExecFluxCons(FluxCons * node)
{

	FluxConsState *lfcstate;
	TupleTableSlot *slot;
	int			i;
	bool		rvalid;

	/* Wait until all connections are made before continuing */
	if (!node->all_done)
	{
		/* checkNewConn(node); */	/* FluxAcceptor does it now */
		return NULL;			/* Hope the other operators obey */
	}

	lfcstate = node->fcstate;
	slot = lfcstate->csstate.cstate.cs_ResultTupleSlot;

	ExecClearTuple(slot);		/* Make sure we have no tuple in the
								 * result slot */

	i = 0;
	rvalid = false;

	while (!rvalid && (i < node->nProd))
	{
		rvalid = getNextTuple(node, lfcstate->curr_conn, slot);
		lfcstate->curr_conn = (lfcstate->curr_conn + 1) % node->nProd;
		i++;
	}

	return slot;
}

/* -----------------
 * ExecInitFluxCons
 *
 * -----------------
 */
bool
ExecInitFluxCons(FluxCons * node, EState *estate, Plan *parent)
{
/*	bool rbool; */
	int			i;
	FluxConsState *lfcstate;

/*	node->sched = (FSched *) parent; */

	node->plan.state = estate;

	lfcstate = makeNode(FluxConsState);

	node->fcstate = lfcstate;

#define FLUXCONS_NSLOTS 1		/* Just need one slot for the tuple */

	/*
	 * tuple table initialization
	 */
	/* ExecInitScanTupleSlot(estate, &(lfcstate->csstate)); */
	ExecInitResultTupleSlot(estate, &(lfcstate->csstate.cstate));

	/*
	 * There is no outerplan, so remember to assign the scan type properly
	 * because the node above might need it later.
	 */
/*		/\* */
/*	 * initialize tuple type. */
/*	 *\/ */
/*	ExecAssignScanTypeFromOuterPlan((Plan *) node, node->scanstate); */

	/*
	 * Initialize tuple type for both result and scan. This node does no
	 * projection
	 */

	ExecAssignResultType(&(lfcstate->csstate.cstate), node->tupDesc, true);
/* 	ExecAssignResultTypeFromTL(node, &(lfcstate->csstate.cstate)); */
/* 	node->tupDesc = lfcstate->csstate.cstate->cs_ResultTupleSlot; */

	/*
	 * ExecAssignResultTypeFromTL((Plan *) node,
	 * &(lfcstate->csstate.cstate));
	 */

	/*
	 * ExecAssignProjectionInfo((Plan *) node,
	 * &(lfcstate->csstate.cstate));
	 */

	/* Initialize the port for accepting parallel connections */
/*	rbool = initAcceptFd(node); */

/*	if (!rbool) return FALSE; /\* Port initialization did not succeed *\/ */

	/* nProd should also be set by now. */

	/* Initialize the arry */
	lfcstate->conn_info = (FluxConnInfo *) palloc(sizeof(FluxConnInfo) * node->nProd);

	lfcstate->num_conn = 0;
	lfcstate->curr_conn = 0;

	for (i = 0; i < node->nProd; i++)
	{
		fconn_init_empty(&(lfcstate->conn_info[i]));
		lfcstate->conn_info[i].conn_idx = i;
		lfcstate->conn_info[i].role     = F_PRIMARY;
	}

	return TRUE;
}

int
ExecCountSlotsFluxCons(FluxCons * node)
{
	return FLUXCONS_NSLOTS;
}

/* ------------------------
 *		ExecEndFluxCons(node)
 *
 * -----------------------
 */
void
ExecEndFluxCons(FluxCons * node)
{
	int					 i;
	FluxConsState 		*lfcstate = node->fcstate;
	TupleTableSlot      *slot;
	
	/*
	 * Don't really need to do this since its all in the query context as
	 * it is. Isn't palloc and pfree beautiful?
	 */

	if (IsPushPlan(node))
	{
		ExecEndNode(parentPlan(node), (Plan *) node);
	}
	
	slot = node->fcstate->csstate.cstate.cs_ResultTupleSlot;

	ExecClearTuple(slot);

	for (i = 0; i < node->nProd; i++)
	{
		ftrans_reset(lfcstate->conn_info[i].trans);
	}
	
	pfree(lfcstate->conn_info);
}

/*-------------------------------------------------------------------------
 *
 * nodeSegProd
 *	  The operator feeding output to the egress.
 *
 * Copyright 2003, 2004 Mehul A. Shah and U.C. Regents.
 *
 *
 * DESCRIPTION
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeSegProd.c,v 1.6 2004/07/15 20:39:55 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeFluxProd.h"
#include "executor/nodeFluxProd_FT.h"
#include "parser/parse_oper.h"
#include "parser/parse_type.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"
#include "catalog/pg_type.h"
#include <sys/time.h>
#include <sys/types.h>

#include <stdio.h>				/* These are included for the networking */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <errno.h>

#include "executor/instrument.h"
#include "executor/nodeFluxAcceptor.h"	/* I need
										 * ExecInsertConnectionFluxAcceptor */
#include "executor/nodeSegProd.h"
#include "executor/nodeFSched.h"
#include "rqueue/rqdest.h"
#include "telegraphcq/fluxroutines.h"

/*
 * ----------------
 *	ExecSegProd
 * ----------------
 */
TupleTableSlot *
ExecSegProd(SegProd * snode)
{
	return ExecFluxProd_FT((FluxProd_FT *) snode);
}

#define IsPrimary(node) (((FluxProd_FT *) node)->m_role == F_PRIMARY)

#define EGRESSCONN 0

void
init_conn_stat_SegProd(SegProd * node)
{
	FluxProdState_FT *lfpstate = node->fprod.fpstate_ft;

	if (IsPrimary(node))
	{
		lfpstate->conn_info_p[EGRESSCONN].status = CONN_SEND_DATA;
	}
	else
	{
		lfpstate->conn_info_s[EGRESSCONN].status = CONN_ACK_DATA;
	}
}

static void
seg_prod_route_init(FluxProd * sp)
{
	/* Nothing to save between invocations */
}

static int
seg_prod_route(FluxProd *sp, TupleTableSlot *slot)
{
	return EGRESSCONN;
}


static bool
partner_failed(SegProd *snode)
{
	FluxProd_FT *node = (FluxProd_FT *) snode;
	
	if ((node->m_role == F_PRIMARY) &&
		(ROTATE_R(tnodeid_2_vmach(node->fprod.node_id), node->fprod.sched->ddeclust) == which_failed))
	{
		return true;
	}
	
	if ((node->m_role == F_SECONDARY) &&
		(ROTATE_L(tnodeid_2_vmach(node->fprod.node_id), node->fprod.sched->ddeclust) == which_failed))
	{
		return true;
	}
	
	return false;
}

void
do_take_over_SegProd(SegProd *snode)
{
	FluxProd_FT *node = (FluxProd_FT *) snode;
	
	/* Partner did not fail, no sync will be coming. */
	if (!partner_failed(snode))
	{
		node->mode = FLUX_NORMAL;
		set_recovered_fluxen(((FluxProd *) node)->sched,
							 ((FluxProd *) node)->flux_idx);
	}

	/* Otherwise wait for the prod_sync to set it to normal */
	return;
}

void
do_prod_sync_SegProd(SegProd *snode)
{
	FluxProd_FT 		*node 	 = (FluxProd_FT *) snode;
	FluxConnInfo   		*fcinfo;
	FluxProdState_FT  	*fpstate = node->fpstate_ft;

	elog(DEBUG1, "%d: SegProd: got and sending sync", node->m_role);
	
	if (IsPrimary(node))
	{
		fcinfo = &(fpstate->conn_info_p[EGRESSCONN]);
		fcinfo->next_state = CONN_SEND_DATA; /* Egress doesn't fail */
	}
	else
	{
		fcinfo = &(fpstate->conn_info_s[EGRESSCONN]);
		fcinfo->next_state = CONN_ACK_DATA;
	}

	fcinfo->res.data.info = (-1);
	
	fcinfo->res.type = RESULT_FPRODSYNC;
	fcinfo->status = CONN_SEND_MARKER;

	node->sync = false;
	node->mode = FLUX_NORMAL;
	node->isStandby = false;

	set_recovered_fluxen(((FluxProd *) node)->sched,
						 ((FluxProd *) node)->flux_idx);

	return;
}

void
received_frunning_SegProd(FluxProd_FT *node, int part_no, bool mask)
{
	FluxConnInfo   *fcinfo;

	if (IsPrimary(node))
	{
		Assert((mask == TSB_PRIMARY) && (part_no == 0));
		fcinfo = &(node->fpstate_ft->conn_info_p[0]);

		fcinfo->status = CONN_SEND_DATA;
	}
	else
	{
		Assert((mask == TSB_SECONDARY) && (part_no == 0));

		fcinfo = &(node->fpstate_ft->conn_info_s[0]);
		fcinfo->status = CONN_ACK_DATA;
	}
}

bool
ready_to_copy_SegProd(SegProd *node)
{
	FluxConnInfo   		*fcinfo;
	FluxProdState_FT  	*fpstate = node->fprod.fpstate_ft;

	if (IsPrimary(node))
	{
		fcinfo = &(fpstate->conn_info_p[EGRESSCONN]);

		if (IsSending(fcinfo))
		{
			return false;
		}

	}
	else
	{
		fcinfo = &(fpstate->conn_info_s[EGRESSCONN]);

		if (IsSending(fcinfo))
		{
			return false;
		}

		/* I better have received the ack, before I start sending again */
		if (IsAlive(fcinfo) && (fcinfo->status == CONN_ACK_DATA))
		{
			return false;
		}
	}
	
	return true;
}

/*
 * --------------------
 *	ExecInitSegProd
 * --------------------
 */
bool
ExecInitSegProd(SegProd * node, EState *estate, Plan *parent)
{
	FluxProdState_FT *lfpstate;

	((FluxProd *) node)->route_fun_init = seg_prod_route_init;
	((FluxProd *) node)->route_fun = seg_prod_route;
	
	if (!ExecInitFluxProd_FT((FluxProd_FT *) node, estate, parent))
	{
		return FALSE;
	}

	lfpstate = node->fprod.fpstate_ft;
	
	/* Adjust the fmask */
	if (IsPrimary(node))
	{
		lfpstate->fmask = (TSB_PRODUCED | TSB_PRIMARY);

		if (((FluxProd_FT *) node)->isPair)
		{
			/* Never use the secondary connection */
			lfpstate->conn_info_s[EGRESSCONN].status = CONN_DO_NOTHING;
			lfpstate->num_conn_s++;
		}
	}
	else
	{
		lfpstate->fmask = (TSB_PRODUCED | TSB_SECONDARY);

		/* Never use the primary connection */
		lfpstate->conn_info_p[EGRESSCONN].status = CONN_DO_NOTHING;
		lfpstate->num_conn_p++;
	}
	
	/* Adjust the partition maps */
	lfpstate->map_p2m_s[EGRESSCONN] = 0;
	lfpstate->map_m2p_s[EGRESSCONN] = 0;

	lfpstate->fmasks[EGRESSCONN] = lfpstate->fmask;

	return TRUE;
}

int
ExecCountSlotsSegProd(SegProd * node)
{
	return ExecCountSlotsFluxProd_FT((FluxProd_FT *) node);
}

/*
 * -----------------
 *	ExecEndFluxIngress
 * -----------------
 */
void
ExecEndSegProd(SegProd * node)
{
	ExecEndFluxProd_FT((FluxProd_FT *) node);
}

/* State movement functions -- exactly as nodeFluxProd_FT */

bool ExecGetStateInitSegProd(SegProd * node)
{
	return ExecGetStateInitFluxProd_FT((FluxProd_FT *) node);
}

bool ExecGetStateNextSegProd(SegProd * node, char *buf, int bsize)
{
	return ExecGetStateNextFluxProd_FT((FluxProd_FT *) node, buf, bsize);
}

bool ExecGetStateCloseSegProd(SegProd * node)
{
	return ExecGetStateCloseFluxProd_FT((FluxProd_FT *) node);
}

bool ExecInstallStateInitSegProd(SegProd * node)
{
	return ExecInstallStateInitFluxProd_FT((FluxProd_FT *) node);
}

bool ExecInstallStateNextSegProd(SegProd * node, char *buf, int bsize)
{
	if (ExecInstallStateNextFluxProd_FT((FluxProd_FT *) node, buf, bsize))
	{
		if (IsPrimary(node))
		{
			node->fprod.fpstate_ft->fmasks[EGRESSCONN] = (TSB_PRODUCED | TSB_PRIMARY);
		}
		else
		{
			node->fprod.fpstate_ft->fmasks[EGRESSCONN] = (TSB_PRODUCED | TSB_SECONDARY);
		}

		return TRUE;
	}

	return FALSE;
}

bool ExecInstallStateCloseSegProd(SegProd * node)
{
	return ExecInstallStateCloseFluxProd_FT((FluxProd_FT *) node);
}

bool ExecResetStateSegProd(SegProd * node)
{
	return ExecResetStateFluxProd_FT((FluxProd_FT *) node);
}

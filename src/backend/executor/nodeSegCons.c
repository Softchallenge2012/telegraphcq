/*-------------------------------------------------------------------------
 *
 * nodeFluxCons_FT.c
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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeSegCons.c,v 1.19 2004/07/15 20:39:55 owenc Exp $
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

#include "executor/nodeSegCons.h"
#include "executor/nodeFSched.h"
#include "telegraphcq/fluxroutines.h"

#include "executor/nodeFluxProd_FT.h"

#define  INGRESS_CONN 0

#define  SCROLE(sc)  (((FluxCons_FT *) sc)->m_role)

/*
 * ---------------------------------------
 *	 ExecSegCons
 *
 * ---------------------------------------
 */

bool
getResultFromConn(Plan *node, FluxConnInfo * fcinfo,
				  result * p_res, int conn_idx)
{
	int			rval;

	if (fcinfo->p_htup) return true;
	
	rval = fconn_recv_result(fcinfo, p_res);

	if (rval < 0)
	{
		elog(DEBUG1, "Error receiving queue from FluxProd: %d. Setting it to failed",
			 conn_idx);
		elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
		set_failed_conn(node, fcinfo, rval, conn_idx);
		return false;
	}
	else if (rval > 0)
	{
		if (p_res->type == RESULT_HEAPTUPLE)
		{
			fcinfo->p_htup = p_res->data.tuple;
		}
	}
	
	return rval;
}

static bool
getNextTuple(FluxCons_FT * node, int conn_idx,
			 result * p_res)
{

#ifdef DEBUG_TUPLES
	int seqnum; bool isNull;
#endif

	FluxConnInfo *fcinfo;

	fcinfo = &(node->fcstate_ft->conn_info_p)[conn_idx];

	/* Ensures that the acks are sent out if needed */
	if (!process_acks_FluxCons_FT(node, fcinfo))
	{
		return false;
	}
	
	if (!getResultFromConn((Plan *) node,
						   fcinfo,
						   p_res,
						   conn_idx))
	{
		return false;
	}
	
#ifdef DEBUG_TUPLES
	elog(DEBUG1, "Got a result");
#endif

	/* If the result has a marker, don't ack it, but return true. */
	if (!fcinfo->p_htup)
	{
		return true;
	}
	
#ifdef DEBUG_TUPLES
	elog(DEBUG1, "Enqueing ack for result.");


	seqnum = heap_getattr(fcinfo->p_htup, 1,
						  node->fcons.tupDesc,
						  &isNull);

	elog(DEBUG1, "R-%d: ACK: %d", node->m_role, seqnum);
#endif

	/* Send the ack */
	if (!sendAckToConn_FluxCons_FT(node, fcinfo, fcinfo->p_htup))
	{
		return false;
	}
	
#ifdef DEBUG_TUPLES
	elog(DEBUG1, "Enqueue succeeded.");
#endif

	p_res->type = RESULT_HEAPTUPLE;
	p_res->data.tuple = fcinfo->p_htup;
	fcinfo->p_htup = NULL;

	return true;
}

static void
check_conn_complete(FluxCons_FT * node)
{

	node->fcons.all_done =
		(node->fcstate_ft->num_conn_p ==
		 node->fcons.nProd);

}

/* Sets up a new statemovement request and puts the operator in the
   given mode. */
static void
process_fprodsync(SegCons * node, int mach, smr_t action, FluxMode next_mode)
{
	StateMover *sm = (StateMover *) ((FluxCons *) node)->sched->fluxen[STATE_MOVER_IDX];

#ifdef DRECOVERY
	elog(DEBUG1, "%d: SegCons --- Got an fprodsync message.",
		 SCROLE(node));
#endif
	
	Assert(nodeTag(sm) == T_StateMover);

#ifdef DRECOVERY
	elog(DEBUG1, "%d: SegCons --- Creating request: ACTION: %d, MACH: %d",
		 SCROLE(node),
		 action, mach);
#endif
	
	node->curr_smr = new_smr(mach, &(node->ready_flag),
							 sm->move_bufs, action);

	if (!failed_node)
	{
		int mfail;
			
		if (((FluxCons_FT *) node)->isStandby)
		{
			mfail = (SCROLE(node) == F_SECONDARY) ?
				ROTATE_R(mach, ((FluxCons *) node)->sched->ddeclust) :
				ROTATE_L(mach, ((FluxCons *) node)->sched->ddeclust);
		}
		else
		{
			mfail = (SCROLE(node) == F_PRIMARY) ?
				ROTATE_R(tnodeid_2_vmach(((FluxCons *)node)->node_id),
						 ((FluxCons *)node)->sched->ddeclust) :
				ROTATE_L(tnodeid_2_vmach(((FluxCons *)node)->node_id),
						 ((FluxCons *)node)->sched->ddeclust);
		}

/* #ifdef DRECOVERY */
 		elog(DEBUG1, "%d: SegCons --- fprodsync, none failed, setting %d failed",
			 SCROLE(node), mfail);
/* #endif */
			 
		/* No connection really died, but a machine did */
		set_failed_machine(((FluxCons *) node)->sched, mfail); 
	}
	
	((FluxCons_FT *) node)->mode = next_mode;
	((FluxCons_FT *) node)->fcstate_ft->rec_mode = SIGNAL_SMOVER;
}

static TupleTableSlot *
do_normal(SegCons * snode)
{
	TupleTableSlot 		*slot;
	FluxCons_FT 		*node 	  = (FluxCons_FT *) snode;
	FluxConsState_FT 	*lfcstate = node->fcstate_ft;
	result				 res;
	
/* #define DEBUG_TUPLES_UP */

#ifdef DEBUG_TUPLES_UP
	
	int  seqnum;
	bool isNull;
	
#endif

	if (IsPushPlan(node) && parentPlan(node) && (!IsInputDone(parentPlan(node))))
	{
		return NULL;
	}
	
	slot = lfcstate->csstate.cstate.cs_ResultTupleSlot;

	ExecClearTuple(slot);

	if (!getNextTuple(node, INGRESS_CONN, &res))
	{
		return slot;
	}
	
	switch (res.type)
	{
		
		case RESULT_HEAPTUPLE:
			
			ExecStoreTuple(res.data.tuple, slot, InvalidBuffer, true);

#ifdef DEBUG_TUPLES_UP
			seqnum = heap_getattr(slot->val, 1,
								  slot->ttc_tupleDescriptor, &isNull);
			
			elog(DEBUG1, "%d: SCONS Returning a slot: SNUM %d", node->m_role, seqnum);
#endif

			break;

		case RESULT_FPRODSYNC:
#ifdef DRECOVERY
			elog(DEBUG1, "Got an fprodsync.");
#endif
			process_fprodsync(snode, res.data.info,
							  SMR_SEND_BUF,
							  FLUX_FAIL_OVER_1);
			break;

		default:
			elog(ERROR, "Don't understand type:%d", res.type);
			break;
	}

#define COLLECT_STATS

#ifdef COLLECT_STATS
	if (!TupIsNull(slot))
	{
		node->instrument->firsttuple++;
	}
#endif

	if (IsPushPlan(node) && parentPlan(node) && (!TupIsNull(slot)))
	{
		SetInputNew(parentPlan(node));
	}
	
	return slot;
}


/* XXX This should never be called */
static void
do_fail_over(SegCons * node)
{
	FluxCons_FT *fcons = (FluxCons_FT *) node;
	
	if (which_failed == ((FluxCons *) fcons)->map_c2m[INGRESS_CONN])
	{
		elog(ERROR,"%d: SegCons ingress connection failed, killing myself",
			 SCROLE(node));
		return;
	}

#ifdef DRECOVERY
	elog(DEBUG1, "%d: SegCons doing fail over, setting back to normal.", SCROLE(node));
#endif
	
	/* Otherwise reset the state, so we can continue until the sync comes in */
	fcons->mode = fcons->isStandby ? STANDBY_NORMAL : FLUX_NORMAL;
}

static void
do_fail_over_1(SegCons * node)
{
	FluxConsState_FT *lfcstate = ((FluxCons_FT *) node)->fcstate_ft;

	switch (lfcstate->rec_mode)
	{
		case (SIGNAL_SMOVER):
			{
				StateMover *sm = (StateMover *)
					((FluxCons *) node)->sched->fluxen[STATE_MOVER_IDX];

				Assert(nodeTag(sm) == T_StateMover);

#ifdef DRECOVERY
				elog(DEBUG1, "%d: SegCons --- enqueuing an smr: %x",
					 SCROLE(node), (uint) (node->curr_smr));
#endif
	
				if (ENQUEUE_INT(sm->request_queue, &(node->curr_smr)))
				{
					lfcstate->rec_mode = WAIT_SMOVER;
				}
				
				break;
			}

		case (WAIT_SMOVER):
			{
/* 				elog(DEBUG1, "%d: SegCons waiting for SM readiness" , SCROLE(node)); */
				/* Check to see if statemover is ready */
				if (node->ready_flag)
				{
					
#ifdef DRECOVERY
					elog(DEBUG1, "%d: SegCons ... SM ready." , SCROLE(node));
#endif
					lfcstate->rec_mode = WAIT_FPRODSYNC;

				}

				break;
			}

		case (WAIT_FPRODSYNC):
			{
				if (((FluxCons_FT *) node)->isStandby)
				{
#ifdef DRECOVERY
					elog(DEBUG1, "%d: SegCons ... Standby ready for take_over." ,
						 SCROLE(node));
#endif
					/* The above FProd should always be ready on a standby */
					ExecInstallStateInit(parentPlan(node));
					((FluxCons_FT *) node)->mode = STANDBY_TAKE_OVER;
					lfcstate->rec_mode = ADJUST_CONN;
					
					node->fcons.instrument->pages_moved = 0;
					gettimeofday(&(node->fcons.instrument->move_start), NULL);
					
				}
				else
				{
					/* Make sure that the above FProd is ready to move stuff */
					if (((FluxCons_FT *)node)->fprod_parent)
					{
						if (!ready_to_copy_FluxProd_FT(
								((FluxCons_FT *) node)->fprod_parent))
							return;
					}

#ifdef DRECOVERY
					elog(DEBUG1, "%d: SegCons ... FProd ready, doing take_over." ,
						 SCROLE(node));
#endif
					
					((FluxCons_FT *) node)->mode = FLUX_TAKE_OVER;
					lfcstate->rec_mode = ADJUST_CONN;
				}
				
				break;
			}

		default:
			elog(ERROR, "%d: SegCons does not know recovery mode %d",
				 SCROLE(node), lfcstate->rec_mode);
			break;
	}
}

static void
do_take_over(SegCons * node)
{
	FluxConsState_FT *lfcstate = ((FluxCons_FT *) node)->fcstate_ft;
	
    FluxConnInfo *fcinfo;

#ifdef NOT_USED
	elog(DEBUG1, "%d: SegCons in do_take_over", SCROLE(node));
#endif
	
	fcinfo = &(lfcstate->conn_info_p[INGRESS_CONN]);

	switch (lfcstate->rec_mode)
	{
		case (ADJUST_CONN):

			/* Must ensure the return queue is valid. */
			if (!process_acks_FluxCons_FT((FluxCons_FT *) node, fcinfo))
			{
				return;
			}
			

			/* Send out a signal indicating we are copying state, must flush. */
			if (!sendIntAckToConn_FluxCons_FT((FluxCons_FT *) node, fcinfo,
											  ACK_FCONSSYNC, true))
				return;

#ifdef DRECOVERY
			elog(DEBUG1, "%d: SegCons FCONSSYNC enqueued.", SCROLE(node));
#endif

/* #define DEBUG_EDGES */

#ifdef DEBUG_EDGES
			fcinfo->tmp_seqno = 1;
#endif
			lfcstate->rec_mode = WAIT_ACKS;
			
			break;

		case (WAIT_ACKS):

			/* Make sure the FCONSSYNC is flushed */
			if (!process_acks_FluxCons_FT((FluxCons_FT *) node, fcinfo))
			{
				return;
			}
			
#ifdef DRECOVERY
			elog(DEBUG1, "%d: SegCons FCONSYNC flushed and FProd ready", SCROLE(node));
#endif
			
			ExecGetStateInit(parentPlan(node));
			
			node->fcons.instrument->pages_moved = 0;
			gettimeofday(&(node->fcons.instrument->move_start), NULL);

			lfcstate->rec_mode = UNKNOWNFOS; /* Unknown */

			node->move_done = false;
				
			((FluxCons_FT *) node)->mode = FLUX_TAKE_OVER_1;

			break;
	}
	
}

/* Actually doing the statemovement. */
static void
do_take_over_1(SegCons * node)
{
/* 	elog(DEBUG1, "%d: SegCons in do_take_over_1", SCROLE(node)); */
	Instrumentation *instr = node->fcons.instrument;

	if ((node->curr_q == NULL) && (!node->move_done))
	{
		make_queue(&(node->curr_q), *p_no_lock, sm_queue_size);
		if (!ExecGetStateNext(parentPlan(node), node->curr_q->start,
							  sm_queue_size - QSTRUCTSIZE))
		{
#ifdef DRECOVERY
			elog(DEBUG1, "%d: SegCons sending page", SCROLE(node));
#endif
			instr->pages_moved++;
		}
		else
		{
			node->move_done = true;

			elog(DEBUG1, "%d: SegCons moved: %d pages, %d bytes, %.3f ms",
				 SCROLE(node),
				 instr->pages_moved,
				 instr->pages_moved * sm_queue_size,
				 f_time_since(&(instr->move_start)));
			
			instr->pages_moved = 0;
		}
	}

	/* Could not enqueue the current buffer */
	if (!ENQUEUE_INT(node->curr_smr->state_q, &(node->curr_q)))
		return;

	/* Are we done ? */
	if (node->curr_q != NULL)
	{
		/* No */
		node->curr_q = NULL;
		
		return;
	}

	/* Yes, we are done! */
	/* pfree(node->curr_smr); */
	node->curr_smr = NULL;

	ExecGetStateClose(parentPlan(node));

#define SIGNAL_PARENT
	
#ifdef SIGNAL_PARENT
	if (((FluxCons_FT *) node)->fprod_parent)
	{
		
#ifdef DRECOVERY
		elog(DEBUG1, "%d: SegCons signalling parent FProd.", SCROLE(node));
#endif
		
		/* Signal the parent */
		((FluxCons_FT *) node)->fprod_parent->sync = true;
	}
#endif

	/* We do not need to send a sync ack, so we are done! */
	elog(DEBUG1, "%d: SegCons going back into normal mode.", SCROLE(node));

	((FluxCons_FT *) node)->mode = FLUX_NORMAL;
	
	set_recovered_fluxen(((FluxCons *) node)->sched,
						 ((FluxCons *) node)->flux_idx);

	return;
}

/* Standby code */
static TupleTableSlot *
do_standby_normal(SegCons * snode)
{
	TupleTableSlot 		*slot;
	FluxCons_FT 		*node 	  = (FluxCons_FT *) snode;
	FluxConsState_FT 	*lfcstate = node->fcstate_ft;
	result				 res;


	/* Don't need to worry about returning anything here */
	   
	slot = lfcstate->csstate.cstate.cs_ResultTupleSlot;

	ExecClearTuple(slot);

	if (!getNextTuple(node, INGRESS_CONN, &res))
	{
		return slot;
	}
	
	switch (res.type)
	{
		case RESULT_FPRODSYNC:
			process_fprodsync(snode, res.data.info,
							  SMR_RECV_BUF,
							  STANDBY_FAIL_OVER_1);
			break;

		default:

#define DEBUG_FIRST_TUPS

#ifdef DEBUG_FIRST_TUPS
			elog(DEBUG1, "%d: SegCons In standby mode, cannot process tuple of type %d",  
				 ((FluxCons_FT *) snode)->m_role, res.type);
			sleep(10000);
#else
			elog(ERROR, "%d: SegCons In standby mode, cannot process tuple of type %d",  
				 ((FluxCons_Ft *) snode)->m_role, res.type);
#endif
			break;
	}

	return slot;
}

static void
do_standby_take_over(SegCons * node)
{
/* 	elog(DEBUG1, "%d: SegCons In do_standby_take_over", */
/* 		 SCROLE(node)); */

	Instrumentation *instr = node->fcons.instrument;
		
	if (node->curr_q == NULL)
	{
		if (!DEQUEUE_INT(node->curr_smr->state_q, &node->curr_q))
			return;

#ifdef DRECOVERY
		elog(DEBUG1, "%d: SegCons: Received another queue.",
			 SCROLE(node));
#endif

		/* End of packets */
		if (node->curr_q->size == QSTRUCTSIZE)
		{

#ifdef DEBUG_EDGES
			FluxConnInfo *fcinfo;
			fcinfo = &(((FluxCons_FT *) node)->fcstate_ft->conn_info_p[INGRESS_CONN]);
			fcinfo->tmp_seqno = 1;
#endif

			free_queue(&(node->curr_q));
			node->curr_q = NULL;

			/* Since I'm last, I free the request */
			pfree(node->curr_smr);
			node->curr_smr = NULL;

			/* Unfreeze the dataflow segment */
			ExecInstallStateClose(parentPlan(node));

#ifdef SIGNAL_PARENT
			if (((FluxCons_FT *) node)->fprod_parent)
			{
				
#ifdef DRECOVERY
				elog(DEBUG1, "%d: SegCons signalling parent FProd.", SCROLE(node));
#endif
				/* Signal the parent */
				((FluxCons_FT *) node)->fprod_parent->sync = true;
			}
#endif

#ifdef DRECOVERY
			/* We do not need to send a sync ack, so we are done! */
			elog(DEBUG1, "%d: SegCons: finished with transfer request, going to normal.",
				 SCROLE(node));
#endif
			elog(DEBUG1, "%d: SegCons received: %d pages, %d bytes, %.3f ms",
				 SCROLE(node),
				 instr->pages_moved,
				 instr->pages_moved * sm_queue_size,
				 f_time_since(&(instr->move_start)));

			instr->pages_moved = 0;

			/* No longer a standby. */
			((FluxCons_FT *) node)->isStandby = false;

			((FluxCons_FT *) node)->mode = FLUX_NORMAL;

			/* Tell runtime we are recovered. */
			set_recovered_fluxen(((FluxCons *) node)->sched,
								 ((FluxCons *) node)->flux_idx);

			return;
		}
	}

#ifdef DRECOVERY
	elog(DEBUG1, "%d: SegCons: Received and installing state",
		 SCROLE(node));
#endif

	if (instr->pages_moved == 0)
	{
		elog(DEBUG1, "%d: SegCons time to first page: %.3f",
			 SCROLE(node),
			 f_time_since(&(instr->move_start)));
	}
	
	instr->pages_moved++;
	
	ExecInstallStateNext(parentPlan(node),
						 node->curr_q->start,
						 sm_queue_size - QSTRUCTSIZE);

	free_queue(&(node->curr_q));
	node->curr_q = NULL;
}

TupleTableSlot *
ExecSegCons(SegCons * snode)
{
	TupleTableSlot *slot = NULL;
	FluxCons_FT *node = (FluxCons_FT *) snode;

	/* For SegCons, there are no failed paired nodes */
	if (!node->fcons.all_done)
	{
		check_conn_complete((FluxCons_FT *) node);
		return NULL;
	}

	if (IsPushPlan(node) && parentPlan(node))
	{
		ExecProcNode(parentPlan(node), (Plan *) node);
	}
	
	switch (node->mode)
	{
		case FLUX_NORMAL:
			slot = do_normal(snode);
			break;

		case FLUX_FAIL_OVER_1:
			do_fail_over_1(snode);
			slot = do_normal(snode);
			break;

		case FLUX_TAKE_OVER:
			do_take_over(snode);
			break;

		case FLUX_TAKE_OVER_1:
			do_take_over_1(snode);
			break;

		case STANDBY_NORMAL:
			slot = do_standby_normal(snode);
			break;

		case STANDBY_FAIL_OVER_1:
			do_fail_over_1(snode);
			break;

		case STANDBY_TAKE_OVER:
			do_standby_take_over(snode);
			break;

			/* Just resets the state */
		case FLUX_FAIL_OVER:
			do_fail_over(snode);
			break;

		default:
			break;
	}

	/* There will only be one connection */
	return slot;
}

/* -----------------
 * ExecInitSegCons
 *
 * -----------------
 */
bool
ExecInitSegCons(SegCons * node, EState *estate, Plan *parent)
{
	return ExecInitFluxCons_FT((FluxCons_FT *) node, estate, parent);
}

int
ExecCountSlotsSegCons(SegCons * node)
{
	return ExecCountSlotsFluxCons_FT((FluxCons_FT *) node);
}


/* -----------------------
 *	ExecEndSegCons(node)
 * -----------------------
 */
void
ExecEndSegCons(SegCons * node)
{
	ExecEndFluxCons_FT((FluxCons_FT *) node);
}

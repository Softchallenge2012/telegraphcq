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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxCons_FT.c,v 1.33 2005/02/09 00:16:42 phred Exp $
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

#include "rqueue/rqdest.h"		/* result */
#include "telegraphcq/fluxroutines.h"

#include "executor/nodeSegCons.h" /* for getResultFromConn */

#include "executor/nodeFSched.h"  /* for recovered Fluxen */

#include "executor/nodeFluxProd_FT.h" /* for ready_to_copy_FluxProd_FT */

#include "executor/tsb.h"

static bool getNextTuple_ord(FluxCons_FT *node, result *p_res);
static bool getNextTuple_bord(FluxCons_FT *node, result *p_res);
static void permute_cmerge(CMerge *cmerge, int *perm_map, int size);

/*
 * Message handlers
 *
 * Ack handlers.
 *
 */

static void
init_snapshot(FluxCons_FT *node)
{
	int i;
	int i_s;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	FluxConnInfo *fcinfo;
	
	fcstate->snapshot.initp = true;

	fcstate->snapshot.partner_fail = false;

	fcstate->snapshot.partner_m = UNKNOWN_MACHINE;
	
	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		/* Record wether the machines are alive or not */
		fcinfo = &(fcstate->conn_info_p[i]);
		fcstate->snapshot.alive_p[i] = IsAlive(fcinfo);
		i_s = fcstate->map_p2s_m[i];
		fcinfo = &(fcstate->conn_info_s[i_s]);
		fcstate->snapshot.alive_s[i] = IsAlive(fcinfo);
		fcstate->snapshot.got_syncs[i] = false;
	}
}

static bool
ready_to_move(FluxCons_FT *node)
{
	int i;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	SnapshotCB *snapshot = &(fcstate->snapshot);

	if (!(snapshot->initp))
	{
		elog(ERROR, "Snapshot should be initialized!");
	}
	
	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		if (!snapshot->partner_fail)
		{
			/* My partner didn't fail. */
			/* Both were alive at snapshot time. */
			/* They don't need to send me syncs. */
			if (snapshot->alive_p[i] &&
				snapshot->alive_s[i])
				continue;
		}
		
		if (!snapshot->got_syncs[i])
			return false;
	}

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons ready to move.", node->m_role, node->fcons.flux_idx);
#endif
	
	return true;
}

static void
prepare_move(FluxCons_FT *node)
{
	smr_t action;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	StateMover *sm = getSM(node, FluxCons);
	
	/* Clear the sync control block */
	fcstate->snapshot.initp = false;

	/* If partner did not fail, no movement necessary */
	if (!fcstate->snapshot.partner_fail) {

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons going back to normal mode", node->m_role, node->fcons.flux_idx);
#endif
		
		node->mode = FLUX_NORMAL;
		fcstate->rec_mode = 0;
			
		return; /* We are done */
	}

	/* If not a standby we receive, otherwise send */
	action = (node->isStandby) ?
		SMR_RECV_BUF : SMR_SEND_BUF;

	/* Create a smr */
	node->curr_smr = new_smr(fcstate->snapshot.partner_m,
							 &(node->ready_flag),
							 sm->move_bufs,
							 action);

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons not signalling smaover -- action: %d, mach: %d",
		 node->m_role, node->fcons.flux_idx, node->curr_smr->action, node->curr_smr->mach);
#endif
	
	fcstate->rec_mode = SIGNAL_SMOVER;
}


/* XXX These two don't work quite as they should because if things change
   between receiving alives and syncs, this won't work */

static void
received_fprodalive(FluxCons_FT *node, int cidx, int data, bool mask)
{
	int i_s, part_no;
	FluxConnInfo *fcinfo;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	
	if (!fcstate->snapshot.initp)
	{
		init_snapshot(node);
	}

	/* XXX I should set the machine to failed? Probably, but it does not matter
	   right now. */
	fcstate->snapshot.partner_fail = true;

	if (data != -1)
	{
		fcstate->snapshot.partner_m  = data;
	}

#define DEBUG_MARKERS

#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons received fprodalive from conn: %d, mask: %d, data: %d",
		 node->m_role, node->fcons.flux_idx, cidx, mask, data);
#endif
#endif
	
	switch (mask)
	{
		case (TSB_PRIMARY):
			fcinfo = &(fcstate->conn_info_p[cidx]);
			if (!IsAlive(fcinfo)) 
			{
				elog(ERROR, "Not alive and got an fprodalive");
			}
			
			/* Look at the secondary. I know I'm alive, cause I just got the message */
			i_s = fcstate->map_p2s_m[cidx];
			fcinfo = &(fcstate->conn_info_s[i_s]);
			part_no = cidx;


			/* I received both syncs --- reset them */
			if (fcstate->syncs.sync_recv_s[part_no] ==
				RESULT_FPRODALIVE)
			{
					
				fcstate->syncs.sync_recv_p[part_no] = false;
				fcstate->syncs.sync_recv_s[part_no] = false;
				fcstate->snapshot.got_syncs[part_no] = true;

				if (fcinfo->status == CONN_ACK_CHECK_DATA)
				{
					fcinfo->status = CONN_ACK_DATA;
				}
						
				/* Otherwise, we just ignore it because it is an
				   FPRODSYNC an we are expecting that, this ALIVE is
				   the stray one before the sync. */
					
			}
			/* I received just one sync --- set it */
			else
			{
				fcstate->syncs.sync_recv_p[part_no] = RESULT_FPRODALIVE;

				/* Gotta tell secondary to look out for a sync. */
				if (fcinfo->status == CONN_ACK_DATA)
				{
					fcinfo->status = CONN_ACK_CHECK_DATA;
				}
					
			}
			
			break;

		case (TSB_SECONDARY):
			fcinfo = &(fcstate->conn_info_s[cidx]);
			if (!IsAlive(fcinfo))
			{
				elog(ERROR, "Not alive and got an fprodalive");
			}

			/* Look at the primary. I know I'm alive, cause I just got the message */
			i_s = fcstate->map_s2p_m[cidx];
			part_no = i_s;

			/* I received both syncs --- reset them */
			if (fcstate->syncs.sync_recv_p[part_no] ==
				RESULT_FPRODALIVE)
			{
					
				fcstate->syncs.sync_recv_p[part_no] = false;
				fcstate->syncs.sync_recv_s[part_no] = false;
				fcstate->snapshot.got_syncs[part_no] = true;
					
				/* No longer need to check for the data */
				if (fcinfo->status == CONN_ACK_CHECK_DATA)
				{
					fcinfo->status = CONN_ACK_DATA;
				}

				/* Otherwise just ignore, we are looking for the
				   matching prod-sync */
					
			}
			/* I received just one sync --- set it */
			else
			{
				fcstate->syncs.sync_recv_s[part_no] =
					RESULT_FPRODALIVE;
			}
			
			break;
	}

	if (!ready_to_move(node)) return;

#define DEBUG_MOVE
	
#ifdef DEBUG_MOVE
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons preparing to move.", node->m_role, node->fcons.flux_idx);
#endif
#endif
	
	prepare_move(node);

	return;
}

static void
received_fprodsync(FluxCons_FT *node, int cidx, int data, bool mask)
{
	int i_s, part_no;

	FluxConnInfo *fcinfo_p, *fcinfo_s;
	FluxConsState_FT *fcstate = node->fcstate_ft;

	int ack_code =
		(node->isStandby) ?
		ACK_FSTANDBY : ACK_FRUNNING;
	
	/* If it was reset, we need to undo the flag, and take snapshot. */
	if (!fcstate->snapshot.initp)
	{
		init_snapshot(node);
	}
	
	/* Someone told me that my partner failed */
	if (data != -1)
	{
		fcstate->snapshot.partner_fail = true;
		fcstate->snapshot.partner_m = data;
	}

#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons received fprodsync from conn: %d, mask: %d, data: %d",
		 node->m_role, node->fcons.flux_idx, cidx, mask, data);
#endif
#endif

	if (mask == TSB_PRIMARY)
	{
		fcinfo_p = &(fcstate->conn_info_p[cidx]);
		i_s = fcstate->map_p2s_m[cidx];
		fcinfo_s = &(fcstate->conn_info_s[i_s]);
		part_no = cidx;

#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons last received seqnum for prim part: %d is %d, htup: %x, r: %d",
			 node->m_role, node->fcons.flux_idx, part_no, fcstate->merge.snum[part_no],
			 (uint) fcstate->merge.htup[part_no],
			 fcstate->merge.ready[part_no]);
#endif
#endif
		
		fcinfo_p = &(fcstate->conn_info_p[cidx]);
		/* Turn on the bit for the primary */
/* 		fcstate->syncs.sync_recv_p[part_no] = true; */

		/* XXX I don't check for the alive status of the secondary here because
		   it will never really be in the ERROR state. If its in the unkown
		   state, then it will eventually send me a sync. And if there are
		   multiple failures, I won't know because I don't send version numbers
		   around yet. Eventually you want to send version numbers. */

		if (fcstate->syncs.sync_recv_s[part_no] ==
			RESULT_FPRODSYNC) 
		{
			/* Both syncs were received */
			fcstate->syncs.sync_recv_p[part_no] = false;
			fcstate->syncs.sync_recv_s[part_no] = false;
			fcstate->snapshot.got_syncs[part_no] = true;

#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons marking *primary* & secondary as ALIVE, part_no: %d",
				 node->m_role, node->fcons.flux_idx, part_no);
#endif

			if (fcinfo_p->status == CONN_RECOVER)
			{
				/*
				 * We need to send a marker indicating we are running.
				 */
				smark_conn(fcinfo_p, CONN_RECV_DATA,
						   0, ack_code);
			}
			
			/* Because the secondary may be paused */
			if (IsSending(fcinfo_s) &&
				(fcinfo_s->next_state == CONN_PAUSE))
			{
				/* When done sending the marker, we are acking */
				fcinfo_s->next_state = CONN_ACK_DATA;
			}
			else
			{
				fcinfo_s->status  = CONN_ACK_DATA;
			}
		}
		else
		{
			/* Received a sync from the primary, but not the secondary */
			fcstate->syncs.sync_recv_p[part_no] = RESULT_FPRODSYNC;
				
			/* Gotta tell the secondary to look out for a sync. */
			if (fcinfo_s->status == CONN_ACK_DATA)
			{
/* 				elog(DEBUG1, "%d: FCons setting secondary to CONN_ACK_CHECK_DATA", */
/* 					 node->m_role); */
				fcinfo_s->status = CONN_ACK_CHECK_DATA;
			} /* Otherwise is should be CONN_RECOVER, and already checking */
			
			/*
			 * Because I am waiting for the secondary to send me a sync, I don't
			 * want to process any incoming data. Otherwise, I will throw away
			 * that data if the secondary is in CONN_RECOVER or CONN_RECV_DATA.
			 */

#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons marking primary as PAUSE, part_no: %d",
				 node->m_role, node->fcons.flux_idx, part_no);
#endif
			
			if (fcinfo_p->status == CONN_RECOVER)
			{
				/*
				 * We need to send a marker indicating we are running.
				 */
				smark_conn(fcinfo_p, CONN_PAUSE,
						   0, ack_code);
			}
			else
			{
				fcinfo_p->status = CONN_PAUSE;
			}
		}
	}
	else /* TSB_SECONDARY */
	{
		fcinfo_s = &(fcstate->conn_info_s[cidx]);
		i_s = fcstate->map_s2p_m[cidx];
		fcinfo_p = &(fcstate->conn_info_p[i_s]);

		part_no = i_s;

#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons last received seqnum for sec part on: %d is %d, htup: %x, r: %d",
			 node->m_role, node->fcons.flux_idx, part_no, fcstate->merge.snum[part_no],
			 (uint) fcstate->merge.htup[part_no],
			 fcstate->merge.ready[part_no]);
#endif
#endif

		/* Indicate we received the data marker */
/* 		fcstate->syncs.sync_recv_s[part_no] = true; */

		if (fcstate->syncs.sync_recv_p[part_no] ==
			RESULT_FPRODSYNC)
		{
			fcstate->syncs.sync_recv_p[part_no] = false;
			fcstate->syncs.sync_recv_s[part_no] = false;
			fcstate->snapshot.got_syncs[part_no] = true;

			/*
			 * Both syncs were received, the pause on the primary must
			 * be rescinded, and this secondary connection must now ack
			 * the primary.
			 */

#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons marking primary & *secondary* as ALIVE, part_no: %d",
				 node->m_role, node->fcons.flux_idx, part_no);
#endif

			/* Because the primary may be paused */
			if (IsSending(fcinfo_p) &&
				(fcinfo_p->next_state == CONN_PAUSE))
			{
				/* When done sending the marker, we are acking */
				fcinfo_p->next_state = CONN_RECV_DATA;
			}
			else
			{
				/* Should already have been paused ... or receiving. */
				fcinfo_p->status  = CONN_RECV_DATA;
			}

			if (fcinfo_s->status == CONN_RECOVER)
			{
				/*
				 * We need to send a marker indicating we are running.
				 */
				smark_conn(fcinfo_s, CONN_ACK_DATA,
						   0, ack_code);
			}
			else
			{
				/* Could be CONN_ACK_CHECK_DATA or CONN_RECV_DATA anymore. */
				fcinfo_s->status = CONN_ACK_DATA;
			}
		}
		else
		{
			/* Sync received from the secondary */
			fcstate->syncs.sync_recv_s[part_no] = RESULT_FPRODSYNC;

#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons marking secondary as PAUSE, part_no: %d",
				 node->m_role, node->fcons.flux_idx, part_no);
#endif

			/* The primary has not sent me a sync. */
			if (fcinfo_s->status == CONN_RECOVER)
			{
				/*
				 * We need to send a marker indicating we are running.
				 */
				smark_conn(fcinfo_s, CONN_PAUSE,
						   0, ack_code);
			}
			else
			{
				/*
				 * I don't want to send ACKS for tuples that arrive before the
				 * sync from the primary. Those don't need to be processed by
				 * the secondary.
				 */
				
				fcinfo_s->status = CONN_PAUSE;
			}
			
			/* The primary should always be checking, in recovery or in
			   CONN_RECV_DATA mode */
		}
	}
	
	if (!ready_to_move(node)) return;

#ifdef DEBUG_MOVE
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons preparing to move.", node->m_role, node->fcons.flux_idx);
#endif
#endif

	prepare_move(node);
	
}

static void
received_pauseack(FluxCons_FT * node, int fidx, int mask)
{
	int i;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	
	Assert(mask == TSB_PRIMARY); /* No way it should be SECONDARY */

#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons received pause ack from conn: %d",
		 node->m_role, node->fcons.flux_idx, fidx);
#endif
#endif
	
	fcstate->pauses.ack_recv[fidx] = true;

	/* XXX This is the merge buffer stuff @dmerge-buf */
	fcstate->merge.use_buf = true;
	fcstate->merge.move_to_buf = true;
	
		
	for (i = 0; (i < ((FluxCons *) node)->nProd); i++)
	{
		if (IsAlive(&(fcstate->conn_info_p[i])) &&
			!fcstate->pauses.ack_recv[i])
			return; /* Have not received an ack from something
					   already alive */
	}

	/* All acks received, make sure all tuple acks are sent */

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons all pause acks received.",
		 node->m_role, node->fcons.flux_idx);
#endif
	
	fcstate->rec_mode = WAIT_TUP_ACKS;

	/* XXX @dmerge-buf */
	fcstate->merge.move_to_buf = false;
}

static void
check_tup_acks(FluxCons_FT * node)
{
	int i;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	
	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		if (CMergeTupPending(fcstate->merge, i))
		{
			return;
		}
	}

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons all tup acks sent.", node->m_role, node->fcons.flux_idx);
#endif
	
	/* No tuples pending */
	node->mode = FLUX_TAKE_OVER;
	fcstate->rec_mode = INIT_SM;
}

/* Sets the all_done flag in node to indicate its totally complete */
static void
check_conn_complete(FluxCons_FT * node)
{
	FluxConsState_FT *lfcstate = node->fcstate_ft;

	if (!node->isPair)
	{
		node->fcons.all_done =
			(lfcstate->num_conn_p == node->fcons.nProd);

		if (node->fcons.all_done)
		{
			elog(DEBUG1, "%d - %d: FCons all done.", node->m_role, node->fcons.flux_idx);
		}

		/* XXX Need to the set these ... sorry for the ugliness */
		if ((IsA(node, FluxCons_FT) || IsA(node, FluxEgress)) &&
			node->fcons.all_done)
		{
			int conn_no;
			
			for (conn_no = 0; conn_no < node->fcons.nProd; conn_no++)
			{
				lfcstate->conn_info_p[conn_no].status = CONN_RECV_DATA;
			}
		}
	}
	else
	{
		if (node->isPair == true)
		{
			node->fcons.all_done =
				(lfcstate->num_conn_p == node->fcons.nProd) &&
				(lfcstate->num_conn_s == node->fcons.nProd) &&
				(!FC_DCONN(lfcstate->conn_spare_p)) &&		/* Error is ok? */
				(!FC_DCONN(lfcstate->conn_spare_s));		/* Error is ok? */
		}
		else if (node->isPair == (true + 1))
		{
			node->fcons.all_done =
				(lfcstate->num_conn_p == node->fcons.nProd) &&
				(lfcstate->num_conn_s == node->fcons.nProd);
		}
		
		if (node->fcons.all_done)
		{
			elog(DEBUG1, "%d - %d: FCons all done.", node->m_role, node->fcons.flux_idx);
		}
		
		/* XXX Need to the set these ... sorry for the ugliness */
		if ((IsA(node, FluxCons_FT) || IsA(node, FluxEgress)) &&
			node->fcons.all_done)
		{
			int conn_no;
			
			for (conn_no = 0; conn_no < node->fcons.nProd; conn_no++)
			{
				lfcstate->conn_info_p[conn_no].status = CONN_RECV_DATA;
				lfcstate->conn_info_s[conn_no].status = CONN_ACK_DATA;
			}

#define DEBUG_CONN
			
#ifdef DEBUG_CONN
			elog(DEBUG1, "FCons m_role: %d - %d", node->m_role, node->fcons.flux_idx);
			print_connections(node);
#endif
		}
	}
}

/* Node id that is send is really partition id that is sent */
void
ExecFluxCons_FTAddConn(FluxCons_FT * node,
					   int sockfd, struct sockaddr_in *addr,
					   int node_id, int fprod_idx)
{
	FluxConnInfo 		*fcinfo = NULL;
	FluxConsState_FT 	*lfcstate;
	bool				 fromMirror = false;
	int					 cidx = -1;

	cidx = node->fcons.map_m2c[tnodeid_2_vmach(node_id)];

	if (cidx == UNKNOWN_CONN_IDX)
	{
		elog(DEBUG1, "This FluxCons_FT does not know about machine %d",
			 node_id);
		elog(DEBUG1, "Treating it as a spare ... ");
		cidx = SPARE_CONN_IDX;
	}

	lfcstate = node->fcstate_ft;

	if (fprod_idx == node->fcons.fprod_idx)
	{							/* This is my primary node */
		if (cidx != SPARE_CONN_IDX) 
		{
			lfcstate->num_conn_p++;
			fcinfo = &(lfcstate->conn_info_p[cidx]);
		}
		else
		{
			fcinfo = lfcstate->conn_spare_p;
			
#define T_OVERHEAD
			
#ifdef T_OVERHEAD
			if (!node->isPair) return;
#endif
		}
		
		fromMirror = false;		/* This is a normal connection */
	}
	else if (fprod_idx == node->fprod_idx_s)
	{
		if (cidx != SPARE_CONN_IDX)
		{
			lfcstate->num_conn_s++;
			fcinfo = &(lfcstate->conn_info_s[cidx]);
		}
		else
		{
			fcinfo = lfcstate->conn_spare_s;
#ifdef T_OVERHEAD
			if (!node->isPair) return;
#endif
		}
		
		fromMirror = true;		/* This is the reverse connection */
	}
	
	/* This is a remote connection */
	if (node_id != node->fcons.node_id)
	{
/* 		Queue	   *ack_q; */

		elog(DEBUG1, "Adding remote forward queue. From %d, from_idx %d",
			 node_id, fprod_idx);
		elog(DEBUG1, "Adding remote reverse queue. From %d, from_idx %d",
			 node_id, fprod_idx);
		elog(DEBUG1, "On fd: %d", sockfd);

		fcinfo->trans = newSocketTrans(sockfd, addr);
		
/* 		make_queue(&ack_q, *p_no_lock, ack_queue_size); */
/* 		init_conn_info(fcinfo, sockfd, */
/* 					   (Queue *) NULL, flux_queue_size, */
/* 					   ack_q, ack_queue_size); */

		fcinfo->status =
			(cidx == SPARE_CONN_IDX) ? CONN_RECOVER : CONN_NORMAL;

	}
	else
	{							/* This is a local connection */

		elog(DEBUG1, "Adding local forward queue. From %d, from_idx %d",
			 node_id, fprod_idx);
		elog(DEBUG1, "Adding local reverse queue. From %d, from_idx %d",
			 node_id, fprod_idx);
		elog(DEBUG1, "On fd: %d", sockfd);

		if (!fromMirror)
		{
			
/* 			init_conn_info(fcinfo, (-1), */
/* 						   node->fcons.local_q, flux_queue_size, */
/* 						   node->local_r, ack_queue_size); */
			
			fcinfo->trans = newLocalTrans_q(node->fcons.local_q, node->local_r);

#ifdef DEBUG_CONN
			elog(DEBUG1,"Connecting local_q: %x, local_r: %x",
				 (uint) node->fcons.local_q, (uint) node->local_r);
			fconn_print_info(fcinfo);
#endif
		}
		else
		{
			
/* 			init_conn_info(fcinfo, (-1), */
/* 						   node->local_q_s, flux_queue_size, */
/* 						   node->local_r_s, ack_queue_size); */

			fcinfo->trans = newLocalTrans_q(node->local_q_s, node->local_r_s);

#ifdef DEBUG_CONN
			elog(DEBUG1,"Connecting local_q_s: %x, local_r_s: %x",
				 (uint) node->local_q_s, (uint) node->local_r_s);
			fconn_print_info(fcinfo);
#endif
		}

		fcinfo->status =
			(cidx == SPARE_CONN_IDX) ? CONN_RECOVER : CONN_NORMAL;

	}

}

/* #define DBG_ACKS */

/* Returns false if not successfully done processing acks */
bool
process_acks_FluxCons_FT(FluxCons_FT * node,
						 FluxConnInfo * fcinfo)
{
	int			rval;
	
	if (!node->isPair) return true; /* No acks to send back */

	/* We have a secondary producer for this data, may need to ack to them */
	rval = fconn_is_ack_flushed(fcinfo);

	if (rval < 0)
	{
		elog(DEBUG1, "Error sending acks from FluxCons: %d."
			 " Setting it to failed", fcinfo->conn_idx);
		elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
		set_failed_conn((Plan *) node, fcinfo, rval, fcinfo->conn_idx);
		return false;
	}
	
	return rval;
}

bool
sendAckToConn_FluxCons_FT(FluxCons_FT * node,
						  FluxConnInfo * fcinfo,
						  HeapTuple p_htup)
{
	int			seqnum;
	bool		isNull;
	
#define T_OVERHEAD
	
#ifdef T_OVERHEAD
	if (!node->isPair) return true;
#endif
	
	/* We have a pending heap tuple from the input now */
	seqnum = heap_getattr(p_htup, 1,
						  node->fcons.tupDesc,
						  &isNull);

	return sendIntAckToConn_FluxCons_FT(node, fcinfo, seqnum, false);
}

bool
sendIntAckToConn_FluxCons_FT(FluxCons_FT * node,
							 FluxConnInfo * fcinfo,
							 int num, bool flush)
{
	int rval;

	rval = fconn_send_ack(fcinfo, num, flush);

	if (rval < 0)
	{
		elog(DEBUG1, "Error sending acks from FluxCons: %d."
			 " Setting it to failed", fcinfo->conn_idx);
		elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
		set_failed_conn((Plan *) node, fcinfo, rval, fcinfo->conn_idx);
		return false;
	}
	
	return rval;
}

#ifdef TEST_INSTALL
static void
test_extract_and_install(FluxCons_FT * node)
{

	Plan	   *parent = ((Plan *) node)->parent;
	int			bsize = 8192;
	BufFile    *state_file;
	FILE	   *firstFile;
	FILE	   *secondFile;
	char	   *buf;
	int			numpages;

	buf = palloc(bsize);

	elog(DEBUG1, "Dumping data to FirstFile.");
	firstFile = fopen("FirstFile", "w+");
	ExecDump(parent, firstFile);
	fclose(firstFile);

	state_file = BufFileCreateTemp();

	elog(DEBUG1, "Getting state - init.");
	ExecGetStateInit(parent);

	numpages = 0;
	elog(DEBUG1, "Getting state - extract.");

	while (!ExecGetStateNext(parent, buf, bsize))
	{
		elog(DEBUG1, "Writing page %d", numpages);
		BufFileWrite(state_file, (void *) buf, bsize);
		numpages++;
	}

	/* Remember to write the last page */
	elog(DEBUG1, "Writing page %d", numpages);
	BufFileWrite(state_file, (void *) buf, bsize);

	/* Reset the state of operator */
	ExecResetState(parent);

	/* Reset the file pointer */
	BufFileSeek(state_file, 0, 0, SEEK_SET);

	elog(DEBUG1, "Installing state - init.");
	ExecInstallStateInit(parent);

	numpages = 0;
	elog(DEBUG1, "Installing state - install.");
	while (BufFileRead(state_file, (void *) buf, bsize))
	{
		elog(DEBUG1, "Read and installing %d", numpages);
		ExecInstallStateNext(parent, buf, bsize);
		numpages++;
	}

	elog(DEBUG1, "Installing state - close.");
	ExecInstallStateClose(parent);

	BufFileClose(state_file);

	elog(DEBUG1, "Dumping data to SecondFile.");
	secondFile = fopen("SecondFile", "w+");
	ExecDump(parent, secondFile);
	fclose(secondFile);

	elog(DEBUG1, "Exiting.");

	sleep(2000);
	proc_exit(1);
}
#endif

static void
received_resdone(FluxCons_FT * node, int fidx, int mask)
{
	FluxConnInfo *fcinfo;
	FluxConsState_FT *lfcstate = node->fcstate_ft;
	int sidx;

	switch (mask)
	{
		case (TSB_PRIMARY):
			sidx = lfcstate->map_p2s_m[fidx];
			fcinfo = &(lfcstate->conn_info_s[sidx]);
			fconn_ack_flush(fcinfo);
			break;
		case (TSB_SECONDARY):
			/* Nothing to flush, because primary should be dead */
			break;
	}
}


/* Dispatch routine for punctuation handlers */
static void
process_punct(FluxCons_FT *node, FluxConnInfo *fcinfo,
			  int fidx, int mask, result *p_res)
{
	switch (p_res->type) 
	{
		case (RESULT_FPRODSYNC):
#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons got an fprodsync from: %d mask: %d",
				 node->m_role, node->fcons.flux_idx, fidx, mask);
#endif
			received_fprodsync(node,
							   fidx,
							   p_res->data.info,
							   mask);
			break;

		case (RESULT_FPRODALIVE):
#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons got an fprodalive from: %d mask: %d",
				 node->m_role, node->fcons.flux_idx, fidx, mask);
#endif
			received_fprodalive(node,
								fidx,
								p_res->data.info,
								mask);
			break;

		case (RESULT_PAUSEACK):
#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons got a pause ack from: %d mask: %d",
				 node->m_role, node->fcons.flux_idx, fidx, mask);
#endif
			received_pauseack(node, fidx, mask);
						
			break;

		case (RESULT_DONE):
			received_resdone(node, fidx, mask);
			break;
			
		default:
			elog(ERROR, "Don't understand type: %d", p_res->type);
			break;
	}
}

static bool
service_fdest(FluxCons_FT *node, int pidx, int mask)
{
	FluxConsState_FT    *lfcstate = node->fcstate_ft;
	FluxConnInfo         *fcinfo;
	int                  fidx = (-1);
	result               res;
	bool                 isNull;
	
	if (mask == TSB_PRIMARY)
	{
		fidx = pidx;
		fcinfo = &(lfcstate->conn_info_p[fidx]);
	}
	else
	{
		fidx = lfcstate->map_p2s_m[pidx];
		fcinfo = &(lfcstate->conn_info_s[fidx]);
	}
	
	switch (fcinfo->status)
	{
		case (CONN_RECV_DATA):

			/* Already filled, nothing to bring in */
			if (lfcstate->merge.htup[pidx])
			{
				return false;
			}
			
			/* Empty we need to get something */
			
			/* Try and bring in a tuple */
			if (!getResultFromConn((Plan *) node,
								   fcinfo,
								   &res,
								   fidx))
				return false;

			/* We got a tuple */
			if (fcinfo->p_htup)
			{
				CMergeSetTup(lfcstate->merge, pidx,
							 fcinfo->p_htup,
							 heap_getattr(fcinfo->p_htup, 1,
										  node->fcons.tupDesc,
										  &isNull));
							 
#ifdef DRECOVERY
				elog(DEBUG1, "%d - %d: RECV- S: %d PART: %d CONN: %d",
					 node->m_role, node->fcons.flux_idx, lfcstate->merge.snum[pidx], pidx, fidx);
#endif
				
				/* If fed by secondary, or if not a pair no acking needed, so
				   the tuple is ready for consumption */

				/* XXX I don't need the SECONDARY check, because the
				   primary should be dead anyway or paused ? 
				   
				if ( (lfcstate->merge.ready[pidx]
					  = ((mask == TSB_SECONDARY) || (!(node->isPair)))) )
				*/
				if (!(node->isPair))
				{
					CMergeSetTupReady(lfcstate->merge, pidx);
				}
				
				fcinfo->p_htup = NULL; /* Make sure the tuple is NULL! */
				return true;
			}
			
			/* We have a punctuation */
			process_punct(node, fcinfo, fidx, mask, &res);
			return true;
			
			break;

		case (CONN_ACK_DATA):

			if (!process_acks_FluxCons_FT(node, fcinfo))
				return false;

			if (CMergeTupPending(lfcstate->merge, pidx))
			{
				if (!sendIntAckToConn_FluxCons_FT(node, fcinfo,
												  lfcstate->merge.snum[pidx],
												  false))
					return false;

#ifdef DRECOVERY
				elog(DEBUG1, "%d - %d: SACK- S: %d PART: %d, CONN: %d",
					 node->m_role, node->fcons.flux_idx, lfcstate->merge.snum[pidx], pidx, fidx);
#endif

				CMergeSetTupReady(lfcstate->merge, pidx);
			}

			return false; /* Didn't do anything if I just acked something */

			break;

		case (CONN_ACK_CHECK_DATA):

			/* I nested these to make them easier to read */
			/* Try and ack first. */
			if (process_acks_FluxCons_FT(node, fcinfo))
			{
				if (CMergeTupPending(lfcstate->merge, pidx))
				{
					if (sendIntAckToConn_FluxCons_FT(node, fcinfo,
													 lfcstate->merge.snum[pidx],
													 false))
					{
						CMergeSetTupReady(lfcstate->merge, pidx);
					}
				}
			}
			
			/* Try and bring in a punctuation. */
			if (!getResultFromConn((Plan *) node,
								   fcinfo,
								   &res,
								   fidx))
				return false;

			if (fcinfo->p_htup)
			{
				elog(ERROR, "Should never receive a tuple while acking.");
				return false;
			}

			/* We have a punctuation */
			process_punct(node, fcinfo, fidx, mask, &res);
			return true;

			break;

		case (CONN_RECOVER):

			/* We are acking sending because its still a standby. */

			/* XXX I don't think we need the TSB_SECONDARY check
			   because if it is the PRIMARY, it means the merge buffer
			   is being filled by the secondary, so it still needs the
			   ready to be set! */
			
			/* if ((mask == TSB_SECONDARY) && 
			   (!lfcstate->merge.ready[pidx]) &&
			   (lfcstate->merge.htup[pidx] != NULL)) */

			if (CMergeTupPending(lfcstate->merge, pidx))
			{
				CMergeSetTupReady(lfcstate->merge, pidx);
			}

			/* Just checking for punctuations */
			if (!getResultFromConn((Plan *) node,
								   fcinfo,
								   &res,
								   fidx))
				return false;

			/* We got a tuple */
			if (fcinfo->p_htup)
			{
				elog(ERROR, "Should never receive a tuple while CONN_RECOVER");
				return false;
			}
			
			/* We have a punctuation */
			process_punct(node, fcinfo, fidx, mask, &res);
			return true;
			
			break;

		case (CONN_PAUSE):

			/* XXX Why is the secondary pausing at all?  Because of a prod-sync
			   message! We could receive it before getting one from the primary.
			   So we don't want to ack that data to the secondary. */

			/* Not sure if I really need the check, but it shouldn't hurt to
			   leave it in. --- Nope, we *must not* have the check, because if
			   the secondary is feeding and the primary is paused (due to a
			   prod-sync) the tuples need to be set ready for consumption. */
			
			/* if ((mask == TSB_SECONDARY) && */
			
			if (CMergeTupPending(lfcstate->merge, pidx))
			{
				/* We are not sending because it is paused. */
				CMergeSetTupReady(lfcstate->merge, pidx);
			}
			
			return false;

			break;

		case (CONN_SEND_MARKER):
			/* Send a marker */
			if (!process_acks_FluxCons_FT(node, fcinfo))
				return false;

			if (!sendIntAckToConn_FluxCons_FT(node, fcinfo,
											  fcinfo->res.data.info,
											  true))
				return false;

#ifdef D_SEND_MARKER
			elog(DEBUG1, "%d - %d: FCons sent marker %d to conn: %d",
				 node->m_role, node->fcons.flux_idx, fcinfo->res.data.info, fidx);
#endif
			
			/* Now we actually process the data. */
			fcinfo->status = CONN_FLUSHBUF;
			break;

		case (CONN_FLUSHBUF):
			{
				int rval;

				rval = fconn_is_ack_flushed(fcinfo);

				if (rval < 0)
				{
					elog(DEBUG1, "service_fdest: Failed to send queue!");
					elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
					set_failed_conn((Plan *)node, fcinfo, rval, fidx);
					return true;
				}

				if (rval)
				{
					fcinfo->status = fcinfo->next_state;
					fcinfo->next_state = (-1);
				}
			}
			
			break;

		default:
			elog(ERROR, "%d - %d: FCons - can't handle connection state: %d",
				 node->m_role, node->fcons.flux_idx, fcinfo->status);
			break;
	}
	

	return true;
}

static void
serviceConn(FluxCons_FT *node)
{
	FluxConsState_FT 	*lfcstate = node->fcstate_ft;
	bool                 rvalid = false;
	int                  i = 0;

	while (!rvalid && (i < node->fcons.nProd))
	{
		if (node->isPair) 
		{
			/* Process the acks */
			rvalid |= service_fdest(node, lfcstate->curr_conn, TSB_SECONDARY);
		}
		
		/* Process incoming stream */
		rvalid |= service_fdest(node, lfcstate->curr_conn, TSB_PRIMARY);

		lfcstate->curr_conn =
			(lfcstate->curr_conn + 1) % (node->fcons.nProd);
		i++;
	}
}

/* Uses a buffer to store tuples when pauses have been sent */
static bool
getNextTuple_bord(FluxCons_FT *node, result *p_res)
{
	int i;
	int lsn = -1, tsn = -1, midx = -1;
	bool inbuf = false;
	FluxConsState_FT *fcstate = node->fcstate_ft;
	TupleTableSlot *slot = fcstate->csstate.css_ScanTupleSlot;

	if (!fcstate->merge.use_buf) 
	{
		return getNextTuple_ord(node, p_res);
	}

	/* Just copying stuff into the buffer until we receive
	   all the pauses */
	if (fcstate->merge.move_to_buf)
	{
		if (fcstate->merge.nfill == 0) return false;
							
		for (i = 0; i < node->fcons.nProd; i++)
		{
			if (CMergeTupReady(fcstate->merge, i))
			{
				ExecStoreTuple(fcstate->merge.htup[i],
							   slot,
							   InvalidBuffer,
							   false);
				
				put_ack_TSB(fcstate->tsb, slot,
							fcstate->merge.snum[i],
							i,
							TSB_PRODUCED,
							TSB_PRODUCED | TSB_PRIMARY);

				/* Oh no, no more space in the TSB! */
				if (!TupIsNull(slot)) 
				{
					elog(DEBUG1, "Out of space, cannot wait for all pauses! --- Sleeping ...");
					sleep(1000);
					elog(ERROR, "Waited too long.");
				}

				CMergeDelTup(fcstate->merge, i);
			}
		}

		return false;
	}
	
	/* Reading from the buf until they are all empty */

	/* Just do a linear search */
	midx = -1;

	for (i = 0; i < node->fcons.nProd; i++)
	{
		if (TSB_PRIM_PART_EMPTY(fcstate->tsb, i))
		{
			if (CMergeTupReady(fcstate->merge, i))
			{
				if ((midx == -1) ||
					(fcstate->merge.snum[i] < lsn))
				{
					lsn = fcstate->merge.snum[i];
					midx = i;
					inbuf = false;
				}
			}
			else
			{
				/* No tuple from this incoming partition */
				return false;
			}
		}
		else
		{
			tsn = get_unack_seqno_TSB(fcstate->tsb, i, TSB_PRIMARY);
			
			if ((midx == -1) || (tsn < lsn))
			{
				lsn = tsn;
				midx = i;
				inbuf = true;
			}
		}
	}

/* #define DEBUG_BUF_RET */

#ifdef DEBUG_TUPLES_UP
	if (fcstate->debug) 
	{
		elog(DEBUG1, "lsn: %d", lsn);
		for (i = 0; i < node->fcons.nProd; i++)
		{
			if (TSB_PRIM_PART_EMPTY(fcstate->tsb, i))
			{
				elog(DEBUG1, "i: %d, snum: %d - M", i,
					 fcstate->merge.snum[i]);
			}
			else
			{
				elog(DEBUG1, "i: %d, snum: %d - T", i,
					 get_unack_seqno_TSB(fcstate->tsb, i, TSB_PRIMARY));
			}
		}

		elog(DEBUG1, "xx merge list");
		
		for (i = 0; i < node->fcons.nProd; i++)
		{
			elog(DEBUG1, "i: %d, snum: %d", i,
				 CMergeTupReady(fcstate->merge, i) ?
				 fcstate->merge.snum[i]: (-1));
		}
	}
#endif
	
	if (inbuf)
	{
		get_head_TSB_r(fcstate->tsb, p_res, midx, TSB_PRIMARY);
		advance_head_TSB(fcstate->tsb, midx, TSB_PRIMARY);

#ifdef DEBUG_BUF_RET
		elog(DEBUG1, "%d - %d: FCons result-ptr: %x",
			 node->m_role, node->fcons.flux_idx, (int) p_res->data.tuple);
#endif
		
		if (!put_ack_TSB(fcstate->tsb, NULL, lsn, midx, TSB_PRIMARY,
						 TSB_PRODUCED | TSB_PRIMARY))
		{
			elog(ERROR, "Never should happen.");
		}
	}
	else
	{
		p_res->data.tuple = fcstate->merge.htup[midx];
		CMergeDelTup(fcstate->merge, midx);
	}

#ifdef DEBUG_BUF_RET
	elog(DEBUG1, "%d - %d: FCons returning snum: %d - inbuf %d",
		 node->m_role, node->fcons.flux_idx, lsn, inbuf);
#endif
	
	if (TSB_EMPTY(fcstate->tsb))
	{
		elog(DEBUG1, "%d - %d: FCons returning to normal merge.",
			 node->m_role, node->fcons.flux_idx);
		fcstate->merge.use_buf = false;
		/* move_to_buf should be false already */
	}

	return true;
}

static bool
getNextTuple_ord(FluxCons_FT *node, result *p_res)
{
	FluxConsState_FT *fcstate = node->fcstate_ft;
	int i = 1;
	int lsn, midx;

	/* All items must be filled, and positive. */
	if ((fcstate->merge.nfill != node->fcons.nProd) ||
		(node->fcons.nProd == 0))
		return false;

	/* Do a linear search */
	midx = 0;
	lsn = fcstate->merge.snum[midx];
	
	while (i < node->fcons.nProd)
	{
		if (fcstate->merge.snum[i] < lsn)
		{
			lsn = fcstate->merge.snum[i];
			midx = i;
		}
		i++;
	}

	p_res->data.tuple = fcstate->merge.htup[midx];

	CMergeDelTup(fcstate->merge, midx);

	return true;
}

#if 0
/* Just returns the next available tuple */
static bool
getNextTuple_unord(FluxCons_FT *node, result *p_res)
{
	FluxConsState_FT *fcstate = node->fcstate_ft;

	/* No items */
	if (fcstate->merge.nfill == 0) return false;

	while (!fcstate->merge.ready[fcstate->merge.cpos])
	{
		fcstate->merge.cpos = (fcstate->merge.cpos + 1) % ((FluxCons *) node)->nProd;
	}

	p_res->data.tuple = fcstate->merge.htup[fcstate->merge.cpos];

	CMergeDelTup(fcstate->merge, fcstate->merge.cpos);
	
	return true;
}
#endif /* 0 */

static TupleTableSlot *
do_normal(FluxCons_FT *node)
{
	FluxConsState_FT *lfcstate = node->fcstate_ft;
	TupleTableSlot *slot;
	result          res;

/* #define DEBUG_TUPLES_UP */
	
#ifdef DEBUG_TUPLES_UP

	int  seqnum;
	bool isNull;

#endif

	/* Service the connections */
	serviceConn(node);

	if (IsPushPlan(node) && parentPlan(node) && (!IsInputDone(parentPlan(node))))
	{
		/* Don't return a tuple if we have a parent plan */
		return NULL;
	}

	slot  = lfcstate->csstate.cstate.cs_ResultTupleSlot;

	ExecClearTuple(slot);

/* #define GET_UNORD */

#ifdef GET_UNORD
	/* Get a tuple from below */
	if(!getNextTuple_unord(node, &res))
#else
	/* This ensures that I do not miss deadlocks while testing ... funny eh? */
/* 	if(!getNextTuple_ord(node, &res)) */
	if (!getNextTuple_bord(node, &res))
#endif
	{
		return slot;
	}

	ExecStoreTuple(res.data.tuple, slot, InvalidBuffer, true);

#ifdef DEBUG_TUPLES_UP
	if (
/* 		(lfcstate->merge.use_buf) && */
/* 		(!lfcstate->merge.move_to_buf) && */
		node->fcstate_ft->debug
		)
	{
		seqnum = heap_getattr(slot->val, 1,
							  slot->ttc_tupleDescriptor, &isNull);
			
		elog(DEBUG1, "%d - %d: FCONS Returning a slot: SNUM %d", node->m_role, node->fcons.flux_idx, seqnum);
	}
#endif

	if (IsPushPlan(node) && parentPlan(node))
	{
		SetInputNew(parentPlan(node));
	}
	
	return slot;
}

static void
do_adjust_conn(FluxCons_FT *node)
{
	FluxConsState_FT   	*lfcstate = node->fcstate_ft;
	FluxConnInfo    	*fcinfo;
	int 				 fail_conn;

	fail_conn = ((FluxCons *) node)->map_m2c[which_failed];

	Assert(node->spare_m == node->fcons.nProd);
		   
	((FluxCons *) node)->map_c2m[fail_conn] = node->spare_m;
	((FluxCons *) node)->map_m2c[node->spare_m] = fail_conn;
		
	/* Close the connections */
	fcinfo = &(lfcstate->conn_info_p[fail_conn]);
/* 	close(fcinfo->fd); */
	ftrans_reset(fcinfo->trans);

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons closing prim conn: %d, mach: %d",
		 node->m_role, node->fcons.flux_idx, fail_conn, which_failed);
#endif
	
	lfcstate->conn_info_p[fail_conn] = *(lfcstate->conn_spare_p);
	lfcstate->conn_info_p[fail_conn].conn_idx = fail_conn;
	
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons new prim conn: %d, mach: %d",
		 node->m_role, node->fcons.flux_idx, fail_conn,
		 ((FluxCons *) node)->map_c2m[fail_conn]);
#endif

	fcinfo = &(lfcstate->conn_info_s[fail_conn]);
/* 	close(fcinfo->fd); */
	ftrans_reset(fcinfo->trans);

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons closing sec conn: %d, mach: %d",
		 node->m_role, node->fcons.flux_idx, fail_conn, which_failed);
#endif
	
	lfcstate->conn_info_s[fail_conn] = *(lfcstate->conn_spare_s);
	lfcstate->conn_info_s[fail_conn].conn_idx = fail_conn;

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons new sec conn: %d, mach: %d",
		 node->m_role, node->fcons.flux_idx, fail_conn,
		 ((FluxCons *) node)->map_c2m[fail_conn]);
#endif

	if (node->isStandby)
	{

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons is a standby so collecting syncs now.",
			 node->m_role, node->fcons.flux_idx);
#endif
		
		/* We are waiting for the prod-syncs now */
		node->mode = STANDBY_FAIL_OVER;
		lfcstate->rec_mode = WAIT_FPRODSYNC;
		return;
	}
	
	lfcstate->f_idx_s = lfcstate->map_p2s_m[fail_conn];
	/* 	f_idx_p = lfcstate->map_s2p_m[fail_conn]; */

	/* XXX I don't know what I was thinking below. For the failed primary
	   connection, we need to re-receive it from the secondary --- or ack it. */
	
	/* Cannot do this! Everything must be acked, otherwise,
	   the tsb on the secondary won't get it, it'll send it
	   and it'll get bounced to the primary, who does not
	   have it in the TSB, because the cursor was advanced */
	
/* #ifdef NOT_USED */
	/* If there is a tuple pending for acknowledgement, throw it
	   away. XXX We shoud free the tuple too. */
	if (CMergeTupPending(lfcstate->merge, fail_conn))
	{
		heap_freetuple(lfcstate->merge.htup[fail_conn]);
		lfcstate->merge.htup[fail_conn] = NULL;
		lfcstate->merge.ready[fail_conn] = false;
		lfcstate->merge.snum[fail_conn] = (-1);
		/* Since its pending, we don't need to remove it */
	}
	
/* #endif */

/*
    if (CMergeTupPending(lfcstate->merge, fail_conn))
	{
		elog(DEBUG1, "AHHHH ... sleeping.");
		sleep(1000);
	}
*/

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons conn: %d, latest snum: %d",
		 node->m_role, node->fcons.flux_idx, fail_conn, lfcstate->merge.snum[fail_conn]);
#endif
	
	fcinfo = &(lfcstate->conn_info_s[lfcstate->f_idx_s]);

	/* Send a marker indicating that my primary has failed */
	fcinfo->status = CONN_SEND_MARKER;
	fcinfo->res.data.info = ACK_MARKER;
	fcinfo->next_state = CONN_RECV_DATA;
			
	lfcstate->rec_mode = REVERSE_SCONN;
	return;
}

static void
do_check_reverse(FluxCons_FT *node)
{
	FluxConsState_FT *lfcstate = node->fcstate_ft;
	
	if (lfcstate->conn_info_s[lfcstate->f_idx_s].status == CONN_RECV_DATA)
	{
		
#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
		elog(DEBUG1,"%d - %d: FCons sent reverse marker to conn: %d",
			 node->m_role, node->fcons.flux_idx, lfcstate->f_idx_s);
#endif
#endif
		lfcstate->rec_mode = WAIT_FPRODSYNC;
	}
}

static void
do_send_pauses(FluxCons_FT *node)
{
	FluxConsState_FT   	*lfcstate = node->fcstate_ft;
	FluxConnInfo    	*fcinfo;
	int                  i;

#ifdef DRECOVERY
	struct timeval     ts, diff;
	double               dur;
#endif

/* #define STAGGER_RECOVERY */

/* 	return; */

#ifdef STAGGER_RECOVERY
	
	/* Only after 30 secs do take-over */
	if (node->instrument->tdur < (double) ((40.0) * 1e03) ) return;
#endif

	if (node->fprod_parent != NULL)
	{
		if (!ready_to_copy_FluxProd_FT(node->fprod_parent))
			return; /* Not yet ready to copy this dataflow segment over. */
	}

#ifdef DRECOVERY
	gettimeofday(&ts, NULL);

	timersub(&ts, &(node->instrument->counter), &diff);

	dur = ((double) diff.tv_sec * 1.0e3 +
		   (double) diff.tv_usec * 1.0e-3);

	elog(DEBUG1, "%d - %d: FCons catch-up started, ts: %f",
		 node->m_role, node->fcons.flux_idx, dur);
	
	elog(DEBUG1, "%d - %d: FCons sending pauses",
		 node->m_role, node->fcons.flux_idx);
#endif

	/*
	 * Make sure there are no connections that are still trying to send
	 * markers.
	 */

	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		fcinfo = &(lfcstate->conn_info_p[i]);

		/* Can't send pauses yet, wait until no markers need to be sent. */
		if (IsSending(fcinfo))
			return;
	}

	/* We are ready to send pauses to all our nodes. */
	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		/* Inititalize to none received */
		lfcstate->pauses.ack_recv[i] = false;
		fcinfo = &(lfcstate->conn_info_p[i]);
		smark_conn(fcinfo, fcinfo->status,
				   0, ACK_PAUSE);
	}

	lfcstate->rec_mode = WAIT_ACKS;
}

static void
do_signal_smover(FluxCons_FT *node)
{
	StateMover *sm = (StateMover *) ((FluxCons *) node)->sched->fluxen[STATE_MOVER_IDX];

	Assert(nodeTag(sm) == T_StateMover);

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons enqueing a request for statemovment",
		 node->m_role, node->fcons.flux_idx);
#endif
		 
	/* State movement request -- curr_smr -- is setup after all the acks are
	 * received */
	if (ENQUEUE_INT(sm->request_queue, &(node->curr_smr)))
		node->fcstate_ft->rec_mode = WAIT_SMOVER;
}

static void
do_fail_over(FluxCons_FT *node)
{
	FluxConsState_FT   	*lfcstate = node->fcstate_ft;
	
	switch (lfcstate->rec_mode)
	{
		case (ADJUST_CONN):
			do_adjust_conn(node);
			break;
			
		/* Check to see if I am done with reversing my failed connection. */
		case (REVERSE_SCONN):
			/* This is done here because in the future there may
			   be multiple primary producers that are dead. */
			do_check_reverse(node);
			break;
			
		case (WAIT_FPRODSYNC):

			/* Nothing to do --- I'll transition out when the last fprodsync is
			 * received. */
			
			break;

		case (SIGNAL_SMOVER):
			/* Enqueue request with the state mover */
			
			do_signal_smover(node);
			break;

		case (WAIT_SMOVER):
			/* Not ready for statemovement yet */

			if (node->ready_flag)
			{
				
#ifdef DRECOVERY
				elog(DEBUG1, "%d - %d: FCons statemover ready to receive.",
					 node->m_role, node->fcons.flux_idx);
#endif
				
				if (node->isStandby)
				{
					/* Standby is not getting data, does not need to pause
					 * producers. */
#ifdef DRECOVERY
					elog(DEBUG1, "%d - %d: FCons standby is ready to move.",
						 node->m_role, node->fcons.flux_idx);
#endif

					node->mode = STANDBY_TAKE_OVER;
					lfcstate->rec_mode = INIT_SM;
				}
				else
				{
					lfcstate->rec_mode = SEND_PAUSE;
				}
			}

			break;
			
		case (SEND_PAUSE):
			/* Send all pauses. */
			do_send_pauses(node);
			break;
			
		case (WAIT_ACKS):
			/* Check for pause acks. */
			/* Nothing to do here, just wait until they all arrive. */
			/* Transition out happens when I get all acks. */

			break;

		case (WAIT_TUP_ACKS):
			check_tup_acks(node);
			break;

		default:
			elog(ERROR, "do_fail_over: Don't understand recovery mode: %d",
				 lfcstate->rec_mode);
			break;
	}
}

/* XXX We should really merge this and do_take_over_1 in nodeSegCons because it
 * really is repeated code! */

static void
do_send_state(FluxCons_FT *node)
{
/* 	elog (DEBUG1, "%d: FCons send state", node->m_role); */
	Instrumentation *instr = node->instrument;
	
	if ((node->curr_q == NULL) && (!node->move_done))
	{
		make_queue(&(node->curr_q), *p_no_lock, sm_queue_size);
		
		if (!ExecGetStateNext((Plan *) node, node->curr_q->start,
							  sm_queue_size - QSTRUCTSIZE))
/* 		if (ExecGetStateNext(parentPlan(node), node->curr_q->start, */
/* 							 sm_queue_size - QSTRUCTSIZE)) */
		{
			instr->pages_moved++;
#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons Sending page", node->m_role, node->fcons.flux_idx);
#endif
		}
		else
		{
			
			elog(DEBUG1, "%d - %d: FCons moved: %d pages, %d bytes, %.3f ms",
				 node->m_role, node->fcons.flux_idx,
				 instr->pages_moved,
				 instr->pages_moved * sm_queue_size,
				 f_time_since(&(instr->move_start)));
			
			node->move_done = true;

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

	/* Yes */
	/* pfree(node->curr_smr); */
	node->curr_smr = NULL;

	ExecGetStateClose((Plan *) node);

#define SIGNAL_PARENT
	
#ifdef SIGNAL_PARENT
#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons signalling parent FProd.", node->m_role, node->fcons.flux_idx);
#endif
	
	if (node->fprod_parent)
	{
		/* Signal the parent */
		node->fprod_parent->sync = true;
	}
#endif

	((FluxCons_FT *) node)->fcstate_ft->rec_mode = SEND_SYNC;
	
	return;
}

static void
do_send_syncs(FluxCons_FT *node)
{
	FluxConsState_FT   	*lfcstate = node->fcstate_ft;
	FluxConnInfo    	*fcinfo;
	int                  i;

	/* Just in case, check to see if markers are being sent */
	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		fcinfo = &(lfcstate->conn_info_p[i]);

		if (IsSending(fcinfo))
			return;

		fcinfo = &(lfcstate->conn_info_s[i]);

		if (IsSending(fcinfo))
			return;
	}

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons sending syncs.", node->m_role, node->fcons.flux_idx);
#endif
	
	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		fcinfo = &(lfcstate->conn_info_p[i]);
		smark_conn(fcinfo, fcinfo->status,
				   0, ACK_FCONSSYNC);
	}

	for (i = 0; i < ((FluxCons *) node)->nProd; i++)
	{
		fcinfo = &(lfcstate->conn_info_s[i]);
		smark_conn(fcinfo, fcinfo->status,
				   0, ACK_FCONSSYNC);
	}

	elog(DEBUG1, "%d - %d: FCons going back to normal.", node->m_role, node->fcons.flux_idx);
	
	/* Reset the module */
	node->mode = FLUX_NORMAL;
	lfcstate->rec_mode = 0;
	node->isStandby = false; /* No longer a standby. */

	set_recovered_fluxen(((FluxCons *) node)->sched,
						 ((FluxCons *) node)->flux_idx);

	return;
}

static void
do_take_over(FluxCons_FT *node)
{
	FluxConsState_FT *lfcstate = node->fcstate_ft;

	switch (lfcstate->rec_mode)
	{
		case (INIT_SM):
/* 			ExecGetStateInit(parentPlan(node)); */
			ExecGetStateInit((Plan *) node);
			node->instrument->pages_moved = 0;
			gettimeofday(&(node->instrument->move_start), NULL);
			node->move_done = false;
			node->fcstate_ft->rec_mode = MOVE_STATE;
			break;

		case (MOVE_STATE):
			do_send_state(node);
			break;

		case (SEND_SYNC):
			do_send_syncs(node);
			break;

		default:
			elog(ERROR, "%d - %d: FCons -- do_take_over: "
				 "Don't understand recovery mode: %d",
				 lfcstate->rec_mode, node->m_role, node->fcons.flux_idx);
			break;
	}
}

/* Same code as take_over in SegCons */
static void
do_recv_state(FluxCons_FT *node)
{
/* 	elog(DEBUG1, "%d: FCons in do_recv_state", node->m_role); */
	Instrumentation *instr = node->instrument;
	
	if (node->curr_q == NULL)
	{
		if (!DEQUEUE_INT(node->curr_smr->state_q, &node->curr_q))
			return;

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons received another queue.",
			 node->m_role, node->fcons.flux_idx);
#endif

		/* End of packets */
		if (node->curr_q->size == QSTRUCTSIZE)
		{

			/* XXX Fix this --- doesn't work for FluxCons_FT */
#ifdef DEBUG_EDGES
			FluxConnInfo *fcinfo;
			fcinfo = &(((FluxCons_FT *) node)->fcstate_ft->conn_info_p[INGRESS_CONN]);
			fcinfo->tmp_seqno = 1;
#endif

			/* Free up our movement metadata structures. */
			free_queue(&(node->curr_q));
			node->curr_q = NULL;

			/* Since I'm last, I free the request. */
			pfree(node->curr_smr);
			node->curr_smr = NULL;

			/* Unfreeze the dataflow segment */
			ExecInstallStateClose((Plan *) node);

#ifdef SIGNAL_PARENT

#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons signalling parent FProd.",
				 node->m_role, node->fcons.flux_idx);
#endif
	
			if (node->fprod_parent)
			{
				/* Signal the parent */
				node->fprod_parent->sync = true;
			}
#endif

			/* We need to send syncs. */
			node->fcstate_ft->rec_mode = SEND_SYNC;

#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons finished with transfer request.",
				 node->m_role, node->fcons.flux_idx);
#endif
			
			elog(DEBUG1, "%d - %d: FCons received: %d pages, %d bytes, %.3f ms",
				 node->m_role, node->fcons.flux_idx,
				 instr->pages_moved,
				 instr->pages_moved * sm_queue_size,
				 f_time_since(&(instr->move_start)));
			
			instr->pages_moved = 0;

			return;
		}
	}

#ifdef DRECOVERY
	elog(DEBUG1, "Received and installing state");
#endif

	
/* 	ExecInstallStateNext(parentPlan(node), */
/* 						 node->curr_q->start, */
/* 						 sm_queue_size - QSTRUCTSIZE); */

	if (instr->pages_moved == 0)
	{
		elog(DEBUG1, "%d - %d: FCons time to first page: %.3f",
			 node->m_role, node->fcons.flux_idx,
			 f_time_since(&(instr->move_start)));
	}
	
	instr->pages_moved++;

	ExecInstallStateNext((Plan *) node, node->curr_q->start,
						 sm_queue_size - QSTRUCTSIZE);

	free_queue(&(node->curr_q));
	node->curr_q = NULL;
}

static void
do_standby_take_over(FluxCons_FT *node)
{
	FluxConsState_FT *lfcstate = node->fcstate_ft;

	switch (lfcstate->rec_mode)
	{
		case (INIT_SM):
/* 			ExecInstallStateInit(parentPlan(node)); */
			ExecInstallStateInit((Plan *) node);
			node->fcstate_ft->rec_mode = MOVE_STATE;
			node->instrument->pages_moved = 0;
			gettimeofday(&(node->instrument->move_start), NULL);
#ifdef DRECOVERY
			elog(DEBUG1, "%d - %d: FCons done install init.", node->m_role, node->fcons.flux_idx);
#endif
			break;

		case (MOVE_STATE):
			do_recv_state(node);
			break;

		case (SEND_SYNC):
			do_send_syncs(node);
			break;

		default:
			elog(ERROR, "do_take_over: Don't understand recovery mode: %d",
				 lfcstate->rec_mode);
			break;
	}
}

/*
 * ---------------------------------------
 *	 ExecFluxCons
 *
 *	 Does the consuming from the sockets.
 * ---------------------------------------
 */

#if 0
static void
process_stats(FluxCons_FT * node)
{
	struct timeval end_tv;
	struct timeval dur_tv;

	double		duration,
				bw;
	uint32		tuplen = 40;	/* For now */
	long		total_count;

	struct Instrumentation *instr = node->instrument;

/* 	if (instr->firsttuple == 0) */
/* 		return; */

	total_count = (long) (instr->firsttuple);

/* 	if (total_count < ((FluxProd *) node)->sched->ntups) return; */

	gettimeofday(&end_tv, NULL);

	timersub(&end_tv, &(instr->starttime), &dur_tv);

	duration = TV_IN_MSEC_DOUBLE(dur_tv);

	elog(DEBUG1, "%d - %d: FCons stats BEGIN ----", node->m_role, node->fcons.flux_idx);
	elog(DEBUG1, " %d tups ", ((FluxProd *) node)->sched->ntups * 10);
	elog(DEBUG1, " %ld total_count", total_count);
	elog(DEBUG1, " round %.0f ", instr->nloops);
	elog(DEBUG1, " %.0f tuples sent - primary. ", instr->firsttuple);
	elog(DEBUG1, " %.0f tuples sent - secondary. ", instr->tuplecount);
	elog(DEBUG1, " Duration %f ms ", duration);
	elog(DEBUG1, " tuplen %d ", tuplen);
	bw = ((double) (instr->firsttuple * tuplen) / (double) duration) * 1.0e3;
	elog(DEBUG1, " Primary BW %f MB/sec \n", bw / ((double) 1024 * 1024));
	bw = ((double) (instr->tuplecount * tuplen) / (double) duration) * 1.0e3;
	elog(DEBUG1, " Secondary BW %f MB/sec \n", bw / ((double) 1024 * 1024));
	elog(DEBUG1, " starttime sec: %ld usec: %ld", instr->starttime.tv_sec,
		 instr->starttime.tv_usec);
	elog(DEBUG1, " endtime sec: %ld usec: %ld", end_tv.tv_sec,
		 end_tv.tv_usec);
	elog(DEBUG1, " dur_time sec: %ld usec: %ld", dur_tv.tv_sec,
		 dur_tv.tv_usec);

	bw = ((double) (instr->firsttuple) / (double) duration) * 1.0e3;
	elog(DEBUG1, " KTuples per sec - Primary: %f ", bw / ((double) 1024));
	bw = ((double) (instr->tuplecount) / (double) duration) * 1.0e3;
	elog(DEBUG1, " KTuples per sec - Primary: %f ", bw / ((double) 1024));
	
	timersub(&end_tv, &(instr->counter), &dur_tv);

	duration = ((double) dur_tv.tv_sec * 1.0e3 +
				(double) dur_tv.tv_usec * 1.0e-3);

	elog(DEBUG1, " TOTAL_DUR sec: %f ms ", duration);
	
		 
	instr->nloops++;
	instr->firsttuple = 0;
	instr->tuplecount = 0;

	gettimeofday(&instr->starttime, NULL);

	elog(DEBUG1, " new_start sec: %ld usec: %ld", instr->starttime.tv_sec,
		 instr->starttime.tv_usec);

	elog(DEBUG1, "%d - %d: FCons stats END ----", node->m_role, node->fcons.flux_idx);

}
#endif /* 0 */

TupleTableSlot *
ExecFluxCons_FT(FluxCons_FT *node)
{
	TupleTableSlot *slot = NULL;

	if (!node->fcons.all_done)
	{
		check_conn_complete(node);
		return NULL;
	}

	if (IsPushPlan(node) && parentPlan(node))
	{
		ExecProcNode(parentPlan(node), (Plan *) node);
	}
	
	switch (node->mode)
	{
		case (FLUX_NORMAL):
			slot = do_normal(node);
			break;

		case (FLUX_FAIL_OVER):
			do_fail_over(node);
			slot = do_normal(node);
			break;

		case (FLUX_TAKE_OVER):
			do_take_over(node);
			break;

		case (STANDBY_NORMAL):
			slot = do_normal(node); /* Transition logic is received_fprodsync. */
			break;

		case (STANDBY_FAIL_OVER):
			do_fail_over(node);  /* Transitions logic in fail_over */
			slot = do_normal(node); /* Need to received syncs */
			break;

		case (STANDBY_TAKE_OVER):
			do_standby_take_over(node);
			break;
			  
		default:
			elog(ERROR, "Cannot handle failure mode.");
			break;
	}

#define COLLECT_STATS

#ifdef COLLECT_STATS
	if (!TupIsNull(slot))
	{
		node->instrument->firsttuple++;
	}
#endif
	
	return slot;
}

/* -----------------
 * ExecInitFluxCons
 *
 * -----------------
 */
bool
ExecInitFluxCons_FT(FluxCons_FT * node, EState *estate, Plan *parent)
{
	int			i;
	FluxConsState_FT *lfcstate;
	FluxConnInfo *fcinfo;

/* #define DEBUG_MAP */
	
#ifdef DEBUG_MAP
	elog(DEBUG1, "FCONS: My mode: %d, my role: %d - %d ",
		 node->mode, node->m_role, node->fcons.flux_idx);
	elog(DEBUG1, "FCONS: SADDR: %x ", (uint) node->fcons.map_m2c);
	for (i = 0; i < node->fcons.nProd + 1; i++)
	{
		elog(DEBUG1, "map_m2c: %d -> %d", i, node->fcons.map_m2c[i]);
	}
#endif

	node->last_seqnum = -1;

#ifdef COLLECT_STATS
	elog(DEBUG1,"Collecting stats.");
	node->instrument = InstrAlloc();
	InstrStartNode(node->instrument);
	gettimeofday(&(node->instrument->counter), NULL);
#endif

	node->fcons.plan.state = estate;

	lfcstate = makeNode(FluxConsState_FT);

	lfcstate->debug = false;

	node->fcstate_ft = lfcstate;

	/* Create buffer for state-movement. XXX Eventually this must go to disk */
	lfcstate->tsb = palloc(sizeof(TSB));
	init_TSB(lfcstate->tsb, node->fcons.nProd, tsb_size);
	lfcstate->tsb->shouldFree = false; /* We are returning tuples up, cannot free it */
		
#define FLUXCONSFT_NSLOTS 2

	/* I need this slot to fill the incoming buffer */
	ExecInitScanTupleSlot(estate, &(lfcstate->csstate));
	
	/*
	 * tuple table initialization
	 */
	ExecInitResultTupleSlot(estate, &(lfcstate->csstate.cstate));

	ExecAssignResultType(&(lfcstate->csstate.cstate), node->fcons.tupDesc, true);
/* 	ExecAssignResultTypeFromTL(node, &(lfcstate->csstate.cstate)); */
/* 	node->fcons.tupDesc = lfcstate->csstate.cstate->cs_ResultTupleSlot; */
	
	lfcstate->conn_info_p = (FluxConnInfo *)
		palloc(sizeof(FluxConnInfo) * node->fcons.nProd);

	lfcstate->conn_info_s = NULL;

	lfcstate->num_conn_p = 0;
	lfcstate->curr_conn = 0;

	for (i = 0; i < node->fcons.nProd; i++)
	{
		fcinfo = &((lfcstate->conn_info_p)[i]);
		fconn_init_empty(fcinfo);
		fcinfo->conn_idx = i;
		fcinfo->role = F_PRIMARY;
	}

	if (IsPushPlan(node))
	{
		ExecInitNode(parentPlan(node), estate, (Plan *) node);
		SetInputDone(node);
	}

	init_mergeCB(&(lfcstate->merge), node->fcons.nProd);

	if (!node->isPair)
		return TRUE;

	lfcstate->map_p2s_m = palloc(sizeof(int) * node->fcons.nProd);
	lfcstate->map_s2p_m = palloc(sizeof(int) * node->fcons.nProd);

	for (i = 0; i < node->fcons.nProd; i++)
	{
		if (node->m_role == F_PRIMARY)
		{
			lfcstate->map_p2s_m[i] = ROTATE_R(i, node->fcons.nProd);
			lfcstate->map_s2p_m[i] = ROTATE_L(i, node->fcons.nProd);
		}
		else
		{
			lfcstate->map_p2s_m[i] = ROTATE_L(i, node->fcons.nProd);
			lfcstate->map_s2p_m[i] = ROTATE_R(i, node->fcons.nProd);
		}
	}

	lfcstate->map_p2m_p = palloc(sizeof(int) * node->fcons.nProd);
	lfcstate->map_m2p_p = palloc(sizeof(int) * node->fcons.nProd);

	for (i = 0; i < node->fcons.nProd; i++)
	{
		if (node->m_role == F_PRIMARY)
		{
			lfcstate->map_p2m_p[i] = i;
			lfcstate->map_m2p_p[i] = i;
		}
		else
		{
			lfcstate->map_p2m_p[i] = ROTATE_R(i, node->fcons.nProd);
			lfcstate->map_m2p_p[i] = ROTATE_L(i, node->fcons.nProd);
		}
	}
	
	lfcstate->conn_info_s = (FluxConnInfo *)
		palloc(sizeof(FluxConnInfo) * node->fcons.nProd);

	lfcstate->num_conn_s = 0;

	for (i = 0; i < node->fcons.nProd; i++)
	{
		fcinfo = &((lfcstate->conn_info_s)[i]);
		fconn_init_empty(fcinfo);
		fcinfo->conn_idx = i;
		fcinfo->role = F_SECONDARY;
	}

	/* Inititalize spare connections */
	lfcstate->conn_spare_p = palloc(sizeof(FluxConnInfo));
	fconn_init_empty(lfcstate->conn_spare_p);
	lfcstate->conn_spare_p->conn_idx = SPARE_CONN_IDX;
	lfcstate->conn_spare_p->role     = F_PRIMARY;
	
	lfcstate->conn_spare_s = palloc(sizeof(FluxConnInfo));
	fconn_init_empty(lfcstate->conn_spare_s);
	lfcstate->conn_spare_s->conn_idx = SPARE_CONN_IDX;
	lfcstate->conn_spare_s->role     = F_SECONDARY;
	
	init_snapshotCB(&(lfcstate->snapshot), node->fcons.nProd);
	init_syncCB(&(lfcstate->syncs), node->fcons.nProd);
	init_pauseCB(&(lfcstate->pauses), node->fcons.nProd);
					
#ifdef DEBUG_MAP
	elog(DEBUG1, "FCONS: My mode: %d, my role: %d - %d ",
		 node->mode, node->m_role, node->fcons.flux_idx);
	elog(DEBUG1, "FCONS: SADDR: %x ", (uint) node->fcons.map_m2c);
	for (i = 0; i < node->fcons.nProd + 1; i++)
	{
		elog(DEBUG1, "map_m2c: %d -> %d", i, node->fcons.map_m2c[i]);
	}
#endif
	
			 
#ifdef PRINT_CONN
	print_connections(node);
#endif

	lfcstate->stateMoveCB = palloc(sizeof(FProdSMCB));
	
	return TRUE;
}

int
ExecCountSlotsFluxCons_FT(FluxCons_FT * node)
{
	if (IsPushPlan(node))
	{
		return ExecCountSlotsNode(parentPlan(node)) +
			FLUXCONSFT_NSLOTS;
	}
	
	return FLUXCONSFT_NSLOTS;
}

/* Nothing to do here */
void
ExecReScanFluxCons_FT(FluxCons_FT * node, ExprContext *exprCtxt, Plan *parent)
{
	return;
}

/* -----------------------
 *	ExecEndFluxCons(node)
 * -----------------------
 */

void
ExecEndFluxCons_FT(FluxCons_FT * node)
{
	int					 i;
	FluxConsState_FT 	*lfcstate = node->fcstate_ft;

	if (IsPushPlan(node))
	{
		ExecEndNode(parentPlan(node), (Plan *) node);
	}
	
	for (i = 0; i < node->fcons.nProd; i++)
	{
		ftrans_reset(lfcstate->conn_info_p[i].trans);
	}

	if (!node->isPair) return;
	
	for (i = 0; i < node->fcons.nProd; i++)
	{
		ftrans_reset(lfcstate->conn_info_s[i].trans);
	}
}

/* Debugging routines */

void
print_connections(FluxCons_FT * node)
{

	int			i;

	elog(DEBUG1, "Printing primary connections");

	for (i = 0; i < node->fcons.nProd; i++)
	{
		elog(DEBUG1, "Conn: %d", i);
		fconn_print_info(&(node->fcstate_ft->conn_info_p[i]));
	}

	elog(DEBUG1, "Conn: Spare");
	fconn_print_info(node->fcstate_ft->conn_spare_p);

	elog(DEBUG1, "Printing secondary connections");

	for (i = 0; i < node->fcons.nProd; i++)
	{
		elog(DEBUG1, "Conn_s: %d", i);
		fconn_print_info(&(node->fcstate_ft->conn_info_s[i]));
	}

	elog(DEBUG1, "Conn: Spare");
	fconn_print_info(node->fcstate_ft->conn_spare_s);

	elog(DEBUG1, "------------------------------");
}

/* State Movement Functions */
enum CopiedState
{
	NOTHING_COPIED = 0,
	MERGE_COPIED,
	TSB_MD_COPIED
};

static int *tmasks;
static bool *copy_heads;

bool ExecGetStateInitFluxCons_FT(FluxCons_FT * node)
{
	int i;

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons get state init.",
		 node->m_role, node->fcons.flux_idx);
#endif

	node->fcstate_ft->parentDone = false;

	node->fcstate_ft->stateMoveCB->copied_md = NOTHING_COPIED;

	tmasks = palloc(sizeof(int) * node->fcons.nProd);
	copy_heads = palloc(sizeof(bool) * node->fcons.nProd);

	for (i = 0; i < node->fcons.nProd; i++)
	{
		tmasks[i] = (TSB_PRODUCED | TSB_PRIMARY);
		copy_heads[i] = TRUE;
	}
		
	return ExecGetStateInit(parentPlan(node));
}

bool ExecGetStateNextFluxCons_FT(FluxCons_FT * node,
								 char *buf, int bsize)
{
	int 				 nconn 	  = ((FluxCons *) node)->nProd;
	FluxConsState_FT  	*lfcstate = node->fcstate_ft;
	CMerge 				*cm 	  = &(lfcstate->merge);
	int 				 i;
	int 				 size 	  = 0;
	int 				 curr_pos = 0;
	FProdSMCB           *smcb     = lfcstate->stateMoveCB;
	
	if (!node->fcstate_ft->parentDone)
	{
		if (!ExecGetStateNext(parentPlan(node), buf, bsize))
			return FALSE;
		
		node->fcstate_ft->parentDone = true;
		return FALSE;
	}


	if (smcb->copied_md == NOTHING_COPIED)
	{

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons copying merge buffer.",
			 node->m_role, node->fcons.flux_idx);
#endif
	
		/* Waring, I assume that all tuples can fit onto a single page. Otherwise,
		 * this extraction routing will fail! It will indicate an error; however. */

		/* Determine the size of the merge buffer first */
		size += sizeof(cm->nfill);
		
		for (i = 0; i < nconn; i++)
		{
			if (cm->ready[i]) 
			{
				size += sizeof(int)*2; /* Sequence number and index */
				size += tuple_serializer.determineSize((char *) cm->htup[i],
													   NULL);
			}
		}

		if (!(size < bsize))
		{
			elog(ERROR, "%d - %d: FCons not enough space on "
				 "page to serialize the merge buffer.",
				 node->m_role, node->fcons.flux_idx);
		}

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons filling %d tups.",
			 node->m_role, node->fcons.flux_idx, cm->nfill);
#endif

		/* Map the entries to partitions, not the connections */
		permute_cmerge(&(lfcstate->merge), lfcstate->map_m2p_p, nconn);
		
		memcpy(buf + curr_pos, &(cm->nfill), sizeof(cm->nfill));
		curr_pos += sizeof(cm->nfill);
	
		for (i = 0; i < nconn; i++)
		{
			if (cm->ready[i])
			{
				memcpy(buf + curr_pos, &i, sizeof(int));
				curr_pos += sizeof(int);

			
				tuple_serializer.serialize((char *) cm->htup[i],
										   buf + curr_pos,
										   bsize - curr_pos,
										   NULL);
			
				curr_pos += tuple_serializer.determineSize((char *) cm->htup[i],
														   NULL);
			}
		}

		/* Map the entries back to connections */
		permute_cmerge(&(lfcstate->merge), lfcstate->map_p2m_p, nconn);
		
		smcb->copied_md = MERGE_COPIED;

		return FALSE;
	}

	if (smcb->copied_md == MERGE_COPIED)
	{
		/* Permute the TSB to correspond to partitions, not connections */
		permute_tsb_parts(lfcstate->tsb, lfcstate->map_m2p_p);
		
		serTSBMetaDataG(smcb, lfcstate->tsb, NULL, tmasks,
						copy_heads, copy_heads, buf, bsize,
						"FCons", node->m_role);

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons before get state",
			 node->m_role, node->fcons.flux_idx);
		dump_seq_TSB(lfcstate->tsb);
		elog(DEBUG1, "%d - %d: FCons end-tsb",
			 node->m_role, node->fcons.flux_idx);
#endif

		smcb->curr_part = 0;
		smcb->cursor = NULL;
		smcb->copied_md = TSB_MD_COPIED;
		return FALSE;
	}

	if (serTSBTuples(smcb, lfcstate->tsb, tmasks, buf, bsize, "FCons", node->m_role))
	{
		/* Permute the TSB back to connections */
		permute_tsb_parts(lfcstate->tsb, lfcstate->map_p2m_p);
		return TRUE;
	}

	return FALSE;
}

bool ExecGetStateCloseFluxCons_FT(FluxCons_FT * node)
{
	pfree(tmasks);

	pfree(copy_heads);

	/*
	if ((node->fcons.flux_idx > 4) &&
		(node->fcons.flux_idx < 9))
	{
		node->fcstate_ft->debug = true;
	}
	*/
	
	return ExecGetStateClose(parentPlan(node));
}

bool ExecInstallStateInitFluxCons_FT(FluxCons_FT * node)
{
	int i;

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FCons install state init.",
		 node->m_role, node->fcons.flux_idx);
#endif
	
	node->fcstate_ft->parentDone = false;

	node->fcstate_ft->stateMoveCB->copied_md = NOTHING_COPIED;

	tmasks = palloc(sizeof(int) * node->fcons.nProd);

	for (i = 0; i < node->fcons.nProd; i++)
	{
		tmasks[i] = (TSB_PRODUCED | TSB_PRIMARY);
	}

	return ExecInstallStateInit(parentPlan(node));
}

bool ExecInstallStateNextFluxCons_FT(FluxCons_FT * node,
									 char *buf, int bsize)
{
	int 				 nconn 	  = ((FluxCons *) node)->nProd;
	FluxConsState_FT  	*lfcstate = node->fcstate_ft;
	CMerge 				*cm 	  = &(lfcstate->merge);
	int 				 i;
	int 				 curr_pos = 0;
	int                  idx;
	FProdSMCB           *smcb     = lfcstate->stateMoveCB;
	
	if (!node->fcstate_ft->parentDone)
	{
		if (!ExecInstallStateNext(parentPlan(node), buf, bsize))
			return FALSE;

		node->fcstate_ft->parentDone = true;
		return FALSE;
	}


	if (smcb->copied_md == NOTHING_COPIED)
	{

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons install merge buffer.",
			 node->m_role, node->fcons.flux_idx);
#endif

		/* The merge control block should be initialized already */

		memcpy(&(cm->nfill), buf + curr_pos, sizeof(cm->nfill));
		curr_pos += sizeof(cm->nfill);

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons installing %d tups.",
			 node->m_role, node->fcons.flux_idx, cm->nfill);
#endif

		for (i = 0; i < cm->nfill; i++)
		{
			memcpy(&idx, buf + curr_pos, sizeof(int));
			curr_pos += sizeof(int);

			Assert((idx < nconn) && (idx >= 0));
		
			tuple_serializer.deserialize(buf + curr_pos,
										 (char **) &(cm->htup[idx]),
										 NULL);

			curr_pos += tuple_serializer.determineSize((char *)
													   cm->htup[idx],
													   NULL);
		
			cm->ready[i] = true;
		}

		/* Map it to connections */
		permute_cmerge(&(lfcstate->merge), lfcstate->map_p2m_p, nconn);
		
		smcb->copied_md = MERGE_COPIED;
		return FALSE;
	}
	
	if (smcb->copied_md == MERGE_COPIED)
	{
		deserTSBMetaDataG(smcb, lfcstate->tsb, tmasks, NULL, FALSE,
						  buf, bsize, "FCons", node->m_role);
		smcb->copied_md = TSB_MD_COPIED;
		smcb->curr_part = 0;
		return FALSE;
	}

	if (deserTSBTuples(smcb, lfcstate->tsb, FALSE, buf, bsize, "FCons", node->m_role))
	{
#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FCons adjusting pointers and we're almost done.",
			 node->m_role, node->fcons.flux_idx);
#endif
	
		adjustTSBPointers(smcb, lfcstate->tsb, tmasks, "FCons", node->m_role);

		permute_tsb_parts(lfcstate->tsb, lfcstate->map_p2m_p);

		/* We are done */
		return TRUE;
	}
	
	return FALSE;
}

static void
swap_merge_loc(CMerge *cmerge, int a, int b)
{
	HeapTuple tmp_tup;
	seq_num   tmp_sn;
	bool      tmp_bool;

	tmp_tup = cmerge->htup[a];
	cmerge->htup[a] = cmerge->htup[b];
	cmerge->htup[b] = tmp_tup;

	tmp_sn = cmerge->snum[a];
	cmerge->snum[a] = cmerge->snum[b];
	cmerge->snum[b] = tmp_sn;

	tmp_bool = cmerge->ready[a];
	cmerge->ready[a] = cmerge->ready[b];
	cmerge->ready[b] = tmp_bool;
}

static void
permute_cmerge(CMerge *cmerge, int *perm_map, int size)
{
	int i;
	int curr_idx[size];
	int tmp, dloc;

	for (i = 0; i < size; i++)
	{
		curr_idx[i] = i;
	}

	for (i = 0; i < size; i++)
	{
		dloc = perm_map[curr_idx[i]];
		
		while (i != dloc)
		{
			swap_merge_loc(cmerge, i, dloc);
			
			/* Swap */
			tmp = curr_idx[i];
			curr_idx[i] = curr_idx[dloc];
			curr_idx[dloc] = tmp;

			dloc = perm_map[curr_idx[i]];
		}
	}
}

bool ExecInstallStateCloseFluxCons_FT(FluxCons_FT * node)
{
	pfree(tmasks);
	
	ExecInstallStateClose(parentPlan(node));

	node->fcstate_ft->merge.use_buf = true;
	node->fcstate_ft->merge.move_to_buf = false;

#ifdef DRECOVERY
	elog(DEBUG1, "%d - %d: FProd install state close",
		 node->m_role, node->fcons.flux_idx);
#endif

	/*
	if ((node->fcons.flux_idx > 4) &&
		(node->fcons.flux_idx < 9))
	{
		node->fcstate_ft->debug = true;
	}
	*/
	
	return TRUE;
}

bool ExecResetStateFluxCons_FT(FluxCons_FT * node)
{
	return TRUE;
}

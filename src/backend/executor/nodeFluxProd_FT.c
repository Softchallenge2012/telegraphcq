/*-------------------------------------------------------------------------
 *
 * nodeFluxProd_FT.c
 *	  The producer side of the Flux operator.
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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxProd_FT.c,v 1.34 2004/07/15 20:39:54 owenc Exp $
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

#include "executor/nodeFluxAcceptor.h"	/* I need
										 * ExecInsertConnectionFluxAcceptor
										 *	*/
#include "executor/instrument.h"
#include "telegraphcq/fluxroutines.h"

#include "executor/nodeFSched.h"
#include "executor/nodeSegProd.h"

bool serTSBMetaDataG(FProdSMCB *smcb,
					 TSB *tsb,
					 TupleTableSlot *scanslot,
					 int * fmasks,
					 bool * copy_p_head,
					 bool * copy_s_head,
					 char * buf,
					 int bsize,
					 char * debug_name,
					 FluxRole m_role);

bool serTSBTuples(FProdSMCB *smcb, TSB * tsb, int * fmasks,
				  char * buf, int bsize,
				  char * debug_name, FluxRole m_role);

bool deserTSBMetaDataG(FProdSMCB * smcb, TSB * tsb, int * fmasks,
					   TupleTableSlot *scanslot, bool flip_mask,
					   char * buf, int bsize,
					   char * debug_name, FluxRole m_role);

bool deserTSBTuples(FProdSMCB * smcb, TSB * tsb, bool flip_mask,
					char * buf, int bsize,
					char * debug_name, FluxRole m_role);

void
adjustTSBPointers(FProdSMCB * smcb, TSB * tsb, int * fmasks,
				  char * debug_name, FluxRole m_role);

/*
 * Message handlers
 *
 * Ack handlers.
 *
 */
static void
received_frunning(FluxProd_FT *node, int part_no, bool mask)
{
	FluxConnInfo 	*fcinfo, *fcinfo_o;
	int 				 dest;
	FluxProdState_FT *fpstate = node->fpstate_ft;


	if (IsA(node, SegProd))
	{
		received_frunning_SegProd(node, part_no, mask);
		return;
	}

#ifdef DRECOVERY
	/* Turn on the connection and set the bit. */
	elog(DEBUG1, "%d: FProd received a frunning, part_no: %d, mask: %d",
		 node->m_role, part_no, mask);
#endif

	fpstate->fmasks[part_no] |= mask; /* Turn on the mask for it ... shoud be on
									   * already. */

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd new part_no: %d, del_mask: %d",
		 node->m_role, part_no, fpstate->fmasks[part_no]);
#endif

	/* Can't be in SEND_MARKER because we received data */
	switch (mask)
	{
		case (TSB_PRIMARY):
			dest   = fpstate->map_p2m_p[part_no];
			fcinfo = &(fpstate->conn_info_p[dest]);
			dest   = fpstate->map_p2m_s[part_no];
			fcinfo_o = &(fpstate->conn_info_s[dest]);
			
			if (IsAlive(fcinfo_o))
			{
				fcinfo->status = CONN_SEND_DATA;
			}
			else
			{
				fcinfo->status = CONN_SEND_ACK_DATA;
			}
			
			break;
			
		case (TSB_SECONDARY):
			dest   = fpstate->map_p2m_s[part_no];
			fcinfo = &(fpstate->conn_info_s[dest]);
			dest   = fpstate->map_p2m_p[part_no];
			fcinfo_o = &(fpstate->conn_info_p[dest]);

			fcinfo->status = CONN_ACK_DATA;

			if (IsAlive(fcinfo_o))
			{
				/* If sending a marker */
				if (IsSending(fcinfo_o))
				{
					fcinfo_o->next_state = CONN_SEND_DATA;
				}
				else
				{
					fcinfo_o->status = CONN_SEND_DATA;
				}
			}
			
			break;
	}
}

static void
received_fstandby(FluxProd_FT *node, int part_no, bool mask)
{
	FluxConnInfo 	*fcinfo;
	int 				 dest;
	FluxProdState_FT *fpstate = node->fpstate_ft;
	int                  del_mask = fpstate->fmasks[part_no];

#ifdef DRECOVERY
	/* Just turn on the connection and unset the bit */
	elog(DEBUG1, "%d: FProd received a fstandby, part_no: %d, mask: %d",
		 node->m_role, part_no, mask);
#endif

	/* Mark all done with the del_mask for the standby */
	ack_all_TSB(fpstate->tsb, part_no, mask, del_mask);

#ifdef DRECOVERY
	dump_seq_TSB_part(fpstate->tsb, part_no);
#endif
		
	del_mask &= ~mask; /* Turn off the bit because its not ready */
	
	fpstate->fmasks[part_no] = del_mask;

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd new part_no: %d, del_mask: %d",
		 node->m_role, part_no, fpstate->fmasks[part_no]);
#endif

	/* Can't be in SEND_MARKER because we received data */
	switch (mask)
	{
		case (TSB_PRIMARY):

			dest   = fpstate->map_p2m_p[part_no];
			fcinfo = &(fpstate->conn_info_p[dest]);
			fcinfo->status = CONN_RECOVER;
			break;
			
		case (TSB_SECONDARY):
			dest   = fpstate->map_p2m_s[part_no];
			fcinfo = &(fpstate->conn_info_s[dest]);
			fcinfo->status = CONN_RECOVER;
			break;
	}
}

static void
received_fconssync(FluxProd_FT *node, int part_no, bool mask)
{
	FluxProdState_FT 	*fpstate  = node->fpstate_ft;
	int 				 del_mask = fpstate->fmasks[part_no];

	FluxConnInfo 		*fcinfo;
	int 				 dest;
	

#ifdef DRECOVERY
	/* Turn on the connection and set both bits */
	elog(DEBUG1, "%d: FProd received an fconssync, part_no: %d, mask %d",
		 node->m_role, part_no, mask);
#endif

	switch (mask)
	{
		/* Primary connection is repaired */
		case (TSB_PRIMARY):
			dest   = fpstate->map_p2m_p[part_no];
			fcinfo = &(fpstate->conn_info_p[dest]);

			
			/* Got a sync from the secondary */
			if (fpstate->syncs.sync_recv_s[part_no])
			{
				fcinfo->status = CONN_SEND_DATA;

				ResetSync(fpstate->syncs, part_no);
			}
			else
			{
				/* Pause the primary while this is happening */
				fpstate->syncs.sync_recv_p[part_no] = true;
				fcinfo->status = CONN_PAUSE;
			}

			/*
			  Cannot reset cursors here because there may be leftover acks from
			  the secondary. If so, its okay because we are not turning on the
			  primary. When we do turn on the primary, its because we got a sync
			  from the secondary already, so the cursors are already reset.
			*/
			
			break;
			
		case (TSB_SECONDARY):
			dest   = fpstate->map_p2m_s[part_no];
			fcinfo = &(fpstate->conn_info_s[dest]);

			fcinfo->status = CONN_ACK_DATA;

			if (fpstate->syncs.sync_recv_p[part_no])
			{
				/* We need unpause the primary if we received a sync from it */
				dest = fpstate->map_p2m_p[part_no];
				fcinfo = &(fpstate->conn_info_p[dest]);
				
				fcinfo->status = CONN_SEND_DATA;

				ResetSync(fpstate->syncs, part_no);
			}
			else
			{
				fpstate->syncs.sync_recv_s[part_no] = true;
			}

			/* Reset the cursors, so we don't lose anything in the TSB */

			/* Just a check */
			if (del_mask == (TSB_PRODUCED | TSB_PRIMARY | TSB_SECONDARY))
			{
				elog(DEBUG1, "%d: FProd --- del_mask full, why are we getting syncs? ",
					 node->m_role);
				sleep(10000);
				elog(ERROR, "Waited too long ... dying");
			}

			if (del_mask & TSB_PRIMARY)
			{
				fpstate->fmasks[part_no] |= TSB_SECONDARY;
				/* Primary was alive, reset secondary --- its new. */
				reset_cursors_TSB(fpstate->tsb, part_no, TSB_SECONDARY);
			}
			else
			{
				fpstate->fmasks[part_no] |= TSB_PRIMARY;
				/* Secondary was alive, reset primary --- its new. */
				reset_cursors_TSB(fpstate->tsb, part_no, TSB_PRIMARY);
			}

			/* Another check */
			del_mask = fpstate->fmasks[part_no];
			if (del_mask != (TSB_PRODUCED | TSB_PRIMARY | TSB_SECONDARY))
			{
				elog(DEBUG1, "%d: FProd --- del_mask not full: %d ",
					 node->m_role, del_mask);
				sleep(10000);
				elog(ERROR, "Waited too long ... dying");
			}
			
			break;
	}
	
	return;
}

static void
received_ackmarker(FluxProd_FT *node, int part_no, bool mask)
{
	FluxProdState_FT    *lfpstate = node->fpstate_ft;
	FluxConnInfo 		*fcinfo;
	int 				 dest;

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd received ackmarker, part_no: %d", node->m_role, part_no);
#endif
	
	/* Only the secondary sends an ACK marker,
	   to reverse the methods sent. */
	if (mask & TSB_PRIMARY) 
	{
		elog(DEBUG1, "Don't know how to handle markers from primary");
		return;
	}

	dest = lfpstate->map_p2m_s[part_no];
	fcinfo = &(lfpstate->conn_info_s[dest]);

#define DEBUG_MARKERS
	
#ifdef DEBUG_MARKERS
#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd received reverse marker for part: %d from: %d",
		 node->m_role, part_no, dest);
	elog(DEBUG1, "%d: FProd sending to conn: %d starting from seq: %d",
		 node->m_role, dest, get_unack_seqno_TSB(lfpstate->tsb,
												 part_no,
												 mask));
	elog(DEBUG1, "%d: FProd previous seqno: %d", node->m_role, fcinfo->tmp_seqno);
#endif
#endif
	
	/* Uncomment this if you are sending pauses to primaries only */
	fcinfo->status = CONN_SEND_DATA;

	/* Uncomment this if you are expecting to send pauses to secondaries as well */
	/* fcinfo->error = CONN_SEND_ACK_DATA;  */

	/* We must reset the head to the ack, because the head was not being
	   maintained and now needs to be. */
	reset_head_TSB(lfpstate->tsb, part_no, mask);
}

static void
received_pause(FluxProd_FT *node, int part_no, bool mask)
{
	FluxConnInfo 	*fcinfo;
	int 				 dest;

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd received pause, part_no: %d, mask: %d",
		 node->m_role, part_no, mask);
#endif
	
	switch (mask)
	{
		case (TSB_PRIMARY):
			dest = node->fpstate_ft->map_p2m_p[part_no];
			fcinfo = &(node->fpstate_ft->conn_info_p[dest]);
			break;

		case (TSB_SECONDARY):
			dest = node->fpstate_ft->map_p2m_s[part_no];
			fcinfo = &(node->fpstate_ft->conn_info_s[dest]);
			break;
	}

	fcinfo->res.data.info = (-1);
	fcinfo->res.type = RESULT_PAUSEACK;
	fcinfo->status   = CONN_SEND_MARKER;
	fcinfo->next_state = CONN_PAUSE; /* Wait for sync to arrive for unpausing. */
}

/*
 * Returns false if not successfully done processing acks
 */
bool
process_acks_FluxProd_FT(FluxProdState_FT *fpstate,
						 FluxConnInfo *fcinfo,
						 int part_id, int mask)
{
	int rval;
	int seqnum;
	int del_mask = fpstate->fmasks[part_id];

	if (fcinfo->prev_seqno == -1) 
	{
		rval = fconn_recv_ack(fcinfo, &seqnum);
	} 
	else 
	{  /*
		* Have a pending sequence number to handle
		*/
		rval = true;
		seqnum = fcinfo->prev_seqno;
		fcinfo->prev_seqno = -1;
	}

	if (rval < 0) return (-1);
	
	/*
	 * We have bunch of acks that we need to process
	 */
	while (rval)
	{
		if (seqnum < 0) 
		{
			return seqnum;
		}

		if (put_ack_TSB(fpstate->tsb, NULL, seqnum,
						part_id, mask, del_mask))
		{
			fcinfo->tmp_seqno = seqnum;
			rval = fconn_recv_ack(fcinfo, &seqnum);
		} 
		else 
		{
			fcinfo->prev_seqno = seqnum;
			rval = false;
		}

		if (rval < 0) return (-1);
	}

	/*
	 * If queue was not emptied, we'll have a prev_seqno != -1, returns false.
	 */
	return (fcinfo->prev_seqno == -1);
}

/*
 * Make connection for the ConnInfo in conn_no, and telegraph_nodeid conn_no
 * Returns 0 if it needs to continue with the same conn_no later.
 * Returns 1 if it succeeded.
 * Returns -1 if it failed;
 *
 * Primary is the current node, secondary is the opposite node.  To
 * mirror indicates wether the connection is to the secondary or not.
 */

static int
make_new_conn(FluxProd_FT * node, int conn_no, bool toMirror)
{
	FluxProdState_FT 	*lfpstate;
	FluxConnInfo 	*fcinfo;
	int					 to_idx;
	int					 rval;
	int 				 to_mach;

	/* Not dealing with errors while making connections */
	lfpstate = node->fpstate_ft;

	if (!toMirror)
	{
		fcinfo = (conn_no == SPARE_CONN_IDX) ? lfpstate->conn_spare_p :
			&((lfpstate->conn_info_p)[conn_no]);
		to_idx = node->fprod.fcons_idx;
	}
	else
	{
		fcinfo = (conn_no == SPARE_CONN_IDX) ? lfpstate->conn_spare_s :
			&((lfpstate->conn_info_s)[conn_no]);
		to_idx = node->fcons_idx_s;
	}

	to_mach = (conn_no == SPARE_CONN_IDX) ? node->spare_m : node->fprod.map_c2m[conn_no];

	if (to_mach == UNKNOWN_MACHINE)
	{
		/* Don't know what to connect to. */
		fcinfo->status = CONN_DO_NOTHING;
		return true;
	}
	else if (vmach_2_tnodeid(to_mach) != telegraph_nodeid)
	/* Its a remote connection */
	{
		rval = make_remote_conn((FluxProd *) node, to_mach, fcinfo, to_idx);

		if (rval > 0)
		{
			fcinfo->status =
				(node->isStandby) ? CONN_UNKNOWN :
				( (conn_no == SPARE_CONN_IDX) ? CONN_RECOVER : CONN_NORMAL );

			if (!toMirror)
			{
				elog(DEBUG1, "Made forward connection --");
			}
			else
			{
				elog(DEBUG1, "Made reverse connection --");
			}
		}

		return rval;
	}
	else
	{
		/* Insert the connection into corresponding FluxCons */
		ExecInsertConnectionFluxAcceptor(node->fprod.sched->faccept, -1,
										 node->fprod.node_id, to_idx,
										 node->fprod.flux_idx);

		/* Initialize our local queue communication structures			  */
		/* Sizes don't matter for local queues, since they are never sent */
		/* A (-1) value for sockfd indicates machine local queues		  */
		if (!toMirror)
		{
/* 			init_conn_info(fcinfo, -1, node->fprod.local_q, 0, node->local_r, 0); */
			fcinfo->trans = newLocalTrans_q(node->fprod.local_q, node->local_r);
		}
		else
		{
/* 			init_conn_info(fcinfo, -1, node->local_q_s, 0, node->local_r_s, 0); */
			fcinfo->trans = newLocalTrans_q(node->local_q_s, node->local_r_s);
		}
		
		fcinfo->status =
			(node->isStandby) ? CONN_UNKNOWN :
			( (conn_no == SPARE_CONN_IDX) ? CONN_RECOVER : CONN_NORMAL );

		return true;
	}
}

static void
do_fail_over(FluxProd_FT * node)
{
	FluxConnInfo  		*fcinfo;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int 				 fail_conn;
	int 				 del_mask;
	int 				 dest;
	int 				 f_part_p;
	int 				 f_part_s;
	
	/* Just switch over the connections for failed machines. */
	fail_conn = ((FluxProd *) node)->map_m2c[which_failed];

	/* XXX Sorry this is a hack */
/* 	Assert(node->spare_m == node->fprod.nCons); */
	if (fail_conn == UNKNOWN_CONN_IDX)
	{
		/* Nothing for me to send */
		if (node->isStandby)
		{
			node->mode = STANDBY_NORMAL;
		}
		else
		{
			node->mode = FLUX_TAKE_OVER;
		}

		return;
	}
	
	((FluxProd *) node)->map_c2m[fail_conn] = node->spare_m;
	((FluxProd *) node)->map_m2c[node->spare_m] = fail_conn;

	fcinfo = &(lfpstate->conn_info_p[fail_conn]);
/* 	close(fcinfo->fd); */
	ftrans_reset(fcinfo->trans);
		
#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd closing prim socket: %d, conn: %d, mach: %d",
		 node->m_role, fcinfo->fd, fail_conn, which_failed);
#endif
	
	lfpstate->conn_info_p[fail_conn] = *(lfpstate->conn_spare_p);
	lfpstate->conn_info_p[fail_conn].conn_idx = fail_conn;
	
#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd new prim socket: %d conn: %d, mach: %d",
		 node->m_role, 
		 lfpstate->conn_info_p[fail_conn].fd,
		 fail_conn,
		 ((FluxProd *) node)->map_c2m[fail_conn]);
#endif

	fcinfo = &(lfpstate->conn_info_s[fail_conn]);

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd closing sec socket: %d, conn: %d, mach: %d",
		 node->m_role, fcinfo->fd, fail_conn, which_failed);
#endif
	
/* 	close(fcinfo->fd); */
	ftrans_reset(fcinfo->trans);
	
	lfpstate->conn_info_s[fail_conn] = *(lfpstate->conn_spare_s);
	lfpstate->conn_info_s[fail_conn].conn_idx = fail_conn;
	
#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd new sec socket: %d conn: %d, mach: %d",
		 node->m_role, 
		 lfpstate->conn_info_s[fail_conn].fd,
		 fail_conn,
		 ((FluxProd *) node)->map_c2m[fail_conn]);
#endif

	if (node->isStandby)
	{
#ifdef DRECOVERY
		elog(DEBUG1, "%d: FProd is standby, returning to normal standby.",
			 node->m_role);
#endif
		node->mode = STANDBY_NORMAL;
		return;
	}
	
	f_part_p = lfpstate->map_m2p_p[fail_conn];
	f_part_s = lfpstate->map_m2p_s[fail_conn];

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd failed parts: P: %d S: %d",
		 node->m_role, f_part_p, f_part_s);

	elog(DEBUG1, "%d: FProd (before) part %d SEQNOS P: %d S:%d PR: %d",
		 node->m_role, f_part_p,
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_p, TSB_PRIMARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_p, TSB_SECONDARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_p, TSB_PRODUCED));
	

	elog(DEBUG1, "%d: FProd BEF: f_part_p: %d fmask: %d",
		 node->m_role, f_part_p, lfpstate->fmasks[f_part_p]);
#endif
	
	/* Adjust the delete masks for the failures for the partitions */
	del_mask = lfpstate->fmasks[f_part_p];
	/* Primary is dead for this partition */
	ack_all_TSB(lfpstate->tsb, f_part_p, TSB_PRIMARY, del_mask);
	/* Turn off requirement of having primary */
	del_mask &= ~TSB_PRIMARY; 
	lfpstate->fmasks[f_part_p] = del_mask;

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd AFT: f_part_p: %d fmask: %d",
		 node->m_role, f_part_p, lfpstate->fmasks[f_part_p]);

	elog(DEBUG1, "%d: FProd (after) part %d SEQNOS P: %d S:%d PR: %d",
		 node->m_role, f_part_p,
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_p, TSB_PRIMARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_p, TSB_SECONDARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_p, TSB_PRODUCED));

	elog(DEBUG1, "%d: FProd (before) part %d SEQNOS P: %d S:%d PR: %d",
		 node->m_role, f_part_s,
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_s, TSB_PRIMARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_s, TSB_SECONDARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_s, TSB_PRODUCED));

	elog(DEBUG1, "%d: FProd BEF: f_part_s: %d fmask: %d",
		 node->m_role, f_part_s, lfpstate->fmasks[f_part_s]);
#endif

	del_mask = lfpstate->fmasks[f_part_s];
	/* Secondary is dead for this partition */
	ack_all_TSB(lfpstate->tsb, f_part_s, TSB_SECONDARY, del_mask);
	/* Turn off requirement of having secondary */
	del_mask &= ~TSB_SECONDARY;
	lfpstate->fmasks[f_part_s] = del_mask;

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd AFT: f_part_s: %d fmask: %d",
		 node->m_role, f_part_s, lfpstate->fmasks[f_part_s]);


	elog(DEBUG1, "%d: FProd (after) part %d SEQNOS P: %d S:%d PR: %d",
		 node->m_role, f_part_s,
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_s, TSB_PRIMARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_s, TSB_SECONDARY),
		 get_unack_seqno_TSB(lfpstate->tsb, f_part_s, TSB_PRODUCED));
#endif

	/* Send the sync messages to both copies */
	dest   = lfpstate->map_p2m_p[f_part_s];
	fcinfo = &(lfpstate->conn_info_p[dest]);
	fcinfo->res.data.info = node->spare_m;
	fcinfo->res.type = RESULT_FPRODALIVE;
	fcinfo->status = CONN_SEND_MARKER;
	fcinfo->next_state = CONN_SEND_ACK_DATA;

	fcinfo = &(lfpstate->conn_info_s[fail_conn]);
	fcinfo->res.data.info = ((FluxProd *) node)->map_c2m[dest];
	fcinfo->res.type = RESULT_FPRODALIVE;
	fcinfo->status = CONN_SEND_MARKER;
	fcinfo->next_state = CONN_RECOVER;
	
	/* Tell the secondary to copy over to the new place */
	dest   = lfpstate->map_p2m_s[f_part_p];
	fcinfo = &(lfpstate->conn_info_s[dest]);
	fcinfo->res.data.info = node->spare_m;
	fcinfo->res.type = RESULT_FPRODALIVE;
	fcinfo->next_state = fcinfo->status;
	fcinfo->status = CONN_SEND_MARKER;
	
	/* Tell the standby, where the secondary is ... */
	fcinfo = &(lfpstate->conn_info_p[fail_conn]);
	fcinfo->res.data.info = ((FluxProd *) node)->map_c2m[dest];
	fcinfo->res.type = RESULT_FPRODALIVE;
	fcinfo->status = CONN_SEND_MARKER;
	fcinfo->next_state = CONN_RECOVER;
	
	node->mode = FLUX_TAKE_OVER;
}

static void
do_take_over(FluxProd_FT *node)
{
	FluxProdState_FT *fpstate = ((FluxProd_FT *) node)->fpstate_ft;
	FluxConnInfo *fcinfo_p, *fcinfo_s;
	int fail_conn;

	if (IsA(node, SegProd))
	{
		do_take_over_SegProd((SegProd *) node);
		return;
	}

	fail_conn = ((FluxProd *) node)->map_m2c[which_failed];

	fcinfo_p  = &(fpstate->conn_info_p[fail_conn]);
	fcinfo_s  = &(fpstate->conn_info_s[fail_conn]);

	/*
	 * If the failed connections are back to normal, we have received the
	 * FCONSSYNC message and our FPRODSYNC has been sent. We are done with
	 * take-over from this producing end.
	 */

	if ((fcinfo_p->status == CONN_SEND_DATA) &&
		(fcinfo_s->status == CONN_ACK_DATA))
	{
#ifdef DRECOVERY
		elog(DEBUG1,"%d: FProd: going back to normal mode.", node->m_role);
#endif
		node->mode = FLUX_NORMAL;
		set_recovered_fluxen(((FluxProd *) node)->sched,
							 ((FluxProd *) node)->flux_idx);
	}
}

static void
do_prod_sync(FluxProd_FT *node)
{
	FluxConnInfo 	*fcinfo_p, *fcinfo_s;
	FluxProdState_FT 	*fpstate = node->fpstate_ft;
	int 				 i;
	int                  p_dest, s_dest;
	int                  cstate_p, cstate_s;
	bool                 test_p, test_s;

#ifdef DRECOVERY
	elog(DEBUG1, "Sending prod sync!");
#endif

	/* Sorry, wish I had C++ and virtual functions */
	if (IsA(node, SegProd))
	{
		do_prod_sync_SegProd((SegProd *) node);
		return;
	}
		
	for (i = 0; i < ((FluxProd *) node)->nCons; i++)
	{
		p_dest   = fpstate->map_p2m_p[i];
		s_dest   = fpstate->map_p2m_s[i];

		fcinfo_p = &(fpstate->conn_info_p[p_dest]);
		fcinfo_s = &(fpstate->conn_info_s[s_dest]);
		
		fcinfo_p->next_state = fcinfo_p->status;
		fcinfo_s->next_state = fcinfo_s->status;
		
		cstate_p = fcinfo_p->status;
		cstate_s = fcinfo_s->status;
		
		test_p   = (cstate_p == CONN_RECOVER) || (cstate_p == CONN_UNKNOWN);
		test_s   = (cstate_s == CONN_RECOVER) || (cstate_s == CONN_UNKNOWN);
		
		/*
		 * If all the connections exist, or all are recovering we send no
		 * movement instructions.
		 */
		if (((test_p) && (test_s)) || ((!test_p) && (!test_s)))
		{
			fcinfo_p->res.data.info = (-1);
			fcinfo_s->res.data.info = (-1);
		}
		else
		{
			/* Otherwise, we need to tell it where to move the data. */
			fcinfo_p->res.data.info =
				((FluxProd *) node)->map_c2m[s_dest];
			fcinfo_s->res.data.info =
				((FluxProd *) node)->map_c2m[p_dest];
		}

		/* Need to ensure that we process ACKs if take over is supposed to
		   happen. */
		if (test_s && (cstate_p == CONN_SEND_DATA))
		{
			fcinfo_p->next_state = CONN_SEND_ACK_DATA;
		}

		/* If we were sending data before, now the data is coming back from our
		 * secondary */
		if ((cstate_s == CONN_SEND_DATA) ||
			(cstate_s == CONN_SEND_ACK_DATA))
		{
			fcinfo_s->next_state = CONN_ACK_DATA;
		}

		fcinfo_p->res.type = RESULT_FPRODSYNC;
		fcinfo_s->res.type = RESULT_FPRODSYNC;
		
		fcinfo_p->status = CONN_SEND_MARKER;
		fcinfo_s->status = CONN_SEND_MARKER;

#ifdef DRECOVERY
		elog(DEBUG1, "%d - %d: FProd sending sync to prim, conn: %d, ps: %d",
			 node->m_role, node->fprod.flux_idx,
			 p_dest, fcinfo_p->tmp_seqno);
		
		elog(DEBUG1, "%d - %d: FProd sending sync to sec, conn: %d, ps: %d",
			 node->m_role, node->fprod.flux_idx,
			 s_dest, fcinfo_s->tmp_seqno);
#endif
		
	}

/* 	elog(DEBUG1, "%d: FProd going back to normal", node->m_role); */
	node->sync = false;

	if (node->isStandby)
	{
		node->mode = FLUX_NORMAL;
		node->isStandby = false; /* No longer a standby */
	}
}

/* Returns true if we can copy the state of this dataflow segment */
bool
ready_to_copy_FluxProd_FT(FluxProd_FT *node)
{
	FluxConnInfo 		*fcinfo_p, *fcinfo_s;
	FluxProdState_FT 	*fpstate = node->fpstate_ft;
	int                  p_dest, s_dest;
	
	int i;

#ifdef DRECOVERY
	struct timeval ts, diff;
	double dur;
#endif

/* #define STAGGER_RECOVERY */

/* 	return false; */
	
#ifdef STAGGER_RECOVERY
	
/* 	return false; */
	
	/* Only after 20 secs do take-over */
 	if (node->instrument->tdur < (double) ((30.0) * 1e03) ) return false;
#endif

#ifdef DRECOVERY
	gettimeofday(&ts, NULL);

	timersub(&ts, &(node->instrument->counter), &diff);

	dur = ((double) diff.tv_sec * 1.0e3 +
		   (double) diff.tv_usec * 1.0e-3);


	elog(DEBUG1, "%d: FProd catch-up test, ts: %f", node->m_role, dur);
#endif

	if (IsA(node, SegProd))
	{
		return ready_to_copy_SegProd((SegProd *) node);
	}

	for (i = 0; i < ((FluxProd *) node)->nCons; i++)
	{
		p_dest   = fpstate->map_p2m_p[i];
		s_dest   = fpstate->map_p2m_s[i];

		fcinfo_p = &(fpstate->conn_info_p[p_dest]);
		fcinfo_s = &(fpstate->conn_info_s[s_dest]);

		/*
		 * Make sure markers have been sent -- we cannot handle multiple markers
		 * being sent at the same time.
		 */

		if (IsSending(fcinfo_p) || IsSending(fcinfo_s))
		{
			return false; 
		}
		
		/*
		 * Make sure all ACK streams from partner are turned into forward
		 * streams; otherwise, fail-over is not complete.

		 *
		 * This is being called because my partner is dead, so secondary
		 * connections better be sending data --- otherwise failover is
		 * incomplete. 
		 *
		 
		 */

		if ( IsAlive(fcinfo_s) &&
			 (fcinfo_s->status == CONN_ACK_DATA))
		{
			return false;
		}
	}

#ifdef DRECOVERY
	elog(DEBUG1, "%d: FProd catch-up ready, ts: %f", node->m_role, dur);
#endif

	return true;
}

static bool
check_all_done(FluxProd_FT * node)
{
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int					 conn_no;

	/* Check to see if I have made all my connections */
	if (!node->isPair)
	{
		node->fprod.all_done = (lfpstate->num_conn_p ==
								node->fprod.nCons);
	}
	else
	{
		node->fprod.all_done =
			(lfpstate->num_conn_p == node->fprod.nCons) &&
			(lfpstate->num_conn_s == node->fprod.nCons) &&
			(!FC_DCONN(lfpstate->conn_spare_p)) &&		/* Error is ok? */
			(!FC_DCONN(lfpstate->conn_spare_s));		/* Error is ok? */
	}

	if (node->fprod.all_done)
	{
		elog(DEBUG1, "nodeFluxProd on %d with idx %d: all done connecting.",
			 node->fprod.node_id, node->fprod.flux_idx);

		/* Need to set secondary connections for ACKs */
		/* Sorry for the hack, but this is the only place to put this now */
		if (IsA(node, FluxProd_FT) &&
			(!node->isStandby))
		{
			elog(DEBUG1, "Setting up primary connections for sending DATA");
			for (conn_no = 0; conn_no < node->fprod.nCons; conn_no++)
			{
				lfpstate->conn_info_p[conn_no].status = CONN_SEND_DATA;
			}

			if (node->isPair)
			{
				elog(DEBUG1, "Setting up secondary connections for ACKS");
				for (conn_no = 0; conn_no < node->fprod.nCons; conn_no++)
				{
					lfpstate->conn_info_s[conn_no].status = CONN_ACK_DATA;
				}
			}
		}
		else if (IsA(node, SegProd) &&
				 (!node->isStandby))
		{
			init_conn_stat_SegProd((SegProd *) node);
		}
		

	}

	return node->fprod.all_done;
}

void
make_connections_FluxProd_FT(FluxProd_FT * node)
{
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int					 conn_no;

	/* Check for completeness, if so return */
	if (check_all_done(node)) return;
	
	/* Make connections to primary partitions */
	if (lfpstate->num_conn_p != node->fprod.nCons)
	{
		conn_no = lfpstate->num_conn_p;
		if (make_new_conn(node, conn_no, false) != 0)
		{
			lfpstate->num_conn_p++;
		}
	}

	if (!node->isPair) return;

	/* Make connections to secondary partitions */
	if (lfpstate->num_conn_s != node->fprod.nCons)
	{
		conn_no = lfpstate->num_conn_s;
		if (make_new_conn(node, conn_no, true) != 0)
		{
			lfpstate->num_conn_s++;
		}
	}

	/* Make connections to spare machines */
	if (FC_DCONN(lfpstate->conn_spare_p))
	{
		make_new_conn(node, SPARE_CONN_IDX, false);
	}
	
	if (FC_DCONN(lfpstate->conn_spare_s))
	{
		make_new_conn(node, SPARE_CONN_IDX, true);
	}
}

/* Returns the next tuple from operators below in the slot */
bool
getNextTuple_FluxProd_FT(Plan *node, TupleTableSlot *rslot)
{
	TupleTableSlot *outerslot;

	if (!IsPushPlan(node))
	{
		outerslot = ExecProcNode(outerPlan(node), node);
	}
	else
	{
		/* The outerslot is used for the return tuple from below
		   in push-based plans */
		
		if (IsInputDone(node)) return false;
		
		outerslot =
			((FluxProd_FT *) node)->fpstate_ft->csstate.cstate.cs_OuterTupleSlot;
	}
	
	if (TupIsNull(outerslot))
	{
		SetInputDone(node);
		return false;
	}

	/* Store it in the scan tuple slot */
	ExecStoreTuple(outerslot->val, rslot,
				   outerslot->ttc_buffer,
				   outerslot->ttc_shouldFree);

	/*
	 * Take ownership, hope this works, because it is not the recommended
	 * thing to do.
	 */
	outerslot->ttc_shouldFree = false;
	
	SetInputDone(node);
	return true;
}

static bool
service_fdest(FluxProd_FT *node, int pidx, int mask)
{
	FluxConnInfo 	*fcinfo;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int 				 dest;
	result 				 res;
	bool 				 part_empty;
	bool 				 rvalid   = false;
	int  				 rval     = false;
	int                  del_mask = lfpstate->fmasks[pidx];
	int                  seqnum;
	double               *counter;

	switch (mask)
	{
		case TSB_PRIMARY:
			dest       = lfpstate->map_p2m_p[pidx];
			fcinfo     = &(lfpstate->conn_info_p[dest]);
			part_empty = TSB_PRIM_PART_EMPTY(lfpstate->tsb, pidx);
			counter    = &(node->instrument->firsttuple);
			break;

		case TSB_SECONDARY:
			dest       = lfpstate->map_p2m_s[pidx];
			fcinfo     = &(lfpstate->conn_info_s[dest]);
			part_empty = TSB_SEC_PART_EMPTY(lfpstate->tsb, pidx);
			counter    = &(node->instrument->tuplecount);
			break;
	}

	switch (fcinfo->status)
	{
		case (CONN_SEND_DATA):
		case (CONN_SEND_ACK_DATA):
			if (!part_empty)
			{
				get_head_TSB_r(lfpstate->tsb, &res, pidx, mask);
			
				/*
				 * You need the tuple is null check, because we could have an
				 * ack in the TSB that has not yet been produced from nodes
				 * below.
				 */
				if (!ResIsNull(&res))
				{
					
					rval = fconn_send_result(fcinfo, &res, false);
					
					if (rval > 0)
					{
						(*counter)++;
						advance_head_TSB(lfpstate->tsb, pidx, mask);

						/*
						 * Advance the ack pointer too since we only need to receive
						 * acks from the pair.
						 */
						seqnum = get_unack_seqno_TSB(lfpstate->tsb, pidx, mask);
						put_ack_TSB(lfpstate->tsb, NULL, seqnum, pidx, mask, del_mask);

						fcinfo->tmp_seqno = seqnum;
							
/* #define DEBUG_TUPS */
						
#ifdef DEBUG_TUPS
						elog(DEBUG1, "%d: STUP S: %d PART: %d", node->m_role,
							 seqnum, pidx);
#endif
					}
					else
					{
						if (rval < 0) 
						{
							elog(DEBUG1, "service_fdest: Failed to send result tuple!");
							elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
							set_failed_conn((Plan *)node, fcinfo, rval, dest);
							return true; /* Something happened, should yield. */
						}
					}
				}
			}
			else
			{
				/* We want everything to flush if we are running out of space. */
				if (TSB_LAST_ENT_EMPTY(lfpstate->tsb, pidx))
				{
					res.type = RESULT_DONE;
					
					rval = fconn_send_result(fcinfo, &res, true);

					if (rval < 0)
					{
						elog(DEBUG1, "service_fdest: Failed to send queue!");
						elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
						set_failed_conn((Plan *)node, fcinfo, rval, dest);
						return true;
					}
				}

				/* Check to see if the queue needs to be flushed */
				rval = fconn_is_flushed(fcinfo);
				
				if (rval < 0) 
				{
					elog(DEBUG1, "service_fdest: Failed to send queue!");
					elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
					set_failed_conn((Plan *)node, fcinfo, rval, dest);
					return true;
				}
			}
			
			rvalid = rval;

			/* Must break unless we need to check acks. */
			if (fcinfo->status != CONN_SEND_ACK_DATA)
			{
				break;
			}

		case (CONN_ACK_DATA):
		case (CONN_RECOVER):
		case (CONN_PAUSE):
		case (CONN_UNKNOWN):

			rval = process_acks_FluxProd_FT(lfpstate, fcinfo, pidx, mask);

			if (rval < 0)
			{
				switch (rval)
				{
					case (-1): /* error */
						elog(DEBUG1, "service_fdest: Error receiving acks from "
							 "FluxCons: %d. Setting it to failed", dest);
						elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
						set_failed_conn((Plan *)node, fcinfo, rval, dest);
						return true;  /* Must yield */
						break;

					case (ACK_MARKER):
						received_ackmarker(node, pidx, mask);
						break;
					
					case (ACK_FCONSSYNC):
						received_fconssync(node, pidx, mask);
						break;

					case (ACK_PAUSE):
						received_pause(node, pidx, mask);
						break;

					case (ACK_FRUNNING):
						received_frunning(node, pidx, mask);
						break;

					case (ACK_FSTANDBY):
						received_fstandby(node, pidx, mask);
						break;

					default:
						elog(ERROR, "Don't understand the code.");
						break;
				}
				
			}

			rvalid = rval;
			break;

		case (CONN_SEND_MARKER):

			/* Need to flush this */
			rval = fconn_send_result(fcinfo, &(fcinfo->res), true);
			
			if (rval > 0)
			{

/* #ifdef DEBUG_MARKERS */
/* #ifdef DRECOVERY */
				if (IsA(node, SegProd))
				{
					elog(DEBUG1,
						 "%d: FProd sent marker: %d, to conn: %d, mask: %d, ps: %d",
						 node->m_role, fcinfo->res.type, pidx, mask,
						 fcinfo->tmp_seqno);
				}
/* #endif */
/* #endif */
				fcinfo->status = CONN_FLUSHBUF;
			}
			else
			{
				if (rval < 0)
				{
					elog(DEBUG1, "service_fdest: Failed to send result tuple!");
					elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
					set_failed_conn((Plan *)node, fcinfo, rval, dest);
					return true; /* Something happened, should yield. */
				}
			}
			
			rvalid = rval;
			break; /* CONN_SEND_MARKER */
				
		case (CONN_FLUSHBUF):

			/* Check to see if the queue needs to be flushed */
			rval = fconn_is_flushed(fcinfo);
			
			if (rval < 0) 
			{
				elog(DEBUG1, "service_fdest: Failed to send queue!");
				elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
				set_failed_conn((Plan *)node, fcinfo, rval, dest);
				return true;
			}

			if (rval)
			{
				if (IsA(node, SegProd))
				{
					elog(DEBUG1, "%d: FProd flushed marker", node->m_role);
				}
					
				fcinfo->status = fcinfo->next_state;
				fcinfo->next_state = (-1);
			}

			rvalid = rval;
	
			break; /* CONN_FLUSHBUF */

		case (CONN_DO_NOTHING):
			break;
			
		default:
			elog(DEBUG1, "We have a state that I don't handle! ...");
			sleep(1000);
			elog(ERROR, "Exiting ...");
			rvalid = false;
			break;
	}
	return rvalid;
}

static void
serviceConn(FluxProd_FT *node)
{
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	bool 				 rvalid   = false;
	int  				 i 		  = 0;
	
	while (!rvalid && (i < node->fprod.nCons)) 
	{
		rvalid |= service_fdest(node, lfpstate->curr_conn, TSB_PRIMARY);
		if (node->isPair) 
		{
			rvalid |= service_fdest(node, lfpstate->curr_conn, TSB_SECONDARY);
		}
		lfpstate->curr_conn =
			(lfpstate->curr_conn + 1) % (node->fprod.nCons);
		i++;
	}
}

/*
 * ----------------
 *	ExecFluxProd_FT
 * ----------------
 */

#if 0
static void
process_stats(FluxProd_FT * node)
{
	struct timeval end_tv;
	struct timeval dur_tv;

	double		duration,
				bw;
	uint32		tuplen = 40;	/* For now */
	long		total_count;

	struct Instrumentation *instr = ((FluxProd_FT *) node)->instrument;

/* 	if (instr->firsttuple == 0) */
/* 		return; */

	total_count = (long) (instr->firsttuple);

/* 	if (total_count < ((FluxProd *) node)->sched->ntups) return; */

	gettimeofday(&end_tv, NULL);

	timersub(&end_tv, &(instr->starttime), &dur_tv);

	duration = TV_IN_MSEC_DOUBLE(dur_tv);

	elog(DEBUG1, "%d: FProd stats BEGIN ----", node->m_role);
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

	elog(DEBUG1, "%d: FProd stats END ----", node->m_role);

}
#endif /* 0 */

TupleTableSlot *
ExecFluxProd_FT(FluxProd_FT * node)
{
	TupleTableSlot 		*scanslot;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	bool				 isNull;
	int					 dest;
	int 				 seqnum;

	if (!node->fprod.all_done)
	{
		make_connections_FluxProd_FT(node);
		return NULL;
	}

	if (node->mode != FLUX_NORMAL)
	{
		switch(node->mode)
		{
			/*
			 * XXX We need to fix this, failures can happen even during failures.
			 * We don't want to be in a fix where we overwrite our current mode.
			 */
			case (FLUX_FAIL_OVER):
				do_fail_over(node);
				return NULL;

			case (FLUX_TAKE_OVER):
				do_take_over(node);
				if (node->sync)
				{
					do_prod_sync(node);
				}
				break;

			case (STANDBY_NORMAL):
/* 				do_standby(node); */
				if (node->sync)
				{
					do_prod_sync(node);
				}

				if (IsA(node, SegProd))
				{
					return NULL;
				}
				
				break;

			case (STATE_MOVEMENT):

				if (!IsPushPlan(node))
				{
					/* Just call the outer plan */
					ExecProcNode(outerPlan(node), (Plan *) node);
				}
				
				return NULL;
				

			default:
				elog(ERROR, "FluxProd cannot handle mode: %d", node->mode);
				break;
		}
	}

	scanslot = lfpstate->csstate.css_ScanTupleSlot;

	if (!TupIsNull(scanslot))
	{
		seqnum = heap_getattr(scanslot->val, 1,
							  scanslot->ttc_tupleDescriptor, &isNull);
		
		dest = (node->fprod.route_fun)((FluxProd *) node, scanslot);

		/* This will empty the scanslot, if the TSB has enough space. */
		put_ack_TSB(lfpstate->tsb, scanslot, seqnum,
					dest, TSB_PRODUCED, lfpstate->fmasks[dest]);

#ifdef NOT_USED
		if (!TupIsNull(scanslot))
		{
			node->instrument->dropped++;
		}
#endif
		
/* #define DEBUG_TUPS_TSB */
		
#ifdef DEBUG_TUPS_TSB
		if (/* node->fpstate_ft->debug && */ TupIsNull(scanslot))
		{
			elog(DEBUG1, "%d - %d: ITSB S: %d PART: %d", node->m_role,
				 node->fprod.flux_idx, seqnum, dest);
		}
#endif

	}
	else
	{
		/* Returns a tuple into the scanslot from operator below. */
		getNextTuple_FluxProd_FT((Plan *) node, scanslot);
	}

	serviceConn(node);

	/* No need to push any further, this is at plan top */
	
	return scanslot; /* Don't know what else to return */
}

/*
 * --------------------
 *	ExecInitFluxProd_FT
 * --------------------
 */
bool
ExecInitFluxProd_FT(FluxProd_FT * node, EState *estate, Plan *parent)
{

	int			i;
	FluxConnInfo *fpinfo;
	FluxProdState_FT *lfpstate;
	Plan	   *outerPlan;

/* #define DEBUG_MAP */
	
#ifdef DEBUG_MAP
#ifdef DRECOVERY
	elog(DEBUG1, "FPROD: My mode: %d, my role: %d ", node->mode, node->m_role);
	elog(DEBUG1, "FPROD: SADDR: %x ", (uint) node->fprod.map_m2c);
	for (i = 0; i < node->fprod.nCons + 1; i++)
	{
		elog(DEBUG1, "map_m2c: %d -> %d", i, node->fprod.map_m2c[i]);
	}
#endif
#endif

	if (!IsPushPlan(node))
	{
		node->fprod.sched = (FSched *) parent;
	}
	
	lfpstate = makeNode(FluxProdState_FT);

	lfpstate->debug = false;
	
#define COLLECT_STATS
	
#ifdef COLLECT_STATS
	node->instrument = InstrAlloc();
	InstrStartNode(node->instrument);
	gettimeofday(&(node->instrument->counter), NULL);
	node->instrument->dropped = 0;
	node->instrument->m_tuplen = 0;
#endif

	node->fprod.plan.state = estate;
	node->fpstate_ft = lfpstate;

	node->fpstate_ft->tsb = palloc(sizeof(TSB));
	init_TSB(node->fpstate_ft->tsb, node->fprod.nCons, tsb_size);		/* Make some entries */

	if (!node->isPair)
	{
		/* No secondary copies yet */
		lfpstate->fmask = (TSB_PRIMARY | TSB_PRODUCED);
	}
	else
	{
		/* ALl masks must be present */
		lfpstate->fmask = (TSB_PRIMARY | TSB_SECONDARY | TSB_PRODUCED);
	}
	
	lfpstate->fail_prod_pair = false;

	if (!IsPushPlan(node)) /* Bottom will init us if its a push plan */
	{
		outerPlan = outerPlan(node);
		ExecInitNode(outerPlan, estate, (Plan *) node);
	}
	else
	{
		/* No inputs at the start */
		SetInputDone(node);
	}
	
#define FLUXPROD_FT_NSLOTS 2

	ExecInitScanTupleSlot(estate, &(lfpstate->csstate));
	ExecInitResultTupleSlot(estate, &(lfpstate->csstate.cstate));
	
	if (!node->fprod.tupDesc)
	{
		ExecAssignScanTypeFromOuterPlan((Plan *) node, &(lfpstate->csstate));
		ExecAssignResultTypeFromOuterPlan((Plan *) node, &(lfpstate->csstate.cstate));
	}
	else
	{
		ExecAssignScanType(&(lfpstate->csstate), node->fprod.tupDesc, true);
		/* XXX When the scan type tuple descriptor is freed, the result type
		   will be freed as well. */
		ExecAssignResultType(&(lfpstate->csstate.cstate), node->fprod.tupDesc, false);
	}
	
	/* The TSB will free tuples when necessary */
	lfpstate->csstate.css_ScanTupleSlot->ttc_shouldFree = false;

	/* If its a push plan, we need to know where to get the
	   input from */
	if (IsPushPlan(node))
	{
		lfpstate->csstate.cstate.cs_OuterTupleSlot =
			ExecGetTupSlot(outerPlan(node));
	}
	
	lfpstate->map_p2m_p = palloc(sizeof(int) * node->fprod.nCons);
	lfpstate->map_m2p_p = palloc(sizeof(int) * node->fprod.nCons);
	lfpstate->map_p2m_s = palloc(sizeof(int) * node->fprod.nCons);
	lfpstate->map_m2p_s = palloc(sizeof(int) * node->fprod.nCons);

	for (i = 0; i < node->fprod.nCons; i++)
	{
		if (node->m_role == F_PRIMARY)
		{
			lfpstate->map_p2m_p[i] = i;
			lfpstate->map_m2p_p[i] = i;
			lfpstate->map_p2m_s[i] = ROTATE_R(i, node->fprod.nCons);
			lfpstate->map_m2p_s[i] = ROTATE_L(i, node->fprod.nCons);
		}
		else
		{
			lfpstate->map_p2m_p[i] = ROTATE_R(i, node->fprod.nCons);
			lfpstate->map_m2p_p[i] = ROTATE_L(i, node->fprod.nCons);
			lfpstate->map_p2m_s[i] = i;
			lfpstate->map_m2p_s[i] = i;
		}
	}

	/* Initialize primary connection info */
	lfpstate->conn_info_p = (FluxConnInfo *) palloc(sizeof(FluxConnInfo) *
													   node->fprod.nCons);
	lfpstate->num_conn_p = 0;
	lfpstate->curr_conn = 0;

	for (i = 0; i < node->fprod.nCons; i++)
	{
		fpinfo = &(lfpstate->conn_info_p)[i];
		fconn_init_empty(fpinfo);
		fpinfo->conn_idx = i;
		fpinfo->role     = F_PRIMARY;
	}

	lfpstate->conn_info_s = NULL;		/* For now */

	(node->fprod.route_fun_init) ((FluxProd *) node);

	lfpstate->fmasks = palloc(sizeof(int)* node->fprod.nCons);

	for (i = 0; i < node->fprod.nCons; i++)
	{
		lfpstate->fmasks[i] = lfpstate->fmask;
	}

	if (!node->isPair)
	{
		return TRUE;
	}
	
	node->sync= false;
	
	/* Initialize secondary connection info */
	lfpstate->conn_info_s = (FluxConnInfo *) palloc(sizeof(FluxConnInfo) *
													node->fprod.nCons);
	lfpstate->num_conn_s = 0;

	for (i = 0; i < node->fprod.nCons; i++)
	{
		fpinfo = &(lfpstate->conn_info_s)[i];
		fconn_init_empty(fpinfo);
		fpinfo->conn_idx = i;
		fpinfo->role     = F_SECONDARY;
	}

	/* Inititalize spare connections */
	lfpstate->conn_spare_p = palloc(sizeof(FluxConnInfo));
	fconn_init_empty(lfpstate->conn_spare_p);
	lfpstate->conn_spare_p->conn_idx = SPARE_CONN_IDX;
	lfpstate->conn_spare_p->role = F_PRIMARY;
	
	lfpstate->conn_spare_s = palloc(sizeof(FluxConnInfo));
	fconn_init_empty(lfpstate->conn_spare_s);
	lfpstate->conn_spare_s->conn_idx = SPARE_CONN_IDX;
	lfpstate->conn_spare_s->role = F_SECONDARY;
	
#ifdef DEBUG_MAP
#ifdef DRECOVERY
	elog(DEBUG1, "FPROD: My mode: %d, my role: %d ", node->mode, node->m_role);
	elog(DEBUG1, "FPROD: SADDR: %x ", (uint) node->fprod.map_m2c);
	for (i = 0; i < node->fprod.nCons + 1; i++)
	{
		elog(DEBUG1, "map_m2c: %d -> %d", i, node->fprod.map_m2c[i]);
	}
#endif
#endif

	lfpstate->stateMoveCB = palloc(sizeof(FProdSMCB));
	init_syncCB(&(lfpstate->syncs), node->fprod.nCons);
				
	return TRUE;
}

int
ExecCountSlotsFluxProd_FT(FluxProd_FT * node)
{
	if (IsPushPlan(node))
	{
		return FLUXPROD_FT_NSLOTS;
	}
	
	return FLUXPROD_FT_NSLOTS +
		ExecCountSlotsNode(outerPlan(node)) +
		ExecCountSlotsNode(innerPlan(node));
}

/*
 * -----------------
 *	ExecEndFluxProd
 * -----------------
 */
void
ExecEndFluxProd_FT(FluxProd_FT * node)
{
	int					 i;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;

	if (!IsPushPlan(node))
	{
		ExecEndNode(outerPlan(node), (Plan *) node);
	}

	ExecClearTuple(node->fpstate_ft->csstate.css_ScanTupleSlot);
	
	for (i = 0; i < node->fprod.nCons; i++)
	{
		ftrans_reset(lfpstate->conn_info_p[i].trans);
	}

	if (!node->isPair) return;
	
	for (i = 0; i < node->fprod.nCons; i++)
	{
		ftrans_reset(lfpstate->conn_info_s[i].trans);
	}
}

bool ExecGetStateInitFluxProd_FT(FluxProd_FT * node)
{
	node->fpstate_ft->stateMoveCB->copied_md = false;

#ifdef DSMOVE
	elog(DEBUG1, "%d: FProd get state init.", node->m_role);
#endif

	/* Need to freeze the node, so it does not do any more processing */
	
	node->prev_mode = node->mode;
	node->mode = (STATE_MOVEMENT);

	return TRUE;
}

static bool
serTSBMetaData(FluxProd_FT * node, char * buf, int bsize)
{
	FluxProdState_FT *lfpstate = node->fpstate_ft;

	bool ret;
	
	int i, dest;
	
	bool *copy_p_head, *copy_s_head;

	copy_p_head = palloc(sizeof(int) * (lfpstate->tsb->num_parts));
	copy_s_head = palloc(sizeof(int) * (lfpstate->tsb->num_parts));
	
	for (i = 0; i < lfpstate->tsb->num_parts; i++)
	{
		dest = lfpstate->map_p2m_p[i];

		/*
		 * The prim head is relevant only if the mask indicates,
		 * and we are sending to the primary!
		 */

		copy_p_head[i] = ((lfpstate->fmasks[i] & TSB_PRIMARY) &&
						  IsSendingData(&(lfpstate->conn_info_p[dest])));

		dest = lfpstate->map_p2m_s[i];

		/*
		 * The sec head is relevant only if the mask indicates,
		 * and we are sending to the secondary!
		 */

		copy_s_head[i] = ((lfpstate->fmasks[i] & TSB_SECONDARY) &&
						  IsSendingData(&(lfpstate->conn_info_s[dest])));
	}
	
	ret = serTSBMetaDataG(lfpstate->stateMoveCB,
						  lfpstate->tsb,
						  lfpstate->csstate.css_ScanTupleSlot,
						  lfpstate->fmasks,
						  copy_p_head,
						  copy_s_head,
						  buf,
						  bsize,
						  "FProd",
						  node->m_role);

	pfree(copy_p_head);
	pfree(copy_s_head);

	return ret;
}

/* XXX I know this is a ridiculous number of arguments, but I just
   need to do it for now to get things working */

bool
serTSBMetaDataG(FProdSMCB *smcb,
				TSB *tsb,
				TupleTableSlot *scanslot,
				int * fmasks,
				bool * copy_p_head,
				bool * copy_s_head,
				char * buf,
				int bsize,
				char * debug_name,
				FluxRole m_role)
{
	/* I'm being optimistic, the metadata should fit on a single page */
	
	int i;
	int curr_pos = 0;
	int dummy_seq = (-1);
	int md_size;
	int tsize;
	int lent;

#ifdef DSMOVE
	elog(DEBUG1, "%d: %s serializing meta data", m_role, debug_name);
#endif
	
	/* Need to copy the state of the TSB and fmasks */

	/* Copy the state of the TSB meta-data */
	md_size  = sizeof(int)*3; /* max_size, size, num_parts */
	md_size += sizeof(int)*(tsb->num_parts); /* fmasks */
	md_size += sizeof(int)*(tsb->num_parts); /* counts */
		
	/* the table entries always point to the end with
	   seqno (-1) and dmasks = (-1) */
	md_size += sizeof(int)*(tsb->num_parts); /* prod_ack */
	md_size += sizeof(int)*(tsb->num_parts); /* prim_ack */
	md_size += sizeof(int)*(tsb->num_parts); /* sec_ack */
	md_size += sizeof(int)*(tsb->num_parts); /* p_head */
	md_size += sizeof(int)*(tsb->num_parts); /* s_head */
	
	md_size += sizeof(int)*(tsb->num_parts); /* last entries */
	

	if (!TupIsNull(scanslot))
	{
		md_size += sizeof(int);
		tsize = tuple_serializer.determineSize((char *) scanslot->val, NULL);
		md_size += tsize;
	}
	else
	{
		md_size += sizeof(int);
	}
	
	
	if (!(curr_pos + md_size < bsize))
	{
		/* Not enough room to copy the structure info. */
		elog(ERROR, "%d: %s need more space for meta data", m_role, debug_name);
		return false;
	}

	if (!TupIsNull(scanslot))
	{
		i = TRUE;
		memcpy(buf + curr_pos, &(i), sizeof(int));
		curr_pos += sizeof(int);
		
		tuple_serializer.serialize((char *) scanslot->val,
								   buf + curr_pos,
								   bsize - curr_pos,
								   NULL);

		curr_pos += tsize;
#ifdef DSMOVE
		elog(DEBUG1, "%d: %s serialized scantuple, size: %d",
			 m_role, debug_name, tsize);
#endif
	}
	else
	{
		i = FALSE;
		memcpy(buf + curr_pos, &(i), sizeof(int));
		curr_pos += sizeof(int);
#ifdef DSMOVE
		elog(DEBUG1, "%d: %s no scantuple to serialize.", m_role, debug_name);
#endif
	}
	
	memcpy(buf + curr_pos, &(tsb->max_size), sizeof(int));
	curr_pos += sizeof(int);
	memcpy(buf + curr_pos, &(tsb->size), sizeof(int));
	curr_pos += sizeof(int);		
	memcpy(buf + curr_pos, &(tsb->num_parts), sizeof(int));
	curr_pos += sizeof(int);

	for (i = 0; i < tsb->num_parts; i++)
	{
#ifdef DSMOVE
		elog(DEBUG1, "%d: %s fmask[%d] = %d", m_role, debug_name, i, fmasks[i]);
#endif
		memcpy(buf + curr_pos, &(fmasks[i]), sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(buf + curr_pos, &(tsb->counts[i]), sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		if (tsb->last_ents[i])
		{
			lent = TRUE;
		}
		else
		{
			lent = FALSE;
		}
		
		memcpy(buf + curr_pos, &(lent), sizeof(int));
		curr_pos += sizeof(int);
	}
	
	for (i = 0; i < tsb->num_parts; i++)
	{
		/* The prod acks will always be relevant */
		memcpy(buf + curr_pos, &(tsb->prod_ack[i]->seqnum), sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		/* The prim ack is relevant only if the mask indicates */
		if (fmasks[i] & TSB_PRIMARY)
		{
			memcpy(buf + curr_pos, &(tsb->prim_ack[i]->seqnum), sizeof(int));
			curr_pos += sizeof(int);
		}
		else
		{
			memcpy(buf + curr_pos, &(dummy_seq), sizeof(int));
			curr_pos += sizeof(int);
		}
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		/* The sec ack is relevant only if the mask indicates */
		if (fmasks[i] & TSB_SECONDARY)
		{
			memcpy(buf + curr_pos, &(tsb->sec_ack[i]->seqnum), sizeof(int));
			curr_pos += sizeof(int);
		}
		else
		{
			memcpy(buf + curr_pos, &(dummy_seq), sizeof(int));
			curr_pos += sizeof(int);
		}
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		if (copy_p_head[i])
		{
			memcpy(buf + curr_pos, &(tsb->p_head[i]->seqnum), sizeof(int));
			curr_pos += sizeof(int);
		}
		else
		{
			memcpy(buf + curr_pos, &(dummy_seq), sizeof(int));
			curr_pos += sizeof(int);
		}
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		if (copy_s_head[i])
		{
			memcpy(buf + curr_pos, &(tsb->s_head[i]->seqnum), sizeof(int));
			curr_pos += sizeof(int);
		}
		else
		{
			memcpy(buf + curr_pos, &(dummy_seq), sizeof(int));
			curr_pos += sizeof(int);
		}
	}

	/* Initialize counts so we know how many we wrote out */
	smcb->counts = palloc(sizeof(int)*tsb->num_parts);

	for (i = 0; i < tsb->num_parts; i++)
	{
		smcb->counts[i] = 0;
	}

#ifdef DSMOVE
	elog(DEBUG1, "Serialized TSB Metadata");
#endif

	return TRUE;
}

static TSB_Entry *
firstTSBitem(TSB *tsb, int * fmasks, int part)
{
	TSB_Entry *cursor;
	
	cursor = tsb->prod_ack[part];

	/* The acks should always be <= head, so
	   no need to check the head. */

	if (fmasks[part] & TSB_PRIMARY) 
	{
		if (INV_TSB_ENTRY(cursor))
		{
			cursor = tsb->prim_ack[part];
		}
		else if ((!INV_TSB_ENTRY(tsb->prim_ack[part])) &&
				 (cursor->seqnum > tsb->prim_ack[part]->seqnum))
		{
			cursor = tsb->prim_ack[part];
		}

	}

	if (fmasks[part] & TSB_SECONDARY)
	{
		if (INV_TSB_ENTRY(cursor))
		{
			cursor = tsb->sec_ack[part];			
		}
		else if ((!INV_TSB_ENTRY(tsb->sec_ack[part])) &&
				 (cursor->seqnum > tsb->sec_ack[part]->seqnum))
		{
			cursor = tsb->sec_ack[part];			
		}
	}
	
	return cursor;
}

/* XXX This does not copy the state properly if there are
   punctuations in the TSB! */

bool ExecGetStateNextFluxProd_FT(FluxProd_FT * node,
								 char *buf, int bsize)
{
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	TSB       			*tsb  	  = lfpstate->tsb;
	FProdSMCB 			*smcb 	  = lfpstate->stateMoveCB;

#ifdef DSMOVE
	elog(DEBUG1, "%d: FProd get state next.", node->m_role);
#endif
		 
	if (!smcb->copied_md)
	{
		
#ifdef DSMOVE
		int i;
#endif
		
		serTSBMetaData(node, buf, bsize);
		
#ifdef DSMOVE
		dump_seq_TSB(tsb);

		elog(DEBUG1, "Connection status");

		for (i = 0; i < node->fprod.nCons; i++)
		{
			elog(DEBUG1, "%d - %d: FProd conn_info_p ps: %d",
				 node->m_role, node->fprod.flux_idx,
				 node->fpstate_ft->conn_info_p[i].tmp_seqno);
			elog(DEBUG1, "%d - %d: FProd conn_info_s ps: %d",
				 node->m_role, node->fprod.flux_idx,
				 node->fpstate_ft->conn_info_s[i].tmp_seqno);
		}
#endif
		
		smcb->copied_md = true;
		smcb->curr_part = 0;
		smcb->cursor = NULL;
		return FALSE;
	}

	return serTSBTuples(smcb, tsb, lfpstate->fmasks, buf, bsize, "FProd", node->m_role);
}

bool serTSBTuples(FProdSMCB *smcb, TSB * tsb, int * fmasks,
				  char * buf, int bsize,
				  char * debug_name, FluxRole m_role)
{
	int curr_pos = INTALIGN(sizeof(int32));
	int tsize;

#ifdef DSMOVE
	elog(DEBUG1, "%d: %s serializing tuples", m_role, debug_name);
#endif
	
	/* Spool out the results into the buffer until full or results exhausted */
	while (smcb->curr_part < tsb->num_parts)
	{
		/* Copy entries into the buffer until we are done. */
		if (smcb->cursor == NULL)
		{
			/* Initialize cursor to top-most item */
			smcb->cursor = firstTSBitem(tsb, fmasks, smcb->curr_part);

#ifdef DSMOVE
			elog(DEBUG1, "%d: %s first item: seqnum: %d, dmask: %d, tuple: %x",
				 m_role, debug_name, smcb->cursor->seqnum, smcb->cursor->dmask,
				 (uint) smcb->cursor->rtup.data.tuple);
#endif
			
		}

		/* We are at the end, increment part number */
		if (smcb->cursor->dmask < 0) 
		{

#ifdef DSMOVE
			elog(DEBUG1, "%d: %s serialized tups: %d part %d bsize: %d "
				 "write_count: %d",
				 m_role,
				 debug_name,
				 tsb->counts[smcb->curr_part],
				 smcb->curr_part,
				 bsize,
				 smcb->counts[smcb->curr_part]);
#endif
			
			smcb->cursor = NULL;
			smcb->curr_part++;
			continue;
		}

		if (smcb->cursor->rtup.data.tuple == NULL)
		{
/* 			elog(DEBUG1, "%d: FProd tuple is NULL at cursor!", node->m_role); */
/* 			sleep(2000); */
/* 			elog(ERROR, "Waited too long, exiting."); */

			/* XXX Sorry for the HACK, I am going to fix it later */
			smcb->cursor->rtup.type = RESULT_DONE;
		}
		
		tsize = result_serializer.determineSize((char *) &(smcb->cursor->rtup),
												NULL);

		if (!(curr_pos + tsize + 2*sizeof(int) < bsize))
		{
			break;
		}

		/* Copy sequence number */
		memcpy(buf + curr_pos, &(smcb->cursor->seqnum), sizeof(int));
		curr_pos += sizeof(int);

		/* Copy del mask */
		memcpy(buf + curr_pos, &(smcb->cursor->dmask), sizeof(int));
		curr_pos += sizeof(int);

		/* Serialize result */
		result_serializer.serialize((char *) &(smcb->cursor->rtup),
									buf + curr_pos,
									bsize - curr_pos,
									NULL);

		if (smcb->cursor->rtup.type == RESULT_DONE)
		{
			/* XXX Sorry for the HACK, I am going to fix it later */
			/* Just not expecting RESULT errors to be in the TSB  */
			smcb->cursor->rtup.type = RESULT_HEAPTUPLE;
		}
		
		curr_pos += tsize;

		smcb->counts[smcb->curr_part]++;
		
		smcb->cursor = smcb->cursor->next;
	}

#ifdef DSMOVE
	elog(DEBUG1, "%d: %s wrote buffer size of: %d", m_role, debug_name, curr_pos);
#endif
	
	/* Indicate the size of data placed into buffer */
	memcpy(buf, &curr_pos, sizeof(int32));
	
	if (smcb->curr_part < tsb->num_parts)
	{
		return FALSE; /* Not done yet */
	}

	return TRUE; /* Done */
}

bool ExecGetStateCloseFluxProd_FT(FluxProd_FT * node)
{
	
#ifdef DSMOVE
	/* Ideally this is how we should find out about the sync */
	elog(DEBUG1, "%d: FProd get state close.", node->m_role);
#endif

	/* Restore the state */
	node->mode = node->prev_mode;
	node->prev_mode = (-1);

#ifdef NOT_USED
	if (node->fprod.flux_idx > 6)
	{
		node->fpstate_ft->debug = true;
	}
#endif
	
	return TRUE;
}

bool ExecInstallStateInitFluxProd_FT(FluxProd_FT * node)
{
	
#ifdef DSMOVE
	elog(DEBUG1, "%d: FProd install state init", node->m_role);
#endif

	node->fpstate_ft->stateMoveCB->copied_md = false;

	node->prev_mode = node->mode;
	node->mode = (STATE_MOVEMENT);
		
	return TRUE;
}

/* Reverse the primary and secondary bits in the mask */

#define FLIP_TSB_MASK(mask) \
if (((mask) & TSB_PRIMARY) && (!((mask) & TSB_SECONDARY)))       \
{                                                                \
	(mask) &= ~(TSB_PRIMARY);                                    \
    (mask) |= (TSB_SECONDARY);                                   \
}                                                                \
else                                                             \
{                                                                \
	if (((mask) & TSB_SECONDARY) && (!((mask) & TSB_PRIMARY)))   \
	{                                                            \
        (mask) &= ~(TSB_SECONDARY);                              \
        (mask) |= (TSB_PRIMARY);                                 \
	}                                                            \
}                                                                

static void
dump_smcb( FProdSMCB *smcb, int nparts )
{
	int i;

#ifdef DSMOVE
	elog(DEBUG1, "Dumping smcb");
#endif
		
	for (i = 0; i < nparts; i++)
	{
		
#ifdef DSMOVE
		elog(DEBUG1, "i: %d", i);
		elog(DEBUG1, "counts: %d, prod_ack: %d, prim_ack: %d ",
			 smcb->counts[i], smcb->prod_ack[i], smcb->prim_ack[i]);
		elog(DEBUG1, "sec_ack: %d, p_head: %d, s_head: %d ",
			 smcb->sec_ack[i], smcb->p_head[i], smcb->s_head[i]);
#endif
		
	}
}

static bool
deserTSBMetaData(FluxProd_FT * node, char * buf, int bsize)
{
	FluxProdState_FT *lfpstate = node->fpstate_ft;
	TSB *tsb = lfpstate->tsb;
	FProdSMCB  *smcb = lfpstate->stateMoveCB;

	return deserTSBMetaDataG(smcb, tsb, lfpstate->fmasks,
							 lfpstate->csstate.css_ScanTupleSlot, TRUE,
							 buf, bsize, "FProd", node->m_role);
}

bool
deserTSBMetaDataG(FProdSMCB * smcb, TSB * tsb, int * fmasks,
				  TupleTableSlot *scanslot, bool flip_mask,
				  char * buf, int bsize,
				  char * debug_name, FluxRole m_role)
{
	int i;
	int curr_pos = 0;
	int size;
	int dummy;

#ifdef SMOVE
	elog(DEBUG1, "%d: %s Deserializing metadata", m_role, debug_name);
#endif

	memcpy(&i, buf+curr_pos, sizeof(int));
	curr_pos += sizeof(int);

	if (i)
	{
		HeapTuple t;
		
		tuple_serializer.deserialize(buf + curr_pos,
									 (char **) &(t),
									 NULL);

		curr_pos += tuple_serializer.determineSize((char *) t, NULL);

		ExecStoreTuple(t, scanslot, InvalidBuffer, true);

#ifdef SMOVE
		elog(DEBUG1, "%d: %s deserialize scantuple size: %d",
			 m_role, debug_name, (curr_pos - sizeof(int)));
#endif
		
	}
	else
	{

#ifdef SMOVE
		elog(DEBUG1, "%d: %s no scantuple deserialized.", m_role, debug_name);
#endif
		
	}
	
	memcpy(&dummy, buf + curr_pos, sizeof(int));
	curr_pos += sizeof(int);
	
	/* For now assert that these are proper */
	if (dummy != tsb->max_size)
	{
		elog(ERROR, "%d: %s - tsbs are not the same maximum size", m_role, debug_name);
	}

	/* Skip the size */
	memcpy(&size, buf + curr_pos, sizeof(int));
	curr_pos += sizeof(int);

	memcpy(&dummy, buf + curr_pos, sizeof(int));
	curr_pos += sizeof(int);
	
	if (dummy != tsb->num_parts)
	{
		elog(ERROR, "%d: %s - tsbs numparts don't match", m_role, debug_name);
	}

	/* Initialize the statemovement control block */
	smcb->counts   = palloc(sizeof(int) * tsb->num_parts);
	smcb->prod_ack = palloc(sizeof(int) * tsb->num_parts);
	smcb->prim_ack = palloc(sizeof(int) * tsb->num_parts);
	smcb->sec_ack  = palloc(sizeof(int) * tsb->num_parts);
	smcb->p_head   = palloc(sizeof(int) * tsb->num_parts);
	smcb->s_head   = palloc(sizeof(int) * tsb->num_parts);
	smcb->lents    = palloc(sizeof(int) * tsb->num_parts);

	smcb->check_counts   = palloc(sizeof(int) * tsb->num_parts);

	/*
	 * Eventually these masks will all be 111, but I need them
	 * to know what pointers matter and which don't.
	 */
	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(fmasks[i]), buf + curr_pos, sizeof(int));

#ifdef SMOVE
		elog(DEBUG1, "%d: %s got fmask[%d] = %d",
			 m_role, debug_name, i, fmasks[i]);
#endif

		if (flip_mask) 
		{
			FLIP_TSB_MASK(fmasks[i]);

#ifdef SMOVE
			elog(DEBUG1, "%d: %s changed to fmask[%d] = %d",
				 m_role, debug_name, i, fmasks[i]);
#endif
			
		}
		
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->counts[i]), buf + curr_pos, sizeof(int));
		smcb->check_counts[i] = smcb->counts[i];
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->lents[i]), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->prod_ack[i]), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->prim_ack[i]), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->sec_ack[i]), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->p_head[i]), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);
	}

	for (i = 0; i < tsb->num_parts; i++)
	{
		memcpy(&(smcb->s_head[i]), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);
	}

	if (flip_mask) 
	{
		/* If we flip the masks, we should also flip the pointers */

		for (i = 0; i < tsb->num_parts; i++)
		{
			dummy = smcb->prim_ack[i];
			smcb->prim_ack[i] = smcb->sec_ack[i];
			smcb->sec_ack[i] = dummy;
			
			dummy = smcb->p_head[i];
			smcb->p_head[i] = smcb->s_head[i];
			smcb->s_head[i] = dummy;
		}
	}

#ifdef SMOVE
	elog(DEBUG1, "%d: %s Deserialize TSB Metadata", m_role, debug_name);
#endif
	
	dump_smcb(smcb, tsb->num_parts);
	
	return TRUE;
}

static bool
advanceCursor(TSB_Entry **cursor, int seqnum, TSB_Entry *start, TSB_Entry *tail)
{
	if (seqnum == (-1))
	{
		(*cursor) = tail;
		Assert(tail->dmask < 0);
		return TRUE;
	}

	while ((start->seqnum != seqnum) &&
		   (start->dmask > 0))
	{
		start = start->next;
	}

	if (start->seqnum != seqnum)
	{
		return FALSE;
	}

	(*cursor) = start;

	return TRUE;
}

void
adjustTSBPointers(FProdSMCB * smcb, TSB * tsb, int * fmasks,
				  char * debug_name, FluxRole m_role)
{
	TSB_Entry           *head;
	TSB_Entry           *tail;
	int i;
	
	for (i = 0; i < tsb->num_parts; i++)
	{
		/* All pointers should point to the head of the list */
		head = tsb->prod_ack[i];
		tail = tsb->table[i];
		
		if (!advanceCursor(&(tsb->prod_ack[i]), smcb->prod_ack[i], head, tail))
		{
			elog(ERROR, "%d: %s prod_ack seqno not found %d", m_role, debug_name,
				 smcb->prod_ack[i]);
		}
		
		if (!advanceCursor(&(tsb->prim_ack[i]), smcb->prim_ack[i], head, tail))
		{
			elog(ERROR, "%d: %s prim_ack seqno not found %d", m_role, debug_name,
				 smcb->prim_ack[i]);
		}
		
		if (!advanceCursor(&(tsb->sec_ack[i]), smcb->sec_ack[i], head, tail))
		{
			elog(ERROR, "%d: %s sec_ack seqno not found %d", m_role, debug_name,
				 smcb->sec_ack[i]);
		}
		
		if (!advanceCursor(&(tsb->p_head[i]), smcb->p_head[i], head, tail))
		{
			elog(ERROR, "%d: %s p_head seqno not found %d", m_role, debug_name,
				 smcb->p_head[i]);
		}
		
		if (!advanceCursor(&(tsb->s_head[i]), smcb->s_head[i], head, tail))
		{
			elog(ERROR, "%d: %s s_head seqno not found %d", m_role, debug_name,
				 smcb->s_head[i]);
		}

		/* Whichever is turned off, now should be turned on, and cursors reset. */
		if (!(fmasks[i] & TSB_PRIMARY))
		{
			reset_cursors_TSB(tsb, i, TSB_PRIMARY);
		}
		else if (!(fmasks[i] & TSB_SECONDARY))
		{
			reset_cursors_TSB(tsb, i, TSB_SECONDARY);
		}

		fmasks[i] = (TSB_PRODUCED | TSB_PRIMARY | TSB_SECONDARY);
	}

#ifdef DSMOVE
	elog(DEBUG1, "%d: %s tsb after initiailizing", m_role, debug_name);
	dump_seq_TSB(tsb);
	elog(DEBUG1, "%d: %s end-tsb", m_role, debug_name);
#endif
	
}

bool ExecInstallStateNextFluxProd_FT(FluxProd_FT * node,
									 char *buf, int bsize)
{
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	TSB       			*tsb  	  = lfpstate->tsb;
	FProdSMCB 			*smcb 	  = lfpstate->stateMoveCB;

#ifdef DSMOVE
	elog(DEBUG1, "%d: FProd install state next", node->m_role);
#endif

	if (!smcb->copied_md)
	{
		deserTSBMetaData(node, buf, bsize);
#ifdef DSMOVE
		dump_seq_TSB(tsb);
#endif
		smcb->copied_md = true;
		smcb->curr_part = 0;
		return FALSE;
	}

	if (deserTSBTuples(smcb, tsb, TRUE, buf, bsize, "FProd", node->m_role))
	{
		
#ifdef DSMOVE
		elog(DEBUG1, "%d: FProd adjusting pointers and we're almost done.",
			 node->m_role);
#endif
	
		adjustTSBPointers(smcb, tsb, lfpstate->fmasks, "FProd", node->m_role);

		/* We are done */
		return TRUE;
	}
	
	return FALSE;
}

bool
deserTSBTuples(FProdSMCB * smcb, TSB * tsb, bool flip_mask,
			   char * buf, int bsize,
			   char * debug_name, FluxRole m_role)
{
	int 				 curr_pos = INTALIGN(sizeof(int32));
	int 				 dsize;
	int                  dmask;
	int                  seqnum;
	result               res;
	result               *p_res   = &(res);

#ifdef DSMOVE
	elog(DEBUG1, "%d: %s deserializing tuples", m_role, debug_name);
#endif
		 
	memcpy(&dsize, buf, sizeof(int32));

#ifdef DSMOVE
	elog(DEBUG1, "%d: %s buffer size of: %d", m_role, debug_name, dsize);
#endif

	while ((curr_pos < dsize) && (smcb->curr_part < tsb->num_parts))
	{
		if ((smcb->counts[smcb->curr_part]) == 0) 
		{
			Assert(smcb->check_counts[smcb->curr_part] ==
				   tsb->counts[smcb->curr_part]);

#ifdef DSMOVE
			elog(DEBUG1, "%d: %s deserialized tups: %d part: %d",
				 m_role, debug_name,
				 tsb->counts[smcb->curr_part],
				 smcb->curr_part);
#endif

			if (!smcb->lents[smcb->curr_part])
			{
				if (tsb->counts[smcb->curr_part] == 0)
				{
					elog(DEBUG1, "%d: %s -- no way -- no tuples and no last_ent",
						 m_role, debug_name);
					sleep(10000);
					elog(ERROR, "Waited too long ...");
				}

				return_last_ent_TSB(tsb, smcb->curr_part);
			}
			
			smcb->curr_part++;
			continue;
		}

		memcpy(&(seqnum), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);

		memcpy(&(dmask), buf + curr_pos, sizeof(int));
		curr_pos += sizeof(int);

		if (flip_mask)
		{
			/* Flip the mask, because now primary is secondary and vice-versa. */
			FLIP_TSB_MASK(dmask);
		}
		
		result_serializer.deserialize(buf + curr_pos,
									  (char **) &(p_res),
									  NULL);

		curr_pos += result_serializer.determineSize((char *) p_res,
													NULL);

		if (p_res->type == RESULT_DONE)
		{
			p_res->type = RESULT_HEAPTUPLE;
			p_res->data.tuple = NULL;
		}
		
 		append_result_TSB(tsb, &res, seqnum, dmask, smcb->curr_part);

		smcb->counts[smcb->curr_part]--;
		
	}

	if (curr_pos != dsize)
	{
		elog(ERROR, "%d: %s didn't use the whole buffer: "
			 "curr_pos: %d dsize: %d ", m_role, debug_name,
			 curr_pos, dsize);
	}

	/* Advance partitions until we have one with some substance. */
	
	/* XXX We need to do this to know when we are done, so we don't end up
	   swallowing another page which belongs to the child operator. */

	while ((smcb->curr_part < tsb->num_parts) &&
		   ((smcb->counts[smcb->curr_part]) == 0))
	{
		Assert(smcb->check_counts[smcb->curr_part] ==
			   tsb->counts[smcb->curr_part]);

#ifdef DSMOVE
		elog(DEBUG1, "%d: %s deserialized tups: %d part: %d",
			 m_role, debug_name,
			 tsb->counts[smcb->curr_part],
			 smcb->curr_part);
#endif
			
		smcb->curr_part++;
	}

	if (smcb->curr_part < tsb->num_parts)
	{
		return FALSE; /* Not done yet */
	}

/* 	adjustTSBPointers(node); */
	
	return TRUE; /* Done */
}

bool ExecInstallStateCloseFluxProd_FT(FluxProd_FT * node)
{

#ifdef DSMOVE
	/* Ideally this is how we should find out about the sync */
	elog(DEBUG1, "%d: FProd install state close.", node->m_role);
#endif

	/* Restore mode */
	node->mode = node->prev_mode;

#ifdef NOT_USED
	if (node->fprod.flux_idx > 6)
	{
		node->fpstate_ft->debug = true;
	}
#endif
	
	return TRUE;
}

bool ExecResetStateFluxProd_FT(FluxProd_FT * node)
{
	return TRUE;
}

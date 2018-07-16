/*-------------------------------------------------------------------------
 *
 * nodeFluxIngress.c
 *	  The operator feeding input to the rest of the dataflow.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Window
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * DESCRIPTION
 *	  You know, the usual.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxIngress.c,v 1.20 2005/02/09 00:16:42 phred Exp $
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
#include "executor/nodeFluxIngress.h"
#include "executor/nodeFSched.h"
#include "rqueue/rqdest.h"
#include "telegraphcq/fluxroutines.h"

/*
 * Message handlers
 *
 * Ack handlers.
 *
 */
static void
received_fconssync(FluxProd_FT *node, int part_no, bool mask)
{
	FluxProdState_FT *fpstate = node->fpstate_ft;
	
	elog(DEBUG1, "Received an fconssync");

	switch (mask)
	{
		/* Ack came from the primary, so set the secondary connection to normal. */
		case (TSB_PRIMARY):
			reset_cursors_TSB(fpstate->tsb, part_no, TSB_SECONDARY);
			fpstate->fmasks[part_no] |= TSB_SECONDARY;
			break;

		case (TSB_SECONDARY):
			reset_cursors_TSB(fpstate->tsb, part_no, TSB_PRIMARY);
			fpstate->fmasks[part_no] |= TSB_PRIMARY;
			break;
	}

	return;
}

static bool
service_fdest(FluxProd_FT *node, int pidx, int mask) 
{
	FluxConnInfo 	    *fcinfo;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int 				 dest = -1;
	result 				 res;
	bool 				 part_empty = false;
	bool 				 rvalid   = false;
	int  				 rval     = false;
	double  			*counter = NULL;
	int                  seqnum;

#ifdef DEBUG_SEQNO
	bool isNull;
#endif

	switch(mask)
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

	/* Probably should break up this switch statement into multiple functions */
	switch (fcinfo->status) 
	{
		case (CONN_SEND_DATA):
		case (CONN_NORMAL):
			/* Get the first tuple for the partition and send it */
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

						if (((FluxIngress *) node)->no_acks)
						{
							/* Advance ack pointer if we are not getting acks */
							seqnum = get_unack_seqno_TSB(lfpstate->tsb, pidx, mask);
							put_ack_TSB(lfpstate->tsb, NULL, seqnum, pidx, mask,
										lfpstate->fmask);
						}
						
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

			if (((FluxIngress *) node)->no_acks)
			{
				/* Don't process acks if noone is sending them */
				break;
			}
			
			/* **** Its supposed to flow through here ***** */
		case (CONN_ACK_DATA):
			/* Process the acks for this partition. */
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

					case (ACK_FCONSSYNC):
						received_fconssync(node, pidx, mask);
						break;

					default:
						elog(ERROR, "service_fdest: Don't understand ack code.");
						break;
				}
			}

			break; /* CONN_NORMAL && CONN_ACK_DATA */

		case (CONN_SEND_MARKER):
		case (CONN_RECOVER):

			/*
			 * Enqueue a fprodsync marker indicating the machine with which to
			 * recover
			 */

            /* Need to flush this */
			rval = fconn_send_result(fcinfo, &(fcinfo->res), true);
			
			if (rval > 0)
			{
				fcinfo->status = (fcinfo->status == CONN_RECOVER) ?
					CONN_FLUSHBUF : CONN_NORMAL;
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
			break; /* CONN_RECOVER */

		case (CONN_FLUSHBUF):

			rvalid = false;

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
				/* received_fconssync will reset the mask, if so, we are done */
				if (lfpstate->fmasks[pidx] & mask)
				{
					fcinfo->status = CONN_NORMAL;
					rvalid = true;
				}
			}

			break; /* CONN_FLUSHBUF */

		default:  /* This is disconnected or an error I don't understand,
					 do nothing. */
			rvalid = false;
			break;
	}

	return rvalid;
}

static void
serviceConn(FluxProd_FT *node, TupleTableSlot *rslot)
{
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	bool 				 rvalid   = false;
	int  				 i 		  = 0;

	while (!rvalid && (i < node->fprod.nCons))
    {
		/* For each destination partition, process the connection */
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
 * Routines to handle fault-tolerance portion of Ingress operator.
 */

/* Resorts to the spare connection upon fail-over */
static void
do_fail_over(FluxIngress * inode)
{
	FluxConnInfo 	    *fcinfo;
	FluxProd_FT 		*node 	  = (FluxProd_FT *) inode;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int 				 fail_conn;
	int 				 del_mask;
	int 				 dest;

	fail_conn = ((FluxProd *) node)->map_m2c[which_failed];

	/* Determine which parts / machine failed, and use the spares */
	/* inode->fail_cons  = 4; */
                           /*
							* Given our setup, two always fail, and we
							* need to send two syncs for each
							* partition.
							*/
	((FluxProd *) node)->map_c2m[fail_conn] = node->spare_m;

	/* Close the connections */

	/*
	 * XXX Here is where we need to ensure that the spare connections
	 * actually exist. For our purposes right now, they do.
	 */
	fcinfo = &(lfpstate->conn_info_p[fail_conn]);
/* 	close(fcinfo->fd); */
/* 	elog(DEBUG1, "Closing socket: %d", fcinfo->fd); */
	ftrans_reset(fcinfo->trans);
	
	lfpstate->conn_info_p[fail_conn] = *(lfpstate->conn_spare_p);
	
/* 	elog(DEBUG1, "New prim socket: %d", lfpstate->conn_info_p[fail_conn].fd); */
	elog(DEBUG1, "New prim connection.");
	fconn_print_info(&(lfpstate->conn_info_p[fail_conn]));
	
	fcinfo = &(lfpstate->conn_info_s[fail_conn]);
/* 	elog(DEBUG1, "Closing socket: %d", fcinfo->fd); */
/* 	close(fcinfo->fd); */
	ftrans_reset(fcinfo->trans);
	
	lfpstate->conn_info_s[fail_conn] = *(lfpstate->conn_spare_s);
	
/* 	elog(DEBUG1, "New sec socket: %d", lfpstate->conn_info_s[fail_conn].fd); */
	elog(DEBUG1, "New sec connection.");
	fconn_print_info(&(lfpstate->conn_info_s[fail_conn]));
	
	inode->f_part_p = lfpstate->map_m2p_p[fail_conn];
	inode->f_part_s = lfpstate->map_m2p_s[fail_conn];

	/* Choose machines that trade state. */
	
	/*
	 * The source for the standby, will be the machine
	 * of the secondary, since primary failed.
	 */
	fcinfo = &(lfpstate->conn_info_p[fail_conn]);
	dest   = lfpstate->map_p2m_s[inode->f_part_p];
	fcinfo->res.type      = RESULT_FPRODSYNC;
	fcinfo->res.data.info = ((FluxProd *) node)->map_c2m[dest];

	fcinfo = &(lfpstate->conn_info_s[dest]);
	fcinfo->res.type      = RESULT_FPRODSYNC;
	fcinfo->res.data.info = ((FluxProd *) node)->map_c2m[fail_conn];
	fcinfo->status = CONN_SEND_MARKER;
	
	fcinfo = &(lfpstate->conn_info_s[fail_conn]);
	dest   = lfpstate->map_p2m_p[inode->f_part_s];
	fcinfo->res.type      = RESULT_FPRODSYNC;
	fcinfo->res.data.info = ((FluxProd *) node)->map_c2m[dest];

	fcinfo = &(lfpstate->conn_info_p[dest]);
	fcinfo->res.type      = RESULT_FPRODSYNC;
	fcinfo->res.data.info = ((FluxProd *) node)->map_c2m[fail_conn];
	fcinfo->status = CONN_SEND_MARKER;
	
	/* Done specifying all the source and destinations. */
	elog(DEBUG1, "Failed machine: %d", which_failed);
	elog(DEBUG1, "Partition %d - primary failed.", inode->f_part_p);
	elog(DEBUG1, "Partition %d - secondary failed.", inode->f_part_s);

	/* Clear the bits for these guys, the insert a marker to start
	   recovery for each part or machine */
	/* XXX Don't need to ack all, it'll happen automatically when you
	   try and send, the send goes through! */

	/* No clearing bits necessary */

/* 	ack_all_TSB(lfpstate->tsb, f_part_p, TSB_PRIMARY, lfpstate->fmask); */
/* 	reset_head_TSB(lfpstate->tsb, inode->f_part_p, TSB_PRIMARY); */
	
/* 	ack_all_TSB(lfpstate->tsb, f_part_s, TSB_SECONDARY, lfpstate->fmask); */
/* 	reset_head_TSB(lfpstate->tsb, inode->f_part_s, TSB_SECONDARY); */
	
	del_mask = lfpstate->fmasks[inode->f_part_p];
	/* We can probably make this fast, but who cares! */
	ack_all_TSB(lfpstate->tsb, inode->f_part_p, TSB_PRIMARY, del_mask);
	del_mask &= ~TSB_PRIMARY;
	lfpstate->fmasks[inode->f_part_p] = del_mask;

	del_mask = lfpstate->fmasks[inode->f_part_s];
	/* We can probably make this fast, but who cares! */
	ack_all_TSB(lfpstate->tsb, inode->f_part_s, TSB_SECONDARY, del_mask);
	del_mask &= ~TSB_SECONDARY;
	lfpstate->fmasks[inode->f_part_s] = del_mask;
	
/* 	node->mode = FLUX_FAIL_OVER_1; */
	node->mode = FLUX_TAKE_OVER;
}

#define ALL_MASKS (TSB_PRODUCED | TSB_PRIMARY | TSB_SECONDARY)

static void
do_take_over(FluxIngress *inode)
{
	FluxProdState_FT *fpstate = ((FluxProd_FT *) inode)->fpstate_ft;
	FluxConnInfo *fcinfo_p, *fcinfo_s;
	int fail_conn;

	fail_conn = ((FluxProd *) inode)->map_m2c[which_failed];
	fcinfo_p  = &(fpstate->conn_info_p[fail_conn]);
	fcinfo_s  = &(fpstate->conn_info_s[fail_conn]);

	/*
	 * If the failed connections are back to normal, we have received the
	 * FCONSSYNC message and our FPRODSYNC has been sent. We are done with
	 * take-over from this producing end.
	 */

	if ((fcinfo_p->status == CONN_NORMAL) &&
		(fcinfo_s->status == CONN_NORMAL))
	{
		elog(DEBUG1,"FluxIngress: going back to normal mode.");
		((FluxProd_FT *) inode)->mode = FLUX_NORMAL;
		set_recovered_fluxen(((FluxProd *) inode)->sched,
							 ((FluxProd *) inode)->flux_idx);
	}
}

static void
process_stats(FluxIngress * node)
{
	struct timeval end_tv;
	struct timeval dur_tv;

	double		duration,
				bw;
	uint32		tuplen = 40;	/* For now */
	long		total_count;

	struct Instrumentation *instr = ((FluxProd_FT *) node)->instrument;

	if ((instr->firsttuple == 0) &&
		(instr->tuplecount == 0))
		return;

	total_count = (long) (instr->firsttuple + instr->tuplecount);

	return;
	
	if (total_count < ((FluxProd *) node)->sched->ntups * 10) return;

	gettimeofday(&end_tv, NULL);

	timersub(&end_tv, &(instr->starttime), &dur_tv);

	duration = ((double) dur_tv.tv_sec * 1.0e3 +
				(double) dur_tv.tv_usec * 1.0e-3);

	elog(DEBUG1, "FluxIngress stats ----");
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

	instr->nloops++;
	instr->firsttuple = 0;
	instr->tuplecount = 0;

	gettimeofday(&instr->starttime, NULL);

	elog(DEBUG1, " new_start sec: %ld usec: %ld", instr->starttime.tv_sec,
		 instr->starttime.tv_usec);

}

#define CHECK_SEQ

#ifdef CHECK_SEQ
static int p_seq_num = (-1);
#endif

/*
 * ----------------
 *	ExecIngress
 * ----------------
 */
TupleTableSlot *
ExecFluxIngress(FluxIngress * inode)
{
	TupleTableSlot 		*scanslot;
	TupleTableSlot 		*resslot;
	FluxProd_FT 		*node 	  = (FluxProd_FT *) inode;
	FluxProdState_FT 	*lfpstate = node->fpstate_ft;
	int					 dest;
	int 				 seqnum;
	bool				 isNull;

	if (!node->fprod.all_done)
	{
		make_connections_FluxProd_FT(node);
		return NULL;
	}

	if (node->mode != FLUX_NORMAL)
	{
		switch(node->mode)
		{
			case FLUX_FAIL_OVER:
				do_fail_over(inode);
				return NULL;

			case FLUX_TAKE_OVER:
				do_take_over(inode);
				break;

			default:
				elog(ERROR, "Ingress cannot handle mode: %d", node->mode);
				break;
		}
	}

	scanslot = lfpstate->csstate.css_ScanTupleSlot;

	if (!TupIsNull(scanslot))
	{
		seqnum = heap_getattr(scanslot->val, 1,
							  scanslot->ttc_tupleDescriptor, &isNull);

/* #define DEBUG_FIXED_ROUTE */
		
#ifdef DEBUG_FIXED_ROUTE
		
		dest = (node->fprod.route_fun)((FluxProd *) node, &seqnum);
#else
		dest = (node->fprod.route_fun)((FluxProd *) node, scanslot);
#endif

		/* XXX It really should be fmasks[dest] not fmask, but we
		   can get away with it in the FluxIngress operator. */
		
		/* This will empty the scanslot, if the TSB has enough space. */
		put_ack_TSB(lfpstate->tsb, scanslot, seqnum,
					dest, TSB_PRODUCED, lfpstate->fmask);
		

#ifdef CHECK_SEQ
		if (TupIsNull(scanslot))
		{
			if (seqnum > p_seq_num)
			{
				p_seq_num = seqnum;
			}
			else
			{
				elog(DEBUG1, "Error, the input is not in sequence: snum - %d, psnum - %d ... sleeping.", seqnum, p_seq_num);
				sleep(1000);
				elog(ERROR, "Waited too long, exiting");
			}
		}
		else
		{
			ExecClearTuple(scanslot);
/* 			struct timespec bs1; */
/* 			struct timespec bs2; */
/* 			bs1.tv_sec = 0; */
/* 			bs1.tv_nsec = 25; */

/* 			nanosleep(&bs1, &bs2); */

			node->instrument->dropped++;
		}
#endif

#ifdef DEBUG_TUPS
		if (!TupIsNull(scanslot))
		{
			/* elog(DEBUG1, "The tsb is full, seqno: %d", seqnum); */
		}
		else
		{
		  elog(DEBUG1, "Inserted seqno: %d, dest: %d", seqnum, dest);
		}
#endif

	}
	else
	{
		/* Returns a tuple into the scanslot from operator below. */
		getNextTuple_FluxProd_FT((Plan *) node, scanslot);
	}

	/* This slot is not really necessary */
	resslot = lfpstate->csstate.cstate.cs_ResultTupleSlot;
	ExecClearTuple(resslot);

	/* Service our connections to the SegCons downstream. */
	serviceConn(node, resslot);

#define COLLECT_STATS

#ifdef COLLECT_STATS
	process_stats(inode);
#endif

	/* Need to return something? */
	return resslot;
}

/*
 * --------------------
 *	ExecInitFluxIngress
 * --------------------
 */
bool
ExecInitFluxIngress(FluxIngress * node, EState *estate, Plan *parent)
{
/*     tsb_size = 4096*3; */
 	return ExecInitFluxProd_FT((FluxProd_FT *) node, estate, parent);
}

int
ExecCountSlotsFluxIngress(FluxIngress * node)
{
	return ExecCountSlotsFluxProd_FT((FluxProd_FT *) node);
}

/*
 * -----------------
 *	ExecEndFluxIngress
 * -----------------
 */
void
ExecEndFluxIngress(FluxIngress * node)
{
	ExecEndFluxProd_FT((FluxProd_FT *) node);
}

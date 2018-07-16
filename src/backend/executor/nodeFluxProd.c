/*-------------------------------------------------------------------------
 *
 * nodeFluxProd.c
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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxProd.c,v 1.15 2004/07/15 20:39:54 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeFluxProd.h"
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

#include "executor/nodeFluxAcceptor.h"
#include "telegraphcq/fluxroutines.h"

static bool sendTuple(FluxProd * node, int conn_idx, TupleTableSlot *slot, bool flush);
static int	make_new_conn(FluxProd * node, int conn_no);

static bool
sendTuple(FluxProd * node, int conn_idx, TupleTableSlot *slot, bool flush)
{
	result res;
	int    rval;

	res.type  = RESULT_HEAPTUPLE;
	res.data.tuple = slot->val;
	
	rval = fconn_send_result(&(node->fpstate->conn_info[conn_idx]),
							 &res, flush);
	
	return (rval > 0);
}

/* This is a blocking write using a non-blocking socket, don't use too
   often, its slow and does busy writes. */
int
write_b(int fd, char *buf, int size)
{

	int			bwrite,
				twrite;
	int			rval;
	struct pollfd poll_e;

	poll_e.fd = fd;
	poll_e.events = POLLOUT | POLLERR | POLLNVAL | POLLHUP;
	poll_e.revents = 0;

	twrite = 0;

	/* Loop and write until the data is sent */
	while (twrite < size)
	{

		rval = poll(&poll_e, 1, 0);

		if ((rval < 0) && (errno != EINTR))
		{
			elog(DEBUG1, "Could not poll for socket: %d", fd);
			return rval;
		}

		if (poll_e.revents & (POLLERR | POLLNVAL | POLLHUP))
		{
			perror("read_b -- Error");
			elog(DEBUG1, "Read failed on socket: %d", fd);
			return -1;
		}

		/* Nothing to write */
		if (!(poll_e.revents & POLLOUT))
			continue;

		bwrite = write(fd, buf + twrite, size);

		if (bwrite == 0)
		{
			/*
			 * This case is the reason we need to poll first, so we can
			 * return the error. When nothing read and poll says POLLIN.
			 */

			elog(DEBUG1, "Could not write because poll and bwrite conflict.");
			return -1;
		}

		if (bwrite < 0)
		{
			if ((errno == EAGAIN) || (errno == EINTR))
				continue;
			elog(DEBUG1, "Could not write to socket: %d", fd);
			return bwrite;
		}

		twrite += bwrite;
	}

	return twrite;
}

/* to_idx is the name of the destination operator */
int
make_remote_conn(FluxProd * node, int v_mach_no,
				 FluxConnInfo *fcinfo, int to_idx)
{

	int					 sockfd;
	int	                 rval;
	struct sockaddr_in   *new_addr;
	struct sockaddr_in 	 serv_addr;
	struct pollfd 		 poll_e;
	int					 enable = 1;
	int					 optlen;
	int					 buf[3];
	int         		 mach_no;

	SocketTrans         *strans = (SocketTrans *) fcinfo->trans;
	
	mach_no = vmach_2_tnodeid(v_mach_no);
		
	if ((!strans) || (strans->fd < 0))
	{

		if (!strans)
		{
			/* We new a new socket connection */
			fcinfo->trans = newSocketTrans(-1, NULL);
			strans = (SocketTrans *) fcinfo->trans;
		}
		
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			/* Should really kill the query and not the node */
			elog(FATAL, "Could not open socket!");		/* Failed the node */
			fcinfo->status = -1;
			return -1;
		}

		/* Need to set so we can do non-blocking I/O */
		if (ioctl(sockfd, FIONBIO, (char *) &enable) < 0)
		{
			perror("Couldn't enable NBIO");
			elog(FATAL, "Could not set to NONBLOCKING I/O for Flux prod. connection");
			fcinfo->status = -1;
			return -1;
		}

		memset((void *) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
/*		node_addr[12] = '0' + (node->map_c2m[conn_no]); */
		node_addr[12] = '0' + mach_no;

		inet_aton(node_addr, &serv_addr.sin_addr);
		serv_addr.sin_port = htons(fluxPort);

		rval = connect(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr));

		if ((errno != EINPROGRESS) &&
			(errno != EALREADY) &&
			(errno != EISCONN))
		{

			perror("Error: ");
			elog(DEBUG1, "Connect failed: could not connect to Flux on: %d, errno: %d",
				 mach_no, errno);
			fcinfo->status = -1;
			return -1;
		}

		strans->fd = sockfd;
		
		return 0;				/* Why not? */
	}

	/* This is the recommended way to check if you are connected */
	poll_e.fd = strans->fd;
	poll_e.events = POLLOUT | POLLERR | POLLNVAL;
	poll_e.revents = 0;

	rval = poll(&poll_e, 1, 0);

	if (poll_e.revents & POLLOUT)
	{

		int			errv;

		optlen = sizeof(int);
		/* Connected ? */
		if ((errv = getsockopt(strans->fd, SOL_SOCKET, SO_ERROR, (char *) &rval,
							   &optlen))
			< 0)
		{

			perror("Error: ");
			elog(DEBUG1, "Error get value from getsockopt while connecting "
				 "Flux on: %d, err: %d, socket: %d",
				 mach_no, errv, strans->fd);
			fcinfo->status = (-1);
			return -1;
		}

		if (rval != 0)
		{
			perror("Error: ");
			elog(DEBUG1, "Error the connection was not made to Flux on: %d",
				 mach_no);
			fcinfo->status = (-1);
			return -1;
		}

		/* Test if it is connected again? */
		memset((void *) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
/*		node_addr[12] = '0' + (node->map_c2m[conn_no]); */
		node_addr[12] = '0' + mach_no;

		inet_aton(node_addr, &serv_addr.sin_addr);
		serv_addr.sin_port = htons(fluxPort);

		rval = connect(strans->fd, (struct sockaddr *) & serv_addr, sizeof(serv_addr));

		if (rval < 0)
		{
			if (errno != EISCONN)
			{
				elog(FATAL, "What! return value shoud be ISCONN");
				return -1;
			}
		}


		/* Need to set no nagle to send ack information immediately */
		if (setsockopt(strans->fd, IPPROTO_TCP, TCP_NODELAY,
					   (char *) &enable, sizeof(int)) < 0)
		{
			/*
			 * Probably should fail node because we cannot set the socket
			 * option.
			 */
			elog(DEBUG1, "Could not set TCP_NODELAY to Flux on: %d",
				 mach_no);
			fcinfo->status = -1;
			return -1;
		}

		/*
		 * Send the node_id. This is blocking. Should be okay since we
		 * only send an integer or two.
		 */

		buf[0] = node->node_id;
		buf[1] = to_idx;
		buf[2] = node->flux_idx;

		rval = write_b(strans->fd, (char *) buf, 3 * sizeof(int));

		elog(DEBUG1, "FluxProd node_id: %d, conn_no %d, with from_idx: %d, "
			 "Connected to FluxCons on: %d with to_idx: %d",
			 node->node_id, mach_no, node->flux_idx, mach_no, buf[1]);

		if (rval < 0)
		{
			elog(DEBUG1, "Error the connection was not make to FluxCons on: %d",
				 mach_no);
			fcinfo->status = -1;
			return -1;
		}

		/*
		 * We are connected. Do necessary things to setup the queue
		 * structure
		 */

		/* Set the address */
		new_addr = palloc(sizeof(serv_addr));
		(*new_addr) = serv_addr;
		
		initSocketTrans(strans, strans->fd, new_addr);

		/*
		  make_queue(&t_q, *p_no_lock, flux_queue_size);
		  init_conn_info(fcinfo, fcinfo->fd,
					     t_q, flux_queue_size,
					     (Queue *) NULL, ack_queue_size);
		*/
		
		fcinfo->status = CONN_NORMAL;

		return (1);				/* We succeeded! */

	}
	else if (poll_e.revents & (POLLERR | POLLNVAL))
	{

		perror("Poll error: ");
		elog(DEBUG1, "Connect failed: could not connect to FluxCons on: %d errno: %d",
			 mach_no, errno);
		fcinfo->status = -1;
		return (-1);

	}

	return (0);					/* Not sure yet. */
}

static int
make_new_conn(FluxProd * node, int conn_no)
{
	FluxProdState *fpstate;
	FluxConnInfo *fcinfo;
	int			to_idx,
				rval,
				to_mach;

	fpstate = node->fpstate;
	fcinfo = &(fpstate->conn_info)[conn_no];

	to_idx = node->fcons_idx;
	to_mach = node->map_c2m[conn_no];

	if (vmach_2_tnodeid(to_mach) != telegraph_nodeid)
	{

		rval = make_remote_conn(node, to_mach, fcinfo, to_idx);

		if (rval > 0)
		{
			elog(DEBUG1, "Made connection.");
		}

		return rval;
	}
	else
	{
		ExecInsertConnectionFluxAcceptor(node->sched->faccept, (-1),
										 node->node_id, to_idx,
										 node->flux_idx);

		fcinfo->trans = newLocalTrans_q(node->local_q, (Queue *) NULL);
		
		/* 
		   init_conn_info(fcinfo, -1,
		                  node->local_q, 0,
						  (Queue *) NULL, 0);
		*/

		return (1);
	}
}

/*
 * --------------
 *	ExecFluxProd
 * --------------
 */
TupleTableSlot *
ExecFluxProd(FluxProd * node)
{

	TupleTableSlot 		*scanslot;
	TupleTableSlot 		*outerslot;
	FluxProdState 		*lfpstate = node->fpstate;

	/* Make connections before starting */
	if (!node->all_done)
	{

		if (lfpstate->num_conn == node->nCons)
		{
			node->all_done = true;
			elog(DEBUG1, "nodeFluxProd: all done connecting.");
			return NULL;
		}

		if (make_new_conn(node, lfpstate->num_conn) > 0)
		{
			lfpstate->num_conn++;
		}
		
		return NULL;
	}

	scanslot = node->fpstate->csstate.css_ScanTupleSlot;

	if (TupIsNull(scanslot))
	{
		/* Obtain a tuple from below */
		outerslot = ExecProcNode(outerPlan(node), (Plan *) node);

		/* Got nothing from below */
		if (TupIsNull(outerslot))
		{
			return scanslot;
		}
		else
		{
			/* Store it in the scan tuple slot */
			ExecStoreTuple(outerslot->val, scanslot,
						   outerslot->ttc_buffer, outerslot->ttc_shouldFree);

			/*
			 * Take ownership, hope this works, because it is not the
			 * recommended thing to do.
			 */
			outerslot->ttc_shouldFree = false;

			/* Determine the destination */
			node->fpstate->curr_conn = (node->route_fun) (node, scanslot);

#ifdef DBG_PRINT
			elog(DEBUG1, "nodeFluxProd: Sending tuple to %d",
				 node->fpstate->curr_conn);
#endif
		}
	}

	if (sendTuple(node, node->fpstate->curr_conn, scanslot, false))
	{
		/* Tuple was sent */
		ExecClearTuple(scanslot);
	}

	return NULL;
}

/*
 * ------------------
 *	ExecInitFluxProd
 * ------------------
 */
bool
ExecInitFluxProd(FluxProd * node, EState *estate, Plan *parent)
{

	int					 i;
	FluxProdState 		*lfpstate;

	node->sched = (FSched *) parent;
	lfpstate = makeNode(FluxProdState);

	node->plan.state = estate;
	node->fpstate = lfpstate;

	/* Set up the node address string */
	/* node_addr = palloc(strlen(NODE_ADDR)+1); */
	/* strcpy(node_addr, NODE_ADDR); */

#define FLUXPROD_NSLOTS 1

	ExecInitNode(outerPlan(node), estate, (Plan *) node);
	
	ExecInitScanTupleSlot(estate, &(lfpstate->csstate));

	if (!node->tupDesc)
	{
		ExecAssignScanTypeFromOuterPlan((Plan *) node, &(lfpstate->csstate));
	}
	else
	{
		ExecAssignScanType(&(lfpstate->csstate), node->tupDesc, true);
	}
	
	lfpstate->conn_info = (FluxConnInfo *) palloc(sizeof(FluxConnInfo) * node->nCons);
	lfpstate->num_conn = 0;

	for (i = 0; i < node->nCons; i++)
	{
		fconn_init_empty(&(lfpstate->conn_info[i]));
		lfpstate->conn_info[i].conn_idx = i;
		lfpstate->conn_info[i].role     = F_PRIMARY;
	}

	/* Initialize routing funciton */
	(node->route_fun_init)(node);

	return TRUE;
}

int
ExecCountSlotsFluxProd(FluxProd * node)
{
	return FLUXPROD_NSLOTS;
}

/*
 * -----------------
 *	ExecEndFluxProd
 * -----------------
 */
void
ExecEndFluxProd(FluxProd * node)
{
	int					 i;
	FluxProdState 		*lfpstate = node->fpstate;

	/*
	 * Don't really need to do this since its all in the query context as
	 * it is. Isn't palloc and pfree beautiful?
	 */

	/* Any expression contexts to remove? Nope. */
	if (!IsPushPlan(node))
	{
		ExecEndNode(outerPlan(node), (Plan *) node);
	}
	
	/* Free the queues, and close the descriptors */
	for (i = 0; i < node->nCons; i++)
	{
		ftrans_reset(lfpstate->conn_info[i].trans);
	}
	
	pfree(lfpstate->conn_info);

	ExecClearTuple(lfpstate->csstate.css_ScanTupleSlot);
}

/*-------------------------------------------------------------------------
 *
 * nodeFluxAcceptor.c
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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeFluxAcceptor.c,v 1.13 2004/07/15 20:39:54 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "miscadmin.h"
#include "access/heapam.h"
#include "catalog/pg_operator.h"
#include "executor/executor.h"
#include "executor/nodeFluxAcceptor.h"
#include "executor/nodeFluxCons.h"
#include "executor/nodeFluxCons_FT.h"
#include "executor/nodeStateMover.h"
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

static bool init_acceptfd(FluxAcceptor * node);
static void check_new_conn(FluxAcceptor * node);
static void check_all_flux_done(FluxAcceptor * node);
/*
static int	read_b(int fd, char *buf, int size);
*/

#define IsA_FluxCons(a) (IsA(a, FluxCons) || IsA(a, FluxCons_FT))

static bool
init_acceptfd(FluxAcceptor * node)
{

	if (fsched_acceptfd < 0)
		return FALSE;
	else
		node->acceptfd = fsched_acceptfd;

	return TRUE;
}

static void
check_all_flux_done(FluxAcceptor * node)
{

	Plan	   *oper;
	int			i;

	/* At the moment, we are only checking FluxCons */

	for (i = 0; i < node->sched->nfluxen; i++)
	{
		oper = (node->sched->fluxen[i]);

		switch (nodeTag(oper))
		{
			
			case T_FluxCons:
			case T_FluxCons_FT:
			case T_SegCons:
			case T_FluxEgress:
				if (!((FluxCons *) oper)->all_done)
					return;
				break;

			case T_StateMover:
				if (!((FluxProd *) oper)->all_done)
					return;
				break;

				/* Don't care if other operators are not done */
			default:
				break;
		}
	}

	elog(DEBUG1, "All modules on this machine are connected --- FluxAcceptor blocking.");
	node->blocked = true;
}

#if 0
/* A blocking read for non-blocking socket, don't use too often, its slow. */
static int
read_b(int fd, char *buf, int size)
{

	int			bread,
				tread;
	int			rval;
	struct pollfd poll_e;

	poll_e.fd = fd;
	poll_e.events = POLLIN | POLLERR | POLLNVAL | POLLHUP;
	poll_e.revents = 0;

	tread = 0;

	while (tread < size)
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

		/* Nothing to read */
		if (!(poll_e.revents & POLLIN))
			continue;

		bread = read(fd, buf + tread, size);

		if (bread == 0)
		{
			/*
			 * This case is the reason we need to poll first, so we can
			 * return the error. When nothing read and poll says POLLIN.
			 */

			elog(DEBUG1, "Could not read because poll and bread conflict.");
			return -1;
		}

		if (bread < 0)
		{
			if ((errno == EAGAIN) || (errno == EINTR))
			{
				/* Reading failed from the socket, ignore this try */
				continue;
			}
			elog(DEBUG1, "Could not read from socked: %d", fd);
			return bread;
		}

		tread += bread;

	}

	return tread;
}
#endif /* 0 */

/*
  Add connection to node with name: to_idx,
  from node with name: (node_id, from_idx).
*/
void
ExecInsertConnectionFluxAcceptor(FluxAcceptor * node, int sockfd,
								 int node_id, int to_idx, int from_idx)
{
	Plan	   *oper;
	bool		oper_done = false;

	elog(DEBUG1, "nodeFluxAcceptor: t_node_id: %d, Connected to node_id: %d, "
		 "connecting operator from_idx: %d, "
		 "operator to_idx: %d with fd: %d",
		 telegraph_nodeid, node_id, from_idx, to_idx, sockfd);

	oper = (node->sched->fluxen)[to_idx];

	switch (nodeTag(oper))
	{

		case T_FluxCons:
			ExecFluxConsAddConn((FluxCons *) oper, node->p_sockfd, node->p_addr,
								node_id, from_idx);
			oper_done = ((FluxCons *) oper)->all_done;
			break;
		case T_FluxEgress:
		case T_SegCons:
		case T_FluxCons_FT:
			ExecFluxCons_FTAddConn((FluxCons_FT *) oper, node->p_sockfd, node->p_addr,
								   node_id, from_idx);
			oper_done = ((FluxCons *) oper)->all_done;
			break;
		case T_StateMover:
			ExecStateMoverAddConn((StateMover *) oper, node->p_sockfd, node->p_addr,
								  node_id, from_idx);
			oper_done = ((FluxProd *) oper)->all_done;
			break;
		default:
			elog(DEBUG1, "FluxAcceptor: node type %d unsupported for adding connections",
				 (int) nodeTag(oper));
			sleep(1000);
			elog(ERROR, "Waited too long ... aborting.");
			break;
	}

	/* Set myself to blocked if all the fluxen are finished connecting */
	if (oper_done)
	{
		elog(DEBUG1, "Operator on %d with idx %d: all done connecting.",
			 telegraph_nodeid, to_idx);
	}

	return;
}

/*
 * Checks for a new connection. And if it exists, add it to the fcstate
 */
static void
check_new_conn(FluxAcceptor * node)
{

	int			enable,
				cli_len,
				rval;
	struct sockaddr_in cli_addr;
	struct pollfd poll_e;
	int			node_id,
				to_idx,
				from_idx;

#ifdef DBG_PRINT
	elog(DEBUG1, "Checking new connection");
#endif

	if (node->p_sockfd < 0)
	{

		poll_e.fd = node->acceptfd;
		poll_e.events = POLLIN | POLLERR | POLLNVAL;
		poll_e.revents = 0;

		rval = poll(&poll_e, 1, 0);

		if (!(poll_e.revents & POLLIN))
			return;				/* Nothing to look fer */

		cli_len = sizeof(cli_addr);

		if ((node->p_sockfd = accept(node->acceptfd, (struct sockaddr *) & cli_addr,
									 &cli_len)) < 0)
		{

			node->p_sockfd = -1;

			if ((errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK))
				return;			/* Nothing */
			else
			{
				/*
				 * Probably should fail this node because the port is no
				 * longer ours
				 */
				elog(ERROR, "Something screwed up while checking for connections");
				return;
			}

		}

		/* Copy the address */
		node->p_addr = palloc(cli_len);
		*(node->p_addr) = cli_addr;
		
		enable = 1;

		/* Need to set so we can do non-blocking I/O */
		if (ioctl(node->p_sockfd, FIONBIO, (char *) &enable) < 0)
		{

			node->p_sockfd = -1;

			elog(DEBUG1, "Could not set to NONBLOCKING I/O for accept connections");
			perror("Couldn't enable NBIO");
			return;
		}

		/* Need to set no nagle to send ack information immediately */
		if (setsockopt(node->p_sockfd, IPPROTO_TCP, TCP_NODELAY,
					   (char *) &enable, sizeof(int)) < 0)
		{

			node->p_sockfd = -1;

			/*
			 * Probably should fail node because we cannot set the socket
			 * option.
			 */
			elog(DEBUG1, "Could not set TCP_NODELAY");
			return;
		}

		/* We have a valid socket */
		node->p_buf_read = 0;
	}

	rval = read(node->p_sockfd,
				(char *) node->p_buf + node->p_buf_read,
				3 * sizeof(int) - node->p_buf_read);

	if (rval < 0)
	{

		if ((errno == EAGAIN) || (errno == EINTR))		/* We can check again */
			return;

		node->p_sockfd = -1;
		elog(DEBUG1, "Error the connection to new socket not made.");
		return;

	}

	node->p_buf_read += rval;

	if (node->p_buf_read < 3 * sizeof(int))
		return;

	/* Now, the connection is established */
	node_id = node->p_buf[0];
	to_idx = node->p_buf[1];
	from_idx = node->p_buf[2];

	ExecInsertConnectionFluxAcceptor(node, node->p_sockfd, node_id, to_idx, from_idx);

	node->p_sockfd = -1;		/* Added the connection */

	return;						/* Created the connection */
}


/* ---------------------------------------
 *	 ExecFluxCons
 *
 *		Does the consuming from the sockets.
 * ------------------------------------------
 */

TupleTableSlot *
ExecFluxAcceptor(FluxAcceptor * node)
{

	/* If we are blocked and no other nodes have failed, just return */
	/* if (node->blocked && !(failed_node)) */
	if (node->blocked)			/* We must unblock this when we do take
								 * over */
		return NULL;

	/*
	 * Otherwise, check for a new connection and dispatch it to the
	 * approriate node if made.
	 */

	check_new_conn(node);
	check_all_flux_done(node);

	return NULL;				/* I never return any tuples */
}

/* -----------------
 * ExecInitFluxCons
 *
 * -----------------
 */
bool
ExecInitFluxAcceptor(FluxAcceptor * node, EState *estate, Plan *parent)
{
	bool		rbool;

	node->plan.state = estate;

	node->blocked = false;

	node->sched = (FSched *) parent;

	node->p_sockfd = -1;
	node->p_buf_read = 0;

	/* Initialize the port for accepting parallel connections */
	rbool = init_acceptfd(node);

	if (!rbool)
		return FALSE;			/* Port initialization did not succeed */

	/*
	 * The other fields should already be set for me from
	 * converttofsched_dup: nfluxen, and fluxen
	 */

	return TRUE;
}

#define FLUXACCEPTOR_NSLOTS 0;

int
ExecCountSlotsFluxAcceptor(FluxAcceptor * node)
{
	return FLUXACCEPTOR_NSLOTS;
}

/* ------------------------
 *		ExecEndFluxCons(node)
 *
 * -----------------------
 */
void
ExecEndFluxAcceptor(FluxAcceptor * node)
{
	/* Nothing to do */
}


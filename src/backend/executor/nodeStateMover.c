/*-------------------------------------------------------------------------
 *
 * nodeStateMover.c
 *
 *	  StateMover operator. Would normally be a thread, but since its
 *	  an FSched thing, I leave it as an operator inside. Plus, I do
 *	  node want to pay the overhead of going through shared memory between
 *	  postgres processes.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * DESCRIPTION
 *	  You know, the usual.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/nodeStateMover.c,v 1.12 2004/07/01 23:50:08 mashah Exp $
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
#include "executor/nodeFluxProd.h"
#include "executor/nodeStateMover.h"
#include "telegraphcq/fluxroutines.h"

#ifdef DEBUG_CONN
static void print_connections_StateMover(StateMover * node);
#endif

static void notify_requestor(StateMover * node);

static void
check_conn_complete(StateMover * node)
{
	/* I'm already connected to myself */
	if (node->n_conn == ((node->n_smovers) - 1))
	{

		elog(DEBUG1, "StateMover all done connecting.");
		node->fprod.all_done = true;
		node->curr_conn = 0;

#ifdef DEBUG_CONN
		print_connections_StateMover(node);
#endif

	}
}

static int
make_new_conn(StateMover * node, int conn_no)
{
	int					 to_mach;
	int					 rval;
	int					 to_idx = STATE_MOVER_IDX;
	FluxConnInfo 		*fcinfo;

	fcinfo = &(node->conn_info[conn_no]);

	to_mach = node->fprod.map_c2m[conn_no];

	if ((rval = make_remote_conn((FluxProd *) node, to_mach, fcinfo, to_idx)) < 0)
	{

		elog(DEBUG1, "StateMover on %d cannot connect to StateMover on %d",
			 telegraph_nodeid, to_mach);

	}

	if (rval > 0)
	{
/* 		Queue	   *ack_q; */

		elog(DEBUG1, "StateMover on %d connected to mach %d. ",
			 telegraph_nodeid,
			 to_mach);
		
/* 		make_queue(&ack_q, *p_no_lock, sm_queue_size); */

		/* Initialize the structure properly. */
/* 		init_conn_info(fcinfo, fcinfo->fd, */
/* 					   fcinfo->qc_in->queue, sm_queue_size, */
/* 					   ack_q, sm_queue_size); */

		node->n_conn++;
	}

	return rval;
}


static void
process_send_eof(StateMover * node)
{
	int					 cidx;
	int					 rval;
	FluxConnInfo        *fcinfo;
	SocketTrans         *strans;

#ifdef DSMOVE
	elog(DEBUG1, "In process_send_eof");
#endif

	if (node->curr_comm->done)
	{

#ifdef DSMOVE
		elog(DEBUG1, "Qsize: %d", node->curr_comm->queue->size);
#endif
		
		/* Since I'm last, I free the request */
		pfree(node->curr_req);
		node->curr_req = NULL;
		reset_qcomm(node->curr_comm);
		return;
	}

	cidx = node->fprod.map_m2c[node->curr_req->mach];
	fcinfo = &(node->conn_info[cidx]);
	strans = (SocketTrans *) fcinfo->trans;

	rval = send_queue_nb(node->curr_comm, strans->fd);

	if (rval < 0)
	{
		elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
		set_failed_conn((Plan *) node, fcinfo, rval, cidx);
	}

	return;
}

static void
process_send(StateMover * node)
{
	int					 cidx;
	int					 rval;
	FluxConnInfo 		*fcinfo;
	SocketTrans         *strans;

/* 	elog(DEBUG1, "In process_send"); */

	if ((node->curr_comm->done) || (node->curr_comm->size == -1))
	{
		/* Finished sending the previous queue. */
		if (node->curr_comm->queue)
		{
#ifdef DQUEUES
			elog(DEBUG1, "Qsize: %d", node->curr_comm->queue->size);
#endif
			free_queue(&(node->curr_comm->queue));
			node->curr_comm->queue = NULL;
		}

		/* If I did not get a new queue to send, yield and try again. */
		if (!DEQUEUE_INT(node->move_bufs, &(node->curr_comm->queue)))
			return;

		/* Got a queue */

		/* If I got a null pointer, I done with sending this request */
		if (node->curr_comm->queue == NULL)
		{
			node->curr_comm->queue = node->empty_q;
			/* node->curr_req->action = SMR_SEND_EOF; */
			node->curr_req->mode = SM_SEND_EOF;
		}

		node->curr_comm->size = node->curr_comm->queue->size;
		node->curr_comm->done = false;
		node->curr_comm->pos = 0;
	}

	cidx = node->fprod.map_m2c[node->curr_req->mach];
	fcinfo = &(node->conn_info[cidx]);
	strans = (SocketTrans *) fcinfo->trans;

	rval = send_queue_nb(node->curr_comm, strans->fd);

	if (rval < 0)
	{
		elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
		set_failed_conn((Plan *) node, fcinfo, rval, cidx);
	}

	return;
}

static void
process_receive(StateMover * node)
{
	int					 cidx;
	int					 rval;
	FluxConnInfo 		*fcinfo;
	SocketTrans         *strans;

/* 	elog(DEBUG1, "In process_receive"); */

#ifdef DEBUG_CONN
	print_connections_StateMover(node);
#endif
	
	if (node->curr_comm->done)
	{
		
#ifdef DQUEUES
		elog(DEBUG1, "StateMover: Received another queue from mach %d",
			 node->curr_req->mach);
#endif

		if (ENQUEUE_INT(node->move_bufs, &(node->curr_comm->queue)))
		{

#ifdef DQUEUES
			elog(DEBUG1, "Queue size: %d", node->curr_comm->queue->size);
#endif
			/* End marker, we finish now */
			if (node->curr_comm->queue->size == QSTRUCTSIZE)
			{

#ifdef DQUEUES
				elog(DEBUG1, "Finished processing request, size: %d",
					 node->curr_comm->queue->size);
#endif

				/* Request will be freed by the client */
				node->curr_req = NULL;
			}

			/* The queue is actually freed by the consumer of move_bufs */
			reset_qcomm(node->curr_comm);
			return;
		}
	}

	cidx = node->fprod.map_m2c[node->curr_req->mach];
	fcinfo = &(node->conn_info[cidx]);
	strans = (SocketTrans *) fcinfo->trans;

	/* Need to do something here if we die during movement */
	rval = receive_queue_nb(node->curr_comm, strans->fd);

	if (rval < 0)
	{
		elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
		set_failed_conn((Plan *) node, fcinfo, rval, cidx);
	}

	return;
}

static void
prepare_request(StateMover * node)
{
	int			  		 cidx;
	int           		 rval;
	FluxConnInfo 		*fcinfo;
	SocketTrans         *strans;

	switch(node->curr_req->action)
	{
		case (SMR_RECV_BUF):
			/* Send an empty buffer to signal beginning */
			cidx = node->fprod.map_m2c[node->curr_req->mach];
			fcinfo = &(node->conn_info[cidx]);
			strans = (SocketTrans *) fcinfo->trans;

			if (!node->curr_comm->queue)
			{
				reset_qcomm(node->curr_comm);
				node->curr_comm->queue = node->empty_q;
				node->curr_comm->size = node->curr_comm->queue->size;
			}

#ifdef DQUEUES
			elog(DEBUG1, "StateMover sending init queue.");
#endif
			
			rval = send_queue_nb(node->curr_comm, strans->fd);
			
			if (rval < 0)
			{
				elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
				set_failed_conn((Plan *) node, fcinfo, rval, cidx);
			}

			if (!node->curr_comm->done)
			{
				return;
			}
			
#ifdef DQUEUES
			elog(DEBUG1, "StateMover sent init queue, "
				 " notifying requestor for reception");
#endif

			reset_qcomm(node->curr_comm);
			
			notify_requestor(node);
			node->curr_req->mode = SM_RECV_BUF;
			
			break;

		case (SMR_SEND_BUF):
			/* Wait for the signal from other statemover to send. */
			cidx = node->fprod.map_m2c[node->curr_req->mach];
			fcinfo = &(node->conn_info[cidx]);
			strans = (SocketTrans *) fcinfo->trans;

			/* Need to do something here if we die during movement */
			rval = receive_queue_nb(node->curr_comm, strans->fd);

			if (rval < 0)
			{
				elog(DEBUG1, "Returned rval: %d errno: %d", rval, errno);
				set_failed_conn((Plan *) node, fcinfo, rval, cidx);
			}

			if (!node->curr_comm->done)
			{
				return;
			}
			
			if (node->curr_comm->queue->size != QSTRUCTSIZE)
			{
				elog(ERROR, "StateMover: first queue I read best be empty.");
			}

#ifdef DQUEUES
			elog(DEBUG1, "StateMover got init queue, "
				" notifying requestor for transfer.");
#endif

			free_queue(&(node->curr_comm->queue));
			reset_qcomm(node->curr_comm);
				
			notify_requestor(node);
			node->curr_req->mode = SM_SEND_BUF;
			
			break;
	}
}

static void
process_request(StateMover * node)
{

	switch (node->curr_req->mode)
	{
		case (NEW_REQUEST):
			prepare_request(node);
			break;

		case (SM_RECV_BUF):
			process_receive(node);
			break;

		case (SM_SEND_BUF):
			process_send(node);
			break;

		case (SM_SEND_EOF):
			process_send_eof(node);
			break;

		default:
			elog(DEBUG1, "StateMover do not understand request: %d",
				 node->curr_req->mode);
			sleep(2000);
			elog(ERROR, "Waited too long.");
			break;
	}
	
	return;
}

static void
notify_requestor(StateMover * node)
{
	/* Set the ready flag, if other guys are done. */
	if (QEMPTY(node->move_bufs))
	{
		*(node->curr_req->ready_flag) = true;
#ifdef DSMOVE
		elog(DEBUG1, "Notification sent for new request.");
		elog(DEBUG1, "Request - ACTION %d MACH %d ",
			 node->curr_req->action, node->curr_req->mach);
#endif
	}
}

TupleTableSlot *
ExecStateMover(StateMover * node)
{
	if (!node->fprod.all_done)
	{

		while (node->curr_conn < node->n_smovers)
		{
			if (make_new_conn(node, node->curr_conn) != 0)
			{
				node->curr_conn++;		/* Skip if there is an error */
			}
		}

		/* This check must go after the previous call */
		check_conn_complete(node);

		return NULL;

	}

	/*
	 * If the StateMover turns into a thread, better change this request
	 * queue to have a real locking structure.
	 */
	if (node->curr_req != NULL)
	{
		process_request(node);
		return NULL;
	}

	/* Nothing to do */
	if (QEMPTY(node->request_queue))
	{
		return NULL;
	}
	
#ifdef DSMOVE
	elog(DEBUG1, "Queue numelts: %d", node->request_queue->numelts);
#endif
		 
	DEQUEUE_INT(node->request_queue, &(node->curr_req));

	/* We cannot notify them immediately if it is a send request */
/* 	notify_requestor(node); */
	
	return NULL;
}

/*
 * Pre: Need map_c2m and map_m2c initialized.
 *		Need n_smovers initialized.
 */
bool
ExecInitStateMover(StateMover * node, EState *estate, Plan *parent)
{
	int			i;
	FluxConnInfo *fcinfo;

	node->fprod.plan.parent = parent;
	node->fprod.plan.state = estate;

	node->fprod.sched = (FSched *) parent;

	node->conn_info = (FluxConnInfo *)
		palloc(sizeof(FluxConnInfo) * node->n_smovers);

	for (i = 0; i < node->n_smovers; i++)
	{
		fcinfo = &(node->conn_info)[i];
		fconn_init_empty(fcinfo);
		fcinfo->conn_idx = i;
		fcinfo->role     = F_PRIMARY;
/* 		init_empty_conn(fcinfo); */
	}

	/* XXX Assumes machines are in order */
	
	node->fprod.all_done = false;
	node->n_conn = 0;
	node->curr_conn = tnodeid_2_vmach(telegraph_nodeid) + 1;
	node->curr_comm = palloc(sizeof(QueueComm));
	reset_qcomm(node->curr_comm);

	/* This also lets you know what we are actually passing on the queues */
	make_queue(&(node->request_queue), *p_no_lock,
			   CALC_QSIZE(REQ_QSIZE, sm_request *));
	make_queue(&(node->move_bufs), *p_no_lock,
			   CALC_QSIZE(STATE_QSIZE, Queue *));
	make_queue(&(node->empty_q), *p_no_lock,
			   CALC_QSIZE(0, 0));
	return true;
}

int
ExecCountSlotsStateMover(StateMover * node)
{
	return 0;
}

/* Just closes the connections. */
void
ExecEndStateMover(StateMover * node)
{
	int			i;

	for (i = 0; i < node->n_smovers; i++)
	{
		ftrans_reset(node->conn_info[i].trans);
	}
}

void
ExecStateMoverAddConn(StateMover * node,
					  int sockfd, struct sockaddr_in *addr,
					  int node_id, int sidx)
{
/* 	Queue	   *rec_q, */
/* 		*ack_q; */
	FluxConnInfo *fcinfo;
	int			cidx;

	cidx = index_of_mach(((FluxProd *) node)->map_c2m,
						 node->n_smovers,
						 tnodeid_2_vmach(node_id));

	if (cidx == node->n_smovers)
	{
		elog(ERROR, "This StateMover does not know about machine %d",
			 node_id);
	}

	/* Do I really care about the sidx */
	Assert(sidx == STATE_MOVER_IDX);
	fcinfo = &(node->conn_info[cidx]);
	fcinfo->status = CONN_NORMAL;
	node->n_conn++;

	fcinfo->trans = newSocketTrans(sockfd, addr);
		
/* 	make_queue(&rec_q, *p_no_lock, sm_queue_size); */
/* 	make_queue(&ack_q, *p_no_lock, sm_queue_size); */

	/* The incoming queue will be allocated at receive time. */
/* 	init_conn_info(fcinfo, sockfd, */
/* 				   rec_q, sm_queue_size, */
/* 				   ack_q, sm_queue_size); */

#ifdef DEBUG_CONN
	elog(DEBUG1, "Connection: %d socket: %d num_conn: %d", cidx, sockfd, node->n_conn);
#endif

	elog(DEBUG1, "StateMover: %d connected to mover on %d", node->fprod.node_id,
		 node_id);

}

/* Debugging routines */

#ifdef DEBUG_CONN
static void
print_connections_StateMover(StateMover * node)
{

	int			i;

	elog(DEBUG1, "Printing state mover connections");

	for (i = 0; i < node->n_smovers; i++)
	{

		elog(DEBUG1, "Conn: %d", i);
		print_conn_info(&node->conn_info[i]);

	}

}
#endif

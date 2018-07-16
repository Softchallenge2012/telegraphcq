/*-------------------------------------------------------------------------
 *
 * fluxroutines.c
 *	  Bunch of shared helper routines for the Flux modules.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Window
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * DESCRIPTION
 *    You know, the usual.
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/fluxroutines.c,v 1.20 2004/07/15 20:39:56 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"
#include "miscadmin.h"
#include "access/heapam.h"
#include "executor/executor.h"
#include "executor/nodeFSched.h"
#include <errno.h>
#include "telegraphcq/fluxroutines.h"
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>   /* needed for darwin */
#include <netinet/in.h>   /* needed for darwin */

/* Permutation map from virtual machine to telegraph_nodeid */
static int a_vmach_2_tnodeid[] = {1, 2, 3, 4, 5, 6, 7, 0};
/* static int a_vmach_2_tnodeid[] = {2, 3, 4, 5, 6, 7, 0, 1}; */

/* Permutation map from telegraph_nodeid to virtual machine */
static int a_tnodeid_2_vmach[] = {7, 0, 1, 2, 3, 4, 5, 6};
/* static int a_tnodeid_2_vmach[] = {6, 7, 0, 1, 2, 3, 4, 5}; */


int
vmach_2_tnodeid(int vmach)
{
	return a_vmach_2_tnodeid[vmach];
}

int
tnodeid_2_vmach(int tnodeid)
{
	return a_tnodeid_2_vmach[tnodeid];
}

double f_time_since(struct timeval *start_tv)
{
	struct timeval dur_tv, end_tv;

	gettimeofday(&end_tv, NULL);
	timersub(&end_tv, start_tv, &(dur_tv));

	return TV_IN_MSEC_DOUBLE(dur_tv);
}

/* Connection location routines */
int
index_of_mach(int *map_c2m, int size, int mach) 
{
	int i = 0;

	while((i < size) && (map_c2m[i] != mach)) {
		i++;
	}

	return i;
}

#ifdef USE_OLD_FCONN
void
init_empty_conn(FluxConnInfo_FT *fcinfo)
{
	fcinfo->fd     = -1;
	fcinfo->error  = CONN_DISCONN;
	fcinfo->qc_in  = NULL;
	fcinfo->qc_ack = NULL;
	fcinfo->p_htup = NULL;
	fcinfo->local  = false;
	fcinfo->prev_seqno = -1;
	fcinfo->tmp_seqno  = -1;
	fcinfo->conn_idx   = -1;
}
#endif /* USE_OLD_FCONN */

/* ftrans version */
void
fconn_init_empty(FluxConnInfo *fcinfo)
{
	memset(fcinfo, 0, sizeof(FluxConnInfo));
	
	fcinfo->trans      = (NULL);
	fcinfo->status     = CONN_DISCONN;
	
	fcinfo->prev_seqno = -1;
	fcinfo->tmp_seqno  = -1;
	fcinfo->conn_idx   = -1;
	
	fcinfo->p_htup 	   = (NULL);
	fcinfo->next_state = CONN_UNKNOWN;
}

#ifdef USE_OLD_FCONN
int
init_conn_info(FluxConnInfo_FT *fcinfo, int sockfd,
			   Queue *q_out, int q_out_size,
			   Queue *q_ack, int q_ack_size)
{
	fcinfo->fd    = sockfd;
	fcinfo->local = (sockfd < 0);
	fcinfo->error = 1;

	/* This qc_in is a misnomer here */
	fcinfo->qc_in        = palloc(sizeof(QueueComm));
	fcinfo->qc_in->queue = q_out;
	fcinfo->qc_in->size  = q_out_size;
	fcinfo->qc_in->pos   = 0;
	/* Since its a local queue, we do not really need to send
	   or receive it */
	fcinfo->qc_in->done  = (q_out != NULL);

	/* We need to have a reverse localfconnection for 
	   status messages */
	fcinfo->qc_ack        = palloc(sizeof(QueueComm));
	fcinfo->qc_ack->queue = q_ack;
	fcinfo->qc_ack->size  = q_ack_size;
	fcinfo->qc_ack->pos   = 0;
	/* Since its a local queue, we do not really need to send
	   or receive it */
	fcinfo->qc_ack->done  = (q_ack != NULL);
	fcinfo->prev_seqno    = (-1);
	fcinfo->p_htup        = (NULL);

	return 0;
}
#endif /* USE_OLD_FCONN */

/* ftrans version */
void
fconn_init_info(FluxConnInfo *fcinfo, ftransport *trans,
				int cidx, int pno, FluxRole mrole)
{
	fcinfo->trans    = trans;
	fcinfo->status   = CONN_DO_NOTHING;
	
	fcinfo->conn_idx = cidx;
	fcinfo->part_no  = pno;
	fcinfo->role 	 = mrole;
}

void
init_mergeCB(CMerge *merge, int nent)
{
	int i;
	
	merge->cpos = 0;
	merge->htup = palloc(sizeof(HeapTuple) * nent);
	merge->snum = palloc(sizeof(seq_num) * nent);
	merge->ready = palloc(sizeof(bool) * nent);

	for (i = 0; i < nent; i++)
	{
		merge->htup[i] = NULL;
		merge->snum[i] = (-1);
		merge->ready[i] = false;
	}
	
	merge->nfill = 0;
}


void init_snapshotCB(SnapshotCB *snap, int nent)
{
	snap->alive_p   = palloc(sizeof(int) * nent);
	snap->alive_s   = palloc(sizeof(int) * nent);
	snap->got_syncs = palloc(sizeof(int) * nent);
}

void init_syncCB(SyncCB *sync, int nent)
{
	int i;

	sync->sync_recv_p = palloc(sizeof(int) * nent);
	sync->sync_recv_s = palloc(sizeof(int) * nent);

	for (i = 0; i < nent; i++)
	{
		sync->sync_recv_p[i] = false;
		sync->sync_recv_s[i] = false;
	}
}

void init_pauseCB(PauseCB *pauses, int nent)
{
	int i;

	pauses->ack_recv = palloc(sizeof(int) * nent);
	
	for (i = 0; i < nent; i++)
	{
		pauses->ack_recv[i] = false;
	}
}

/* Routine for sending markers from connections and setting the state
 * afterwards */
void
smark_conn(FluxConnInfo *fcinfo, int next_state, ResultType type, int info)
{
	fcinfo->res.data.info = info;
	fcinfo->res.type = type;
	fcinfo->next_state = next_state;
	fcinfo->status = CONN_SEND_MARKER;
}

/* Tell the scheduler that this node has failed */
void
set_failed_conn(Plan *node, FluxConnInfo *fcinfo,
				int eval, int conn_idx)
{
	fcinfo->status = eval;

	ftrans_reset(fcinfo->trans);
	
	switch(nodeTag(node)) {

	case T_FluxProd:
	case T_FluxProd_FT:
	case T_FluxIngress:
	case T_StateMover:
		{
			FluxProd *fprod = (FluxProd *) node;
			set_failed_machine(fprod->sched, fprod->map_c2m[conn_idx]);
		}
		break;

	case T_FluxCons:
	case T_FluxCons_FT:
	case T_SegCons:
		{
			FluxCons *fcons = (FluxCons *) node;
			set_failed_machine(fcons->sched, fcons->map_c2m[conn_idx]);
		}
		break;

	default:
		elog(ERROR, "Cannot set machine failed from this node, tag: %d", 
			 nodeTag(node));
		break;

	}
}

/* #define DEBUG_QUEUES */
/* #define DEBUG_TUPLES */

/* Networking routines */

/* Returns true  (1), if the item was "sent" */
/* Returns false (0), if the item was not "sent" */
/* Returns      < 0 , if there was an error, or the connection is invalid */

/* I know this is ugly, but I need it for now */

/* #define DEBUG_SEQNO */

#ifdef USE_OLD_FCONN
#ifdef DEBUG_SEQNO
int
result_conn_send(FluxConnInfo_FT *fcinfo, result *p_res, int flush, int seqno)
#else
int
result_conn_send(FluxConnInfo_FT *fcinfo, result *p_res, int flush)
#endif
{
	bool rbool;
	int  rval;
	QueueComm *qc_out;

	if (FC_ERR(fcinfo)) return -1;

	qc_out = fcinfo->qc_in; /* I know, its because qc_in is a misnomer */

	/* The queue is valid */
	if (qc_out->done) 
	{
		if (fcinfo->fd > 0) /* Network connection */
		{ 
			rbool = ENQUEUE_NRES(qc_out->queue, p_res);

			/*
			 * If it did not succeed or we need to flush, then we need to
			 * clear the buffer.
			 */
			if (!rbool || flush) 
			{
				qc_out->done = false;
				qc_out->pos = 0;

#ifdef DEBUG_SEND_QUEUES
				qc_out->queue->qid++;
				elog(DEBUG1, "result_conn_send: trying to send on fd- %d, seqno: %d, conn_idx: %d, prim: %d qid: %d",
					 fcinfo->fd, fcinfo->tmp_seqno, fcinfo->conn_idx, fcinfo->primary, qc_out->queue->qid);
#endif

			}

/* #define DEBUG_SEQNO_FIX */

#ifdef DEBUG_SEQNO_FIX
			if (rbool) 
			{
				/*
				if ((fcinfo->tmp_seqno != -1) && ((seqno - 4) != fcinfo->tmp_seqno)) {
					elog(DEBUG1, "Prev: %d, Curr: %d, Conn: %d, Rrim: %d -- NOT EQUAL!",
						 fcinfo->tmp_seqno, seqno, fcinfo->conn_idx, fcinfo->primary);
					sleep(2000);
					} else { */
					fcinfo->tmp_seqno = seqno;
					/*				} */
			}
#endif

			return rbool;

		} 
		else 
		{ /* Local connection */
			rbool = ENQUEUE_LRES(qc_out->queue, p_res);
			return rbool;
		}

	}

	/* If we get here, the queue is invalid */
	rval = send_queue_nb(qc_out, fcinfo->fd);

#ifdef DEBUG_SEND_QUEUES
	if (qc_out->done) 
	{
		elog(DEBUG1, "result_conn_send: Sent another queue on fd- %d, seqno: %d, conn_idx: %d, prim: %d",
			 fcinfo->fd, fcinfo->tmp_seqno, fcinfo->conn_idx, fcinfo->primary);
	}
#endif

	return (rval < 0) ? rval : false;
}
#endif /* USE_OLD_FCONN */

/* ftrans version */

/* Forward declarations I will need */ 
static int LocalTrans_send_result(LocalTrans *trans, result *p_res, bool flush);
static int LocalTrans_recv_result(LocalTrans *trans, result *p_res);
static int LocalTrans_send_ack(LocalTrans *trans, int val, bool flush);
static int LocalTrans_recv_ack(LocalTrans *trans, int *p_val);
static int SocketTrans_send_result(SocketTrans *trans, result *p_res, bool flush);
static int SocketTrans_recv_result(SocketTrans *trans, result *p_res);
static int SocketTrans_send_ack(SocketTrans *trans, int val, bool flush);
static int SocketTrans_recv_ack(SocketTrans *trans, int *p_val);
static int ftrans_send_result(ftransport *trans, result *p_res, bool flush);
static int ftrans_recv_result(ftransport *trans, result *p_res);
static int ftrans_send_ack(ftransport *trans, int val, bool flush);
static int ftrans_recv_ack(ftransport *trans, int *p_val);

int
fconn_send_result(FluxConnInfo *fcinfo, result *p_res, bool flush)
{
	if (FC_ERR(fcinfo)) return (-1);

	return ftrans_send_result(fcinfo->trans, p_res, flush);
}

int
fconn_recv_result(FluxConnInfo *fcinfo, result *p_res)
{
	if (FC_ERR(fcinfo)) return (-1);

	return ftrans_recv_result(fcinfo->trans, p_res);
}

int
fconn_send_ack(FluxConnInfo *fcinfo, int val, bool flush)
{
	if (FC_ERR(fcinfo)) return (-1);
		
	return ftrans_send_ack(fcinfo->trans, val, flush);
}

int
fconn_recv_ack(FluxConnInfo *fcinfo, int *val)
{
	if (FC_ERR(fcinfo)) return (-1);

	return ftrans_recv_ack(fcinfo->trans, val);
}

static void
queue_comm_flush(QueueComm *qc)
{
	if (qc->done)
	{
		qc->done = false;
		qc->pos  = 0;
	}
}

#ifdef USE_OLD_FCONN
/* Sets up the connection to be flushed */
void
conn_flush(FluxConnInfo_FT *fcinfo) 
{
	if (!fcinfo->local)
	{
		queue_comm_flush(fcinfo->qc_in);
	}
}
#endif /* USE_OLD_FCONN */

static void
SocketTrans_flush(SocketTrans *trans)
{
	queue_comm_flush(trans->ltrans.qc);
}

static void
SocketTrans_ack_flush(SocketTrans *trans)
{
	queue_comm_flush(trans->ltrans.qc_ack);
}

/* Marks data connection for flushing */
void
fconn_flush(FluxConnInfo *fcinfo)
{
	switch(ftransTag(fcinfo->trans))
	{
		case (TR_LocalTrans):
			break;
		case (TR_SocketTrans):
			SocketTrans_flush((SocketTrans *) fcinfo->trans);
			break;
		default:
			break;
	}
}

#ifdef USE_OLD_FCONN
void
ack_flush(FluxConnInfo_FT *fcinfo)
{
	if (!fcinfo->local)
	{
		queue_comm_flush(fcinfo->qc_ack);
	}
}
#endif /* USE_OLD_FCONN */

/* Marks ack connection for flushing */
void
fconn_ack_flush(FluxConnInfo *fcinfo)
{
	switch(ftransTag(fcinfo->trans))
	{
		case (TR_LocalTrans):
			break;
		case (TR_SocketTrans):
			SocketTrans_ack_flush((SocketTrans *) fcinfo->trans);
			break;
		default:
			break;
	}
}

/* 
   Prerequisite, you have already tried to send a tuple using
   result_conn_send with the flush option on true, or conn_flush has
   been called.
 */

#ifdef USE_OLD_FCONN
int
is_conn_flushed(FluxConnInfo_FT *fcinfo)
{
	int rval;
	QueueComm *qc_out = fcinfo->qc_in;

	if (qc_out->done) return true;
	rval = send_queue_nb(qc_out, fcinfo->fd);

#ifdef DEBUG_SEND_QUEUES
	if (qc_out->done) 
	{
		elog(DEBUG1, "is_conn_flushed: Sent another queue on fd- %d, seqno: %d, conn_idx: %d, prim: %d",
			 fcinfo->fd, fcinfo->tmp_seqno, fcinfo->conn_idx, fcinfo->primary);
	}
#endif

	return (rval < 0) ? rval : false;
}
#endif /* USE_OLD_FCONN */

static int
SocketTrans_is_flushed(SocketTrans *trans)
{
	int rval;
	LocalTrans *ltrans = (LocalTrans *) trans;
	
	if (ltrans->qc->done) return true;
	rval = send_queue_nb(ltrans->qc, trans->fd);

	return (rval < 0) ? rval : false;
}

static int
SocketTrans_is_ack_flushed(SocketTrans *trans)
{
	int rval;
	LocalTrans *ltrans = (LocalTrans *) trans;
	
	if (ltrans->qc_ack->done) return true;
	rval = send_queue_nb(ltrans->qc_ack, trans->fd);

	return (rval < 0) ? rval : false;
}

/* Pre-req: data connection has been marked for flushing */
/* Actiion: Tries to flushes data connection */
/* Post:    returns true if data connection is flushed */
int
fconn_is_flushed(FluxConnInfo *fcinfo)
{
	if (FC_ERR(fcinfo)) return (-1);
	
	switch(ftransTag(fcinfo->trans))
	{
		case (TR_LocalTrans):
			return true;
		case (TR_SocketTrans):
			return SocketTrans_is_flushed((SocketTrans *) fcinfo->trans);
		default:
			return false;
	}
}

#ifdef USE_OLD_FCONN
int
is_ack_flushed(FluxConnInfo_FT *fcinfo)
{
	int rval;
	QueueComm *qc_ack = fcinfo->qc_ack;

	if (qc_ack->done) return true;
	rval = send_queue_nb(qc_ack, fcinfo->fd);

	return (rval < 0) ? rval : false;
}
#endif /* USE_OLD_FCONN */

/* Pre-req: ack connection has been marked for flushing */
/* Actiion: Tries to flushes ack connection */
/* Post:    returns true if ack connection is flushed */
int
fconn_is_ack_flushed(FluxConnInfo *fcinfo)
{
	if (FC_ERR(fcinfo)) return (-1);
			
	switch(ftransTag(fcinfo->trans))
	{
		case (TR_LocalTrans):
			return true;
		case (TR_SocketTrans):
			return SocketTrans_is_ack_flushed((SocketTrans *) fcinfo->trans);
		default:
			return false;
	}
}

/* Returns true (1), if the item was "received".  Places the tuple
   into fcinfo->p_htup, if its a heaptuple. Only returns a new tuple
   if (fcinfo->p_htup == NULL).  p_res->data.tuple also points to the
   returned tuple.*/
/* Returns false (0), if the item was not "received" */
/* Returns      < 0 , if there was an error, or the connection is invalid */
#ifdef USE_OLD_CONN
int
result_conn_recv(FluxConnInfo_FT *fcinfo, result *p_res)
{

	QueueComm *qc_in;
	int rval, sockfd;
	bool rbool;

	if (FC_ERR(fcinfo)) return -1;

	/* Receive the tuple into p_htup */
	qc_in = fcinfo->qc_in;
	sockfd = fcinfo->fd;

	/* Try and get the input queue, if we do not have it already */
	if (!(qc_in->done)) 
	{
		rval = receive_queue_nb(qc_in, sockfd);
		
#ifdef DEBUG_RECV_QUEUES
		if (qc_in->done) 
		{
			elog(DEBUG1, "result_conn_recv: Received a queue on fd- %d, qid %d", 
				 sockfd, qc_in->queue->qid);
		}
#endif
		
		/* Note, side effect: qc_in->done == true, if we are done reading the queue */
		/* If rval < 0 we have an error */
		if (rval < 0) return rval;

		/* 
		 * Note, the qc_in is not valid if the queue was
		 * un-initialized or the queue size changed. Must return
		 * here. Owen suggests something else.
		 */
		return false;
	}

	/* Get tuple from incoming connection */
	if (fcinfo->p_htup == NULL) 
    {
		if (!fcinfo->local) /* This is a remote tuple */
		{
#ifdef DEBUG_TUPLES
			elog(DEBUG1, "Dequeueing a tuple."); 
#endif

			rbool = DEQUEUE_NRES(qc_in->queue, &p_res);

			/* Didn't dequeue a tuple, nothing left, must read from socket */
			if (!rbool) 
			{
				qc_in->done = false;
				qc_in->pos = 0;
				return false;
			}

#ifdef DEBUG_TUPLES
			elog(DEBUG1, "Dequeued finished.");
#endif
			
		} 
		else /* This is a local tuple */
		{ 
			/* XXX This ugliness means, we cannot use buffer pool
			   tuples. Unless we do a copy here */
			rbool = DEQUEUE_LRES(qc_in->queue, &p_res);

			if (!rbool) 
				return false;

		}

		if (p_res->type == RESULT_HEAPTUPLE) 
		{
#ifdef DEBUG_TUPLES
			elog(DEBUG1, "Assigning a tuple."); 
#endif
			fcinfo->p_htup = p_res->data.tuple;
		}
	}
	/* Done receiving the tuple */
	return true;
}
#endif /* USE_OLD_CONN */

/* Queue serialization and deserialization routines */
 
int  determineIntSize(char *obj, char *extrainfo);
bool serializeInt(char *src, char *dest, int destlen, char *extrainfo);
bool deserializeInt(char *src, char **dest, char *extrainfo);

int  determineTTSSize(char *obj, char *extrainfo);
bool serializeTTS(char *src, char *dest, int destlen, char *extrainfo);
bool deserializeTTS(char *src, char **dest, char *extrainfo);
bool serializeTTS_c(char *src, char *dest, int destlen, char *extrainfo);

int  determineRLocalSize(char *obj, char *extrainfo);
bool serializeRLocal(char *src, char *dest, int destlen, char *extrainfo);
bool deserializeRLocal(char *src, char **dest, char *typeinfo);

queue_serializer int_serializer = {
	determineIntSize,
	serializeInt,
	deserializeInt
};

/* 
 * Serialize contents of TupleTableSlot onto for passing regular
 * postgres tuple between modules.
 */
queue_serializer tts_serializer = {
	determineTTSSize,
	serializeTTS,
	deserializeTTS
};

/* Makes a copy of the tuple before serializing, needed by FluxProd */
queue_serializer tts_serializer_c = {

	determineTTSSize,
	serializeTTS_c,
	deserializeTTS

};

queue_serializer rlocal_serializer_c = {

	determineRLocalSize,
	serializeRLocal,
	deserializeRLocal

};

int
determineRLocalSize(char *obj, char *extrainfo) {
	return sizeof(result);
}

bool
serializeRLocal(char *src, char *dest, int destlen, char *extrainfo) {

	result *elt = (result *)src;
	result *d_res = (result *)dest;
	
/* 	memcpy(dest, &(elt->type), sizeof(ResultType)); */
/* 	dest += sizeof(ResultType); */

	/* Fill in the structure, don't copy bits of it. */
	d_res->type = elt->type;
		
	switch(elt->type) {
		
		case RESULT_HEAPTUPLE:
			d_res->data.tuple = heap_copytuple(elt->data.tuple);
			break;

		case RESULT_FPRODALIVE:
			d_res->data.info = elt->data.info;
			break;
			
		case RESULT_FPRODSYNC:
			d_res->data.info = elt->data.info;
			break;

		case RESULT_PAUSEACK:
		case RESULT_FCONSSYNC:
		case RESULT_DONE:
			break;

		default:
			elog(ERROR, "Can't serialize that type");
			break;
	}
	
	return true;
}

bool 
deserializeRLocal(char *src, char **dest, char *typeinfo) {

	/* XXX Fill in the structure, don't copy bit by bit */
	result  *elt  = (result *) src;
	result **pp_r = (result **) dest;

	(*pp_r)->type = elt->type;
	
	switch(elt->type) {

		case RESULT_HEAPTUPLE:
			/* We can do this because it was copied on the enqueue. */
			(*pp_r)->data.tuple = elt->data.tuple;
			break;

		case RESULT_FPRODALIVE:
			(*pp_r)->data.info = elt->data.info;
			break;
			
		case RESULT_FPRODSYNC:
			(*pp_r)->data.info = elt->data.info;
			break;
			
		case RESULT_PAUSEACK:
		case RESULT_FCONSSYNC:
		case RESULT_DONE:
			break;

		default:
			elog(ERROR, "Can't deserialize that type");
			break;
	}

	return true;
}

#define TTSSIZE (sizeof(Buffer) + sizeof(HeapTuple))

int
determineTTSSize(char *obj, char *extrainfo) {
	/* Just need to serialze the heap tuple pointer and the buffer */
   return TTSSIZE;
}

bool 
serializeTTS(char *src, char *dest, int destlen, char *extrainfo) {

	TupleTableSlot  *slot;
	HeapTuple     *ht_ptr;
	Buffer         *b_ptr;

	/* Assume enough space in the destination */

	slot    = (TupleTableSlot *) src;
	ht_ptr  = (HeapTuple *) dest;
	b_ptr   = (Buffer *) (dest + sizeof(HeapTuple));
	*ht_ptr = slot->val;
	*b_ptr  = slot->ttc_buffer;

	/* Since the heap tuple is passed to someone else, this slot 
	   should no longer have the responsibility of freeing it */

	slot->ttc_shouldFree = false;
	return true;
}

bool 
serializeTTS_c(char *src, char *dest, int destlen, char *extrainfo) {

	TupleTableSlot  *slot;
	HeapTuple     *ht_ptr;
	Buffer         *b_ptr;

	/* Assume enough space in the destination */

	slot    = (TupleTableSlot *) src;
	ht_ptr  = (HeapTuple *) dest;
	b_ptr   = (Buffer *) (dest + sizeof(HeapTuple));
	*ht_ptr = heap_copytuple(slot->val);
	*b_ptr  = InvalidBuffer;

	/* The slot was copied, so the ownership of the slot does not change */
	return true;
}

bool 
deserializeTTS(char *src, char **dest, char *typeinfo) {

	TupleTableSlot **slot_ptr;
	HeapTuple         *ht_ptr;
	Buffer             *b_ptr;

	slot_ptr = (TupleTableSlot **) dest;

	ht_ptr  = (HeapTuple *) src;
	b_ptr   = (Buffer *) (src + sizeof(HeapTuple));

	/* Really shouldn't make new slots, but we can't just die here. */
	if ((*slot_ptr) == NULL)
		(*slot_ptr) = MakeTupleTableSlot();

	/* This should copy the contents, without need of a function call to memcpy */
	(*slot_ptr)->val = *ht_ptr;
	(*slot_ptr)->ttc_buffer = *b_ptr;

	/* 
	 * If this is from the buffer pool, shouldFree should be false.
	 * otherwise, we own it.
	 */
	(*slot_ptr)->ttc_shouldFree =
		(BufferIsValid((*slot_ptr)->ttc_buffer)) ? false : true;

	return true;
}


int
determineIntSize(char *obj, char *extrainfo) {
	/* Just need to serialze the heap tuple pointer and the buffer */
	return (sizeof(int));
}

bool
serializeInt(char *src, char *dest, int destlen, char *extrainfo) {
	
	int *pi_src   = (int *) src;
	int *pi_dest  = (int *) dest;

	*pi_dest = *pi_src;
	return true;
}

bool
deserializeInt(char *src, char **dest, char *extrainfo) {
	
	int *pi_src   = (int *) src;
	int *pi_dest  = (int *) dest;

	*pi_dest = *pi_src;
	return true;
}

sm_request *
new_smr(int mach, bool *p_ready, Queue *move_q, smr_t action) 
{

	sm_request *ret = palloc(sizeof(sm_request));

	ret->mach       = mach;
	ret->ready_flag = p_ready;
	ret->state_q    = move_q;
	ret->action     = action;
	ret->mode       = NEW_REQUEST;

	return ret;
}

void
reset_qcomm(QueueComm *qc) 
{
#ifdef DRQ
	elog(DEBUG1, "XXXXXXXXXXXXXXXXXXX RESET QCOMM XXXXXXXXXXXXXXX");
#endif
	qc->queue = NULL;
	qc->size  = -1;
	qc->pos   = 0;
	qc->done  = false;
}

/* Transport routines */
ftransport *
newftrans(Size size, TransTag tag)
{
	ftransport	   *newTrans;

	Assert(size >= sizeof(ftransport));		/* need the tag, at least */

	newTrans = (ftransport *) palloc(size);
	MemSet((char *) newTrans, 0, size);
	newTrans->type = tag;
	return newTrans;
}

void
initLocalTrans(LocalTrans *ltrans, Queue *q, Queue *q_ack)
{
	ltrans->qc = newQueueComm(q);
	ltrans->qc_ack = newQueueComm(q_ack);
}

ftransport *
newLocalTrans(void)
{
	LocalTrans *ltrans;
	Queue *q;
	Queue *q_ack;

	ltrans = makeftrans(LocalTrans);

	/* Make both queues */
	make_queue(&(q), *p_no_lock, flux_queue_size);
	make_queue(&(q_ack), *p_no_lock, ack_queue_size);
	
	initLocalTrans(ltrans, q, q_ack);
	return (ftransport *) ltrans;
}

ftransport *
newLocalTrans_q(Queue *q, Queue *q_ack)
{
	LocalTrans *ltrans;

	ltrans = makeftrans(LocalTrans);
	initLocalTrans(ltrans, q, q_ack);

	return (ftransport *) ltrans;
}

void
initSocketTrans(SocketTrans *strans, int fd, struct sockaddr_in *addr_in)
{
	strans->fd      = fd;
	strans->addr_in = addr_in;
}

ftransport *
newSocketTrans(int fd, struct sockaddr_in *addr_in)
{
	SocketTrans *strans;
	Queue *q;
	Queue *q_ack;

	strans = makeftrans(SocketTrans);
	
	/* Make both queues */
	make_queue(&(q), *p_no_lock, flux_queue_size);
	make_queue(&(q_ack), *p_no_lock, ack_queue_size);

	initLocalTrans((LocalTrans *) strans, q, q_ack);
	initSocketTrans(strans, fd, addr_in);

	return (ftransport *) strans;
}

static void
printLocalTransInfo(LocalTrans *ltrans)
{
	elog(DEBUG1, "qc:            %x", (uint) ltrans->qc);
	elog(DEBUG1, "qc_ack:        %x", (uint) ltrans->qc_ack);
	elog(DEBUG1, "qc->queue:     %x", ((uint) ((ltrans->qc) ? (ltrans->qc->queue) : 0)));
	elog(DEBUG1, "qc_ack->queue: %x", ((uint) ((ltrans->qc) ? (ltrans->qc->queue) : 0)));
}

#define MAX_ADDR_LEN 256

static void
printSocketTransInfo(SocketTrans *strans)
{
	char addr_str[MAX_ADDR_LEN];
	
	elog(DEBUG1, "fd:            %d", strans->fd);
	elog(DEBUG1, "ip_addr:       %s", (strans->addr_in) ?
		 inet_ntop(AF_INET, &(strans->addr_in->sin_addr), addr_str, MAX_ADDR_LEN) : "NULL" );
	elog(DEBUG1, "port:          %d", (strans->addr_in) ?
		 ntohs(strans->addr_in->sin_port) : (-1) );
}

void
printTransInfo(ftransport *ftrans)
{
	switch(ftransTag(ftrans))
	{
		case (TR_LocalTrans):
			elog(DEBUG1, "-----------------");
			elog(DEBUG1, "Local transport: ");
			printLocalTransInfo((LocalTrans *)ftrans);
			elog(DEBUG1, "-----------------");
			break;
			
		case (TR_SocketTrans):
			elog(DEBUG1, "-----------------");
			elog(DEBUG1, "Socket transport:");
			printSocketTransInfo((SocketTrans *)ftrans);
			elog(DEBUG1, "-----------------");
			break;
			
		default:
			elog(DEBUG1, "------------------");
			elog(DEBUG1, "Unknown transport:");
			elog(DEBUG1, "------------------");
			break;
	}
}

/* send and recv routines */
static int
LocalTrans_send_result(LocalTrans *trans, result *p_res, bool flush)
{
	return ENQUEUE_LRES(trans->qc->queue, p_res);
}

/* Pre-req: (p_res != NULL) */
static int
LocalTrans_recv_result(LocalTrans *trans, result *p_res)
{
	return DEQUEUE_LRES(trans->qc->queue, &p_res);
}

static int
LocalTrans_send_ack(LocalTrans *trans, int val, bool flush)
{
	return ENQUEUE_INT(trans->qc_ack->queue, &val);
}

/* Pre-req: (p_val != NULL) */
static int
LocalTrans_recv_ack(LocalTrans *trans, int *p_val)
{
	/* Special case, don't need &p_val */
	return DEQUEUE_INT(trans->qc_ack->queue, p_val);
}

static int
SocketTrans_send_result(SocketTrans *trans, result *p_res, bool flush)
{
	int  rval;
	LocalTrans  *ltrans;
	
	ltrans = (LocalTrans *) trans;
	
	/* Queue is invalid */
	if (!(ltrans->qc->done))
	{
		/* Send queue on the wire. */
		rval = send_queue_nb(ltrans->qc, trans->fd);

		/* Note, side effect: qc->done == true if done sending all of queue */
		return (rval < 0) ? rval : false;
	}

	/* Queue is valid --- enqueue onto queue */
	rval = ENQUEUE_NRES(ltrans->qc->queue, p_res);
	
	/*
	 * If it did not succeed or we need to flush, then we need to
	 * send the buffer.
	 */
	if (!rval || flush)
	{
		queue_comm_flush(ltrans->qc);
	}

	return rval;
}

/* Pre-req: (p_res != NULL) */
static int
SocketTrans_recv_result(SocketTrans *trans, result *p_res)
{
	int  rval;
	LocalTrans  *ltrans;

	ltrans = (LocalTrans *) trans;
	
	/* Queue is invalid */
	if (!(ltrans->qc->done))
	{
		rval = receive_queue_nb(ltrans->qc, trans->fd);

		/* Note, side effect: qc->done == true if done receiving all of queue */
		return (rval < 0) ? rval : false;
	}
	
	rval = DEQUEUE_NRES(ltrans->qc->queue, &p_res);

	/* Didn't dequeue a tuple, nothing left, must read from socket */
	if (!rval)
	{
		queue_comm_flush(ltrans->qc);
	}

	return rval;
}

static int
SocketTrans_send_ack(SocketTrans *trans, int val, bool flush)
{
	int  rval;
	LocalTrans  *ltrans;

	ltrans = (LocalTrans *) trans;
	
	/* Queue is invalid */
	if (!(ltrans->qc_ack->done))
	{
		/* Send queue on the wire. */
		rval = send_queue_nb(ltrans->qc_ack, trans->fd);

		/* Note, side effect: qc->done == true if done sending all of queue */
		return (rval < 0) ? rval : false;
	}

	/* Queue is valid --- enqueue onto queue */
	rval = ENQUEUE_INT(ltrans->qc_ack->queue, &val);
	
	/*
	 * If it did not succeed or we need to flush, then we need to
	 * send the buffer.
	 */
	if (!rval || flush)
	{
		queue_comm_flush(ltrans->qc_ack);
	}

	return rval;
}

/* Pre-req: (p_val != NULL) */
static int
SocketTrans_recv_ack(SocketTrans *trans, int *p_val)
{
	int  rval;
	LocalTrans  *ltrans;

	ltrans = (LocalTrans *) trans;
	
	/* Queue is invalid */
	if (!(ltrans->qc_ack->done))
	{
		rval = receive_queue_nb(ltrans->qc_ack, trans->fd);

		/* Note, side effect: qc->done == true if done receiving all of queue */
		return (rval < 0) ? rval : false;
	}
	
	rval = DEQUEUE_INT(ltrans->qc_ack->queue, p_val);

	/* Didn't dequeue a tuple, nothing left, must read from socket */
	if (!rval)
	{
		queue_comm_flush(ltrans->qc_ack);
	}

	return rval;
}

static int
ftrans_send_result(ftransport *trans, result *p_res, bool flush)
{
	switch(ftransTag(trans))
	{
		case (TR_LocalTrans):
			return LocalTrans_send_result((LocalTrans *) trans, p_res, flush);
			break;
		case (TR_SocketTrans):
			return SocketTrans_send_result((SocketTrans *) trans, p_res, flush);
			break;
		default:
			break;
	}

	return (-1);
}

static int
ftrans_recv_result(ftransport *trans, result *p_res)
{
	switch(ftransTag(trans))
	{
		case (TR_LocalTrans):
			return LocalTrans_recv_result((LocalTrans *) trans, p_res);
			break;
		case (TR_SocketTrans):
			return SocketTrans_recv_result((SocketTrans *) trans, p_res);
			break;
		default:
			break;
	}

	return (-1);
}

static int
ftrans_send_ack(ftransport *trans, int val, bool flush)
{
	switch(ftransTag(trans))
	{
		case (TR_LocalTrans):
			return LocalTrans_send_ack((LocalTrans *) trans, val, flush);
			break;
		case (TR_SocketTrans):
			return SocketTrans_send_ack((SocketTrans *) trans, val, flush);
			break;
		default:
			break;
	}

	return (-1);
}

static int
ftrans_recv_ack(ftransport *trans, int *p_val)
{
	switch(ftransTag(trans))
	{
		case (TR_LocalTrans):
			return LocalTrans_recv_ack((LocalTrans *) trans, p_val);
			break;
		case (TR_SocketTrans):
			return SocketTrans_recv_ack((SocketTrans *) trans, p_val);
			break;
		default:
			break;
	}

	return (-1);
}

/* Warning, these reset methods will not free pointers that are in the queues */
static void
LocalTrans_reset(LocalTrans *trans)
{
	/* need to free the tuples */
	if (ftransIsA(trans, LocalTrans))
	{
		result *p_res;
		int     rval = true;
		
		do
		{
			rval = LocalTrans_recv_result(trans, p_res);
			if ((rval) &&
				(p_res->type == RESULT_HEAPTUPLE) &&
				(p_res->data.tuple))
			{
				heap_freetuple(p_res->data.tuple);
			}
			
		} while (rval);
	}
	
	clear_queue(trans->qc->queue);
	clear_queue(trans->qc_ack->queue);
	trans->qc->done = true;
	trans->qc_ack->done = true;
	trans->qc->pos = 0;
	trans->qc_ack->pos = 0;
}

static void
SocketTrans_reset(SocketTrans *trans)
{
	LocalTrans_reset((LocalTrans *)trans);

	if (trans->fd > 0)
	{
		close(trans->fd);
	}
	
	trans->fd = -1;

	if (trans->addr_in)
	{
		pfree(trans->addr_in);
		trans->addr_in = NULL;
	}
}

void
ftrans_reset(ftransport *trans)
{
	switch(ftransTag(trans))
	{
		case (TR_LocalTrans):
			return LocalTrans_reset((LocalTrans *) trans);
			break;
		case (TR_SocketTrans):
			return SocketTrans_reset((SocketTrans *) trans);
			break;
		default:
			break;
	}
}

#ifdef USE_OLD_FCONN
/* Debugging routines */
void
print_conn_info(FluxConnInfo_FT *fcinfo)
{

	elog(DEBUG1, "fd: %d",     fcinfo->fd);
	elog(DEBUG1, "error: %d",  fcinfo->error);
	elog(DEBUG1, "local: %d",  fcinfo->local);
	elog(DEBUG1, "qc_in: %x",  (uint) fcinfo->qc_in);
	elog(DEBUG1, "qc_ack: %x", (uint) fcinfo->qc_ack);

	if (fcinfo->qc_in)
		elog(DEBUG1, "qc_in->queue: %x",  (uint) fcinfo->qc_in->queue);

	if (fcinfo->qc_ack)
		elog(DEBUG1, "qc_ack->queue: %x",  (uint) fcinfo->qc_ack->queue);
	
}
#endif

void
fconn_print_info(FluxConnInfo *fcinfo)
{
	elog(DEBUG1, "------------------------------------------------------");
	elog(DEBUG1, "FluxConnInfo [%d, %s, %d] : [part_no, role, conn_idx] ",
		 fcinfo->part_no, ROLE_STR(fcinfo->role),  fcinfo->conn_idx);
	printTransInfo(fcinfo->trans);
	elog(DEBUG1, "[%d, %d] : [prev_seqno, tmp_seqno]",
		 fcinfo->prev_seqno, fcinfo->tmp_seqno);
	elog(DEBUG1, "------------------------------------------------------");
}

void
dump_int_queue(Queue *q) {

	int num;

	elog(DEBUG1, "Begin qid: %d", q->qid);
	while (DEQUEUE_INT(q, &num)) {
		elog(DEBUG1,"Got int: %d", num);
	}
	elog(DEBUG1, "End qid: %d", q->qid);
	
}

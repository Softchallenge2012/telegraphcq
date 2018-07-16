/*-------------------------------------------------------------------------
 *
 * fluxroutines.h
 *
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/telegraphcq/fluxroutines.h,v 1.23 2004/07/15 20:39:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef FLUXROUTINES_H
#define FLUXROUTINES_H

#include "nodes/plannodes.h"
#include "nodes/execnodes.h"
#include "rqueue/rqdest.h"
#include "telegraphcq/fluxmerge.h"

extern queue_serializer int_serializer;
extern queue_serializer tts_serializer;
extern queue_serializer tts_serializer_c;
extern queue_serializer rlocal_serializer_c;

#define TV_IN_MSEC_DOUBLE(tv) (((double) tv.tv_sec * 1e03) + ((double) tv.tv_usec * 1.0e-3))

/*
 * For slot info for tuples from same machine (could be in the buffer pool).
 */
#define ENQUEUE_INT(q, p_int) enqueue(q, (char *)p_int, false, &int_serializer)
#define DEQUEUE_INT(q, p_int) dequeue(q,(char **)p_int, false, &int_serializer)

/*
 * For network heap tuple from another machine. (Always InvalidBuffer).
 */
#define ENQUEUE_NHT(q, ht)	enqueue(q,	(char *)ht, false, &tuple_serializer)
#define DEQUEUE_NHT(q, ret) dequeue(q,(char **)ret, false, &tuple_serializer)

/*
 * For slot info for tuples from same machine (could be in the buffer pool).
 */
#define ENQUEUE_TTS(q, tts) enqueue(q, (char *)tts, false, &tts_serializer)
#define DEQUEUE_TTS(q, ret) dequeue(q,(char **)ret, false, &tts_serializer)

/*
 * For slot info for tuples from same machine (could be in the buffer
 * pool).  This makes a copy in case the enqueue wants to hold on to
 * the tuple for a while.
 */
#define ENQUEUE_TTS_C(q, tts) enqueue(q, (char *)tts, false, &tts_serializer_c)

/*
 * Result enqueue and dequeues
 */

/* Network queues */
#define ENQUEUE_NRES(q, p_r) enqueue(q, (char *) p_r, false, &result_serializer)
#define DEQUEUE_NRES(q, pp_r) dequeue(q, (char **) pp_r, false, &result_serializer)

/* Local queues */
#define ENQUEUE_LRES(q, p_r) enqueue(q, (char *) p_r, false, &rlocal_serializer_c)
#define DEQUEUE_LRES(q, pp_r) dequeue(q, (char **) pp_r, false, &rlocal_serializer_c)

/* Calculating q sizes using elements and their sizes */
#define QSTRUCTSIZE sizeof(Queue)
#define CALC_QSIZE(nelt, elt_type) ((nelt*sizeof(elt_type)) + QSTRUCTSIZE)

/* Macros for figuring out machine locations, rotates around the beginning and end */

/* If I am at machine == id, returns next machine (go right) in sequence */
#define ROTATE_R(id, max)  ((id + 1) % max)

/* If I am at machine == id, returns previous machine (go left) in sequence */
#define ROTATE_L(id, max)  ((max + id - 1) % max)

/*
 * Markers sent back in the ack stream. Must be < 0 because sequence nos are
 * used for tuples.
 */
#define ACK_MARKER     -99   /* Some constant that indicates end of acks */
#define ACK_FPRODSYNC  -100
#define ACK_FCONSSYNC  -101
#define ACK_PAUSE      -102  /* Misnomer, actually used to indicate the start of
								a pause sequence */

#define ACK_FRUNNING   -103  /* Still running with valid state */
#define ACK_FSTANDBY   -104  /* Waiting for some valid state   */


#define FC_CONN(fcinfo)      (fcinfo->status >= 0)
#define FC_ERR(fcinfo)       (fcinfo->status < 0)
#define FC_DCONN(fcinfo)     (fcinfo->status == CONN_DISCONN)

/* Useful functions */
extern double f_time_since(struct timeval *start_tv);

/* Connection location routines */

#ifdef USE_OLD_FCONN
extern void init_empty_conn(FluxConnInfo_FT * fcinfo);
extern int  init_conn_info(FluxConnInfo_FT * fcinfo, int sockfd,
						   Queue * q_out, int q_out_size,
						   Queue * q_ack, int q_ack_size);
#endif /* USE_OLD_FCONN */

extern void fconn_init_empty(FluxConnInfo *fcinfo);

extern int	index_of_mach(int *map_c2m, int size, int mach);

extern void smark_conn(FluxConnInfo *fcinfo, int next_state,
					   ResultType type, int info);

/* Result sending and receiving routines */

/* #define DEBUG_SEQNO_HEADER */

#ifdef USE_OLD_FCONN
#ifdef DEBUG_SEQNO_HEADER
extern int  result_conn_send(FluxConnInfo_FT *fcinfo, result *p_res, int flush, int seqno);
#else
extern int  result_conn_send(FluxConnInfo_FT *fcinfo, result *p_res, int flush);
#endif

extern int  result_conn_recv(FluxConnInfo_FT *fcinfo, result *p_res);
extern void conn_flush(FluxConnInfo_FT *fcinfo);
extern void ack_flush(FluxConnInfo_FT *fcinfo);
extern int  is_conn_flushed(FluxConnInfo_FT *fcinfo);
extern int  is_ack_flushed(FluxConnInfo_FT *fcinfo);

/* Debugging routines */
extern void print_conn_info(FluxConnInfo_FT * fcinfo);
#endif /* USE_OLD_FCONN */

extern int 		fconn_send_result(FluxConnInfo *fcinfo, result *p_res, bool flush);
extern int 		fconn_recv_result(FluxConnInfo *fcinfo, result *p_res);
extern int 		fconn_send_ack(FluxConnInfo *fcinfo, int val, bool flush);
extern int 		fconn_recv_ack(FluxConnInfo *fcinfo, int *val);
extern void 	fconn_flush(FluxConnInfo *fcinfo);
extern void 	fconn_ack_flush(FluxConnInfo *fcinfo);
extern int 		fconn_is_flushed(FluxConnInfo *fcinfo);
extern int      fconn_is_ack_flushed(FluxConnInfo *fcinfo);

/* Failure routines */
extern void set_failed_conn(Plan *node, FluxConnInfo * fcinfo,
							int eval, int conn_idx);

extern void reset_qcomm(QueueComm *qc);

extern void dump_int_queue(Queue *q);

/* Index in the fluxen */
#define STATE_MOVER_IDX 0

/* An machine we don't know about, call it a spare */
#define SPARE_CONN_IDX	 -1
#define UNKNOWN_CONN_IDX -2

/* Unspecified machine */
#define UNKNOWN_MACHINE -1

#define IsSending(fcinfo) \
        (((fcinfo)->status == CONN_SEND_MARKER) || \
		 ((fcinfo)->status == CONN_FLUSHBUF))

#define IsAlive(fcinfo) \
        (((fcinfo)->status != CONN_RECOVER) && \
         ((fcinfo)->status != CONN_UNKNOWN))

#define IsSendingData(fcinfo) \
        (((fcinfo)->status == CONN_SEND_DATA) || \
         ((fcinfo)->status == CONN_SEND_ACK_DATA))

#define getSM(node, _type_) \
        ((StateMover *) ((_type_ *) node)->sched->fluxen[STATE_MOVER_IDX])

typedef enum smr_t
{
	SMR_RECV_BUF = 0,
	SMR_SEND_BUF,
}	smr_t;

typedef enum SmMode
{
	NEW_REQUEST = 0,
	SM_RECV_BUF,
	SM_SEND_BUF,
	SM_SEND_EOF,
} SmMode;

typedef struct sm_request
{
	int			mach;
	bool	   *ready_flag;		/* Set the flag */
	Queue	   *state_q;
	smr_t		action;
	SmMode      mode;
}	sm_request;

sm_request *new_smr(int mach, bool *p_ready, Queue * move_q, smr_t action);

extern void init_mergeCB(CMerge *merge, int nent);
extern void init_snapshotCB(SnapshotCB *snap, int nent);
extern void init_syncCB(SyncCB *sync, int nent);
extern void init_pauseCB(PauseCB *pauses, int nent);


extern int vmach_2_tnodeid(int vmach);   /* map from virtual machine to telegraph_nodeid */
extern int tnodeid_2_vmach(int tnodeid); /* map from telegraph_nodeid to virtual machine */


/* So we can have different types of transports for these connections */
typedef enum TransTag
{
	TR_Invalid = 0,
	TR_LocalTrans = 10,
	TR_SocketTrans
} TransTag;

/* Encapsulates the transport used to send to the endpoint */
/* Flux transport */
typedef struct ftransport
{
	TransTag  type;
} ftransport;

#define makeftrans(_type_)        		((_type_ *) newftrans(sizeof(_type_),TR_##_type_))
#define ftransTag(nodeptr)				(((ftransport*)(nodeptr))->type)
#define ftransIsA(nodeptr,_type_)		(ftransTag(nodeptr) == TR_##_type_)
#define ftransIsLocal(nodeptr)			(ftransIsA(nodeptr, LocalTrans))

#define ROLE_STR(_role_) (((_role_) == F_PRIMARY) ? "PRIM" : "SEC")

extern ftransport * newftrans(Size size, TransTag tag);
extern void fconn_init_info(FluxConnInfo *fcinfo, ftransport *trans,
							int cidx, int pno, FluxRole mrole);

typedef struct LocalTrans
{
	ftransport   ftrans;
	QueueComm    *qc;
	QueueComm    *qc_ack;
} LocalTrans;

typedef struct SocketTrans
{
	LocalTrans     		 ltrans;
	int                  fd;            /* Socket descriptor  */
	struct sockaddr_in 	*addr_in;		/* inet address info. */
} SocketTrans;

typedef struct MultConnSocketTrans
{
	SocketTrans         strans;
	/* Probably need some state here to handle multiple FluxConn's */
} MultConnSocketTrans;

extern void initLocalTrans(LocalTrans *ltrans, Queue *q, Queue *q_ack);
extern ftransport *newLocalTrans(void);
extern ftransport *newLocalTrans_q(Queue *q, Queue *q_ack);
extern void initSocketTrans(SocketTrans *strans, int fd, struct sockaddr_in *addr_in);
extern ftransport *newSocketTrans(int fd, struct sockaddr_in *addr_in);
extern void printTransInfo(ftransport *ftrans);
extern void ftrans_reset(ftransport *trans);
extern void fconn_print_info(FluxConnInfo *fcinfo);



#endif   /* FLUXROUTINES_H */












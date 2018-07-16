/*-------------------------------------------------------------------------
 *
 * wraphch.h
 *
 * This file declares the functions that make up the implementation of
 * the wrapper clearinghouse. This essentially is a separate process that
 * performs the functions of asynchronous I/O.
 *
 * Portions Copyright (c) 2002, Regents of the University of California
 *
 * $Id: wrapch.h,v 1.30 2005/06/13 23:45:25 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef WRAPCH_H
#define WRAPCH_H

#include "fmgr.h"

#include "access/htup.h"
#include "nodes/primnodes.h"
#include "parser/parse_wrapper.h"
#include "rqueue/ingresq.h"
#include "sys/poll.h"
#include "utils/fcache.h"
#include "userwrapper.h"
#include "telegraphcq/triageq.h"

#define NUM_INGRES_QUEUES 64
#define MAX_CONN NUM_INGRES_QUEUES		/* For now, this is the maximum
										 * number of incomine connections
										 * we handle */

#define DEF_INGRES_QSIZE (1024)*100		/* Defualt Q size for ingres
										 * queue. */
#define DEF_WRAPPER_PORT 5533	/* Just picked a number for default port. */
#define ARCHIVED_TUPARRAYSIZE 1

#define PERTUPLECONTEXTSIZE 500*1024
#define PERCONNECTIONCONTEXTSIZE 1024*1024
/* wrapper clearinghouse notion of source */

/* Temporary memory context for use within the wrapper clearinghouse; cleared
 * every time through the wrapper clearinghouse main loop. */
extern MemoryContext wch_tmpcontext;

/* The types of stream we support. 
 *
 * TODO: Should we use the (character) constants RELKIND_UNARCHIVEDSTREAM et al
 * here?
 */
typedef enum StreamType {
    ST_ARCHIVED,

    ST_UNARCHIVED_SPILL,
        /* Spill to disk if the query processor falls behind. */
        
    ST_UNARCHIVED_DROP
        /* Drop tuples if the query processor falls behind. */
} StreamType;


typedef struct unarchivedStreamInfo
{
	Queue	   *inQ;			/* Queue to the executor */
	IngresQIdx	idx;			/* Index of the ingres queue */
    TriageQueue *triageq;       /* A lossy priority queue; drops tuples as
                                 * needed to avoid swamping the query engine.
                                 *
                                 * Only used if this is an UNARCHIVED_DROP
                                 * stream; NULL otherwise.
                                 */
    /* We use archived streams for the summaries, so we need to keep their
     * relids instead of their input queues here. */
    Oid         drop_sum_oid;
    Oid         keep_sum_oid;

    Relation    drop_sum_rel;
    Relation    keep_sum_rel;

    int         summarytype;
        /* The type of summary we keep of triaged tuples.  Can be
         * OVERLOAD_BLOCK, OVERLOAD_DROP, OVERLOAD_COUNTS, 
         * OVERLOAD_REGHIST, OVERLOAD_MHIST, or OVERLOAD_WAVELET */

}	unarchivedStreamInfo;

typedef struct archivedStreamInfo
{
	Oid			reloid;
	int			tail;
	int			head;
	bool		forceflush;
	HeapTuple	tupArray[ARCHIVED_TUPARRAYSIZE];
	Relation    r;
}	archivedStreamInfo;

typedef struct _aioconn
{
	wrapper    *wrapper;
	wrapperstate wrapperstate;
	char	   *wrappername;
	List	   *streamname;
	Oid			streamoid;
	StreamType  streamtype;
	unarchivedStreamInfo *uinfo;
	archivedStreamInfo *ainfo;
	int			npends;			/* How many times to spin on the queue,
								 * before dequeing */
	HeapTuple	tuple;			/* processed, but as yet undelivered tuple */
	bool		needsUnlock;	/* was the relation locked */

	Queue	   *pending_probes; /* a queue of HeapTuples which represent
								 * probes to a pull source. NULL if this
								 * is a push source */
	int			batchsize;

	MemoryContext perTupleContext;
	MemoryContext perConnectionContext;
  bool isPullWrapper;
}	aioconn;


extern int	acceptPort;
extern char *telegraphcqwchport;


void		AbortWrapCH(void);
int			WrapCHMain(int argc, char *argv[]);

extern unarchivedStreamInfo *setupUnarchivedStreamInfo(List *streamname,
        StreamType type, Oid streamoid);
extern void setSocketOptions(int sock);

enum AuxNameType  
{
    ANT_DROPPED,
    ANT_KEPT,
    ANT_DROPPED_COUNT,
    ANT_KEPT_COUNT,
};

#define SET_STREAM_NAME_GENERIC(buf, len, relname, suffix)      \
        snprintf((buf), (len), "__%s_" suffix, (relname))

#define SET_DROPPED_STREAM_NAME(buf, len, relname)      \
        SET_STREAM_NAME_GENERIC(buf, len, relname, "dropped") 

#define SET_KEPT_STREAM_NAME(buf, len, relname)      \
        SET_STREAM_NAME_GENERIC(buf, len, relname, "kept") 

#define SET_DROPPED_COUNT_STREAM_NAME(buf, len, relname)      \
        SET_STREAM_NAME_GENERIC(buf, len, relname, "dropped_count") 

#define SET_KEPT_COUNT_STREAM_NAME(buf, len, relname)      \
        SET_STREAM_NAME_GENERIC(buf, len, relname, "kept_count") 

RangeVar * makeAuxStreamName(const RangeVar *orig_relation, 
        enum AuxNameType type);

/* Constant defined in wrapch.h */
extern struct timeval WRAPCH_NOT_A_TIMEVAL;

#define WRAPCH_IS_A_TIME(tv) (0LL != tv_diff(&WRAPCH_NOT_A_TIMEVAL, (tv)))

#endif   /* WRAPCH_H */

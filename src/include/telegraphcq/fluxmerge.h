/*-------------------------------------------------------------------------
 *
 * fluxmerge.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * Used for merging tuple streams.
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/telegraphcq/fluxmerge.h,v 1.7 2004/03/05 21:10:18 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef FLUXMERGE_H
#define FLUXMERGE_H

#include "c.h"
#include "access/htup.h"

typedef int32 seq_num;          /* Change this if you want later */

typedef struct CMerge 
{
	int          cpos;          
	HeapTuple   *htup;          /* Heaptuples */
	seq_num     *snum;          /* Sequence number of current htup */
	bool        *ready;         /* Wether this tuple is ready for removal */
	int          nfill;         /* Number of positions filled */

	bool         use_buf;       /* Are we using the buffer? */
	bool         move_to_buf;   /* Are we moving from the buffer? */
} CMerge;

#define CMergeSetTupReady(merge, idx) \
  { \
	  (merge).ready[(idx)] = true; \
	  (merge).nfill++; \
  }

/* Tuple slot is filled, but ready bit is not set */
#define CMergeTupPending(merge, idx) \
   ((!(merge).ready[(idx)]) && ((merge).htup[(idx)] != NULL))

#define CMergeSetTup(merge, idx, tup, seq) \
  { \
	  (merge).htup[(idx)] = (tup); \
      if ((merge).snum[(idx)] == seq) \
      { \
		  elog(DEBUG1, "Got seqnum twice --- sleeping"); \
		  sleep(1000); \
	  } \
	  (merge).snum[(idx)] = (seq); \
  }

#define CMergeDelTup(merge, idx) \
  { \
      (merge).htup[(idx)]  = NULL; \
      (merge).ready[(idx)] = false; \
      (merge).nfill--; \
  }

#define CMergeTupReady(merge, idx) \
  ((merge).ready[(idx)])

typedef struct SnapshotCB
{
	bool initp;
	/* Snapshot of state -- wether primary is alive or dead */
	int  *alive_p;
	int  *alive_s;
	int  *got_syncs;
	bool partner_fail;
	int  partner_m;
} SnapshotCB;

/* Control block used to maintain information about which producers have failed
 * and recovered in order to determine when to start synchronize. */
typedef struct SyncCB
{
	int  *sync_recv_p;
	int  *sync_recv_s;
} SyncCB;

#define ResetSync(synccb, part) \
{ \
	(synccb).sync_recv_p[part] = false; \
	(synccb).sync_recv_s[part] = false; \
}

/* Information used to keep track of acks received for pauses */
typedef struct PauseCB
{
	bool *ack_recv;
} PauseCB;

/*
 * These are modes of Flux connections denoted in FluxConnInfo_FT.error
 */

typedef enum
{
	CONN_DISCONN = (-2),
	CONN_NORMAL  = 0,
	CONN_RECOVER = 102,
	CONN_SEND_MARKER,
	CONN_FLUSHBUF,        
	CONN_SEND_DATA,
	CONN_ACK_DATA,
	CONN_PAUSE,
	CONN_SEND_ACK_DATA,  /* Want to send and ack */
	CONN_RECV_DATA,
	CONN_ACK_CHECK_DATA, /* Want to ack and check incoming */
	CONN_UNKNOWN,        /* Don't know the status */
	CONN_DO_NOTHING      /* Don't do anything */
} ConnStatus;

#endif

/*-------------------------------------------------------------------------
 *
 * streamheapam.h
 *	  TelegraphCQ streamheap access method definitions.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 *-------------------------------------------------------------------------
 */
#ifndef STREAMHEAPAM_H
#define STREAMHEAPAM_H

#include "access/htup.h"
#include "access/relscan.h"
#include "access/sdir.h"
#include "access/tupmacs.h"
#include "access/xlogutils.h"
#include "storage/block.h"
#include "storage/lmgr.h"
#include "utils/rel.h"
#include "utils/tqual.h"

/* ----------------
 *		streamfastgetattr
 *
 *		Fetch a user attribute's value as a Datum (might be either a
 *		value, or a pointer into the data area of the tuple).
 *
 *		This must not be used when a system attribute might be requested.
 *		Furthermore, the passed attnum MUST be valid.  Use heap_getattr()
 *		instead, if in doubt.
 *
 *		This gets called many times, so we macro the cacheable and NULL
 *		lookups, and call nocachegetattr() for the rest.
 * ----------------
 */

extern Datum nocachegetattr(HeapTuple tup, int attnum,
			   TupleDesc att, bool *isnull);

#if !defined(DISABLE_COMPLEX_MACRO)

#define streamfastgetattr(tup, attnum, tupleDesc, isnull)					\
(																	\
	AssertMacro((attnum) > 0),										\
	((isnull) ? (*(isnull) = false) : (dummyret)NULL),				\
	HeapTupleNoNulls(tup) ?											\
	(																\
		(tupleDesc)->attrs[(attnum)-1]->attcacheoff >= 0 ?			\
		(															\
			fetchatt((tupleDesc)->attrs[(attnum)-1],				\
				(char *) (tup)->t_data + (tup)->t_data->t_hoff +	\
					(tupleDesc)->attrs[(attnum)-1]->attcacheoff)	\
		)															\
		:															\
			nocachegetattr((tup), (attnum), (tupleDesc), (isnull))	\
	)																\
	:																\
	(																\
		att_isnull((attnum)-1, (tup)->t_data->t_bits) ?				\
		(															\
			((isnull) ? (*(isnull) = true) : (dummyret)NULL),		\
			(Datum)NULL												\
		)															\
		:															\
		(															\
			nocachegetattr((tup), (attnum), (tupleDesc), (isnull))	\
		)															\
	)																\
)

#else							/* defined(DISABLE_COMPLEX_MACRO) */

extern Datum streamfastgetattr(HeapTuple tup, int attnum, TupleDesc tupleDesc,
				  bool *isnull);
#endif   /* defined(DISABLE_COMPLEX_MACRO) */



/* ----------------
 *		function prototypes for heap access method
 *
 * heap_create, heap_create_with_catalog, and heap_drop_with_catalog
 * are declared in catalog/heap.h
 * ----------------
 */


extern HeapScanDesc streamheap_beginscan(Relation relation,
					 Snapshot snapshot, int nkeys, ScanKey key);
extern void streamheap_rescan(HeapScanDesc scan, ScanKey key);
extern void streamheap_endscan(HeapScanDesc scan);
extern HeapTuple streamheap_getnext(HeapScanDesc scandesc, ScanDirection direction);
extern Oid	streamheap_insert(Relation relation, HeapTuple tup, CommandId cid);
extern void streamheap_markpos(HeapScanDesc scan);

extern void
			streamheap_restrpos(HeapScanDesc scan);
extern void streamheap_redo(XLogRecPtr lsn, XLogRecord *rptr);
extern void streamheap_undo(XLogRecPtr lsn, XLogRecord *rptr);
extern void streamheap_desc(char *buf, uint8 xl_info, char *rec);


extern void streaminitscan(HeapScanDesc scan, ScanKey key);

void streamheapgettup(Relation relation,
				 int dir,
				 HeapTuple tuple,
				 Buffer *buffer,
				 Snapshot snapshot,
				 int nkeys,
				 ScanKey key,
		                 HeapScanID heapScanID);

#endif   /* STREAMHEAPAM_H */

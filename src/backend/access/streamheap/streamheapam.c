/*-------------------------------------------------------------------------
 *
 * streamheapam.c
 *	  stream heap access method code
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Id: streamheapam.c,v 1.15 2004/10/04 22:48:48 phred Exp $
 *
 * INTERFACE ROUTINES
 *		streamheap_beginscan	- begin relation scan
 *		streamheap_rescan		- restart a relation scan
 *		streamheap_endscan	- end relation scan
 *		streamheap_getnext	- retrieve next tuple in scan
 *		streamheap_insert		- insert tuple into a relation
 *		streamheap_markpos	- mark scan position
 *		streamheap_restrpos - restore position to marked location
 *
 * NOTES
 *	  This file contains the streamheap_ routines which implement
 *	  the TelegraphcQ streamheap access method used for all the TelegraphCQ
 *	  streams.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/streamheapam.h"
#include "access/shio.h"
#include "access/tuptoaster.h"
#include "access/valid.h"
#include "access/xlogutils.h"
#include "catalog/catalog.h"
#include "miscadmin.h"
#include "utils/inval.h"
#include "utils/relcache.h"
#include "pgstat.h"

#include "storage/buf_internals.h"
#include "storage/bufmgr.h"
#include "storage/spin.h"

/* ----------------------------------------------------------------
 *						 streamheap support routines
 * ----------------------------------------------------------------
 */

/* declarations for functions that work with progress arrays */
HeapProgressArray findOrCreateProgressArray(Relation relation);

#define ScansUnarchivedStreamSpill(scan) \
( \
    (scan)->rs_rd->rd_rel->relkind == RELKIND_UNARCHIVEDSTREAM_SPILL \
)

/* ----------------
 *		streaminitscan - scan code common to heap_beginscan and heap_rescan
 * ----------------
 */

void
streaminitscan(HeapScanDesc scan, ScanKey key)
{
	scan->rs_ctup.t_datamcxt = NULL;
	scan->rs_ctup.t_data = NULL;
	scan->rs_cbuf = InvalidBuffer;

	/* we don't have a marked position... */
	ItemPointerSetInvalid(&(scan->rs_mctid));

	/*
	 * copy the scan key, if appropriate
	 */
	if (key != NULL)
		memmove(scan->rs_key, key, scan->rs_nkeys * sizeof(ScanKeyData));

	/*
	 * add self to the scan progress array for its relation; initialize
	 * one if one doesn't yet exist.  Only for unarchived streams.
	 */
	if (ScansUnarchivedStreamSpill(scan))
	{
	  MemoryContext old;
	  HeapProgressArray progressArray;

	  old = MemoryContextSwitchTo(TopSharedMemoryContext);

	  SpinLockAcquire(heapScanProgressTableSpinLock);

	  progressArray = findOrCreateProgressArray(scan->rs_rd);
	  
	  /* find an unused spot in the progressArray and use it as the id */
	  ProgressArrayFindUnusedID(progressArray, scan->heapScanID);

	  if (scan->heapScanID == -1)
	  {
	    elog(ERROR, "no more room in scanProgress array for another scan module for relid %d",
		 scan->rs_rd->rd_id);
	  }
	  else
	  {
	    /* TODO: initialize scanProgress[i] to appropriate value */
	    ProgressArraySetValue(progressArray, scan->heapScanID, -2);
	  }

	  SpinLockRelease(heapScanProgressTableSpinLock);
	  MemoryContextSwitchTo(old);
	}
}

/* ----------------
 *		streamheapgettup - fetch next heap tuple
 *
 *		routine used by streamheap_getnext() which does most of the
 *		real work in scanning tuples.
 *
 *		The passed-in *buffer must be either InvalidBuffer or the pinned
 *		current page of the scan.  If we have to move to another page,
 *		we will unpin this buffer (if valid).  On return, *buffer is either
 *		InvalidBuffer or the ID of a pinned buffer.
 * ----------------
 */
void
streamheapgettup(Relation relation,
				 int dir,
				 HeapTuple tuple,
				 Buffer *buffer,
				 Snapshot snapshot,
				 int nkeys,
				 ScanKey key,
		                 HeapScanID heapScanID)
{
	ItemId		lpp;
	Page		dp;
	BlockNumber page;
	BlockNumber pages;
	int			lines;
	OffsetNumber lineoff;
	int			linesleft;
	ItemPointer tid;

	/* we do not support reverse direction scans */

	/*
	 * We now do - as an initialization condition. if (dir < 0)
	 * elog(ERROR, "streamheapgettup: no reverse scans allowed");
	 */

	tid = (tuple->t_data == NULL) ? (ItemPointer) NULL : &(tuple->t_self);

	/*
	 * debugging stuff
	 *
	 * check validity of arguments, here and for other functions too Note: no
	 * locking manipulations needed--this is a local function
	 */
#ifdef	HEAPDEBUGALL
	if (ItemPointerIsValid(tid))
	{
		elog(DEBUG, "streamheapgettup(%s, tid=0x%x[%d,%d], dir=%d, ...)",
			 RelationGetRelationName(relation), tid, tid->ip_blkid,
			 tid->ip_posid, dir);
	}
	else
	{
		elog(DEBUG, "streamheapgettup(%s, tid=0x%x, dir=%d, ...)",
			 RelationGetRelationName(relation), tid, dir);
	}
	elog(DEBUG, "streamheapgettup(..., b=0x%x, nkeys=%d, key=0x%x", buffer, nkeys, key);

	elog(DEBUG, "streamheapgettup: relation(%c)=`%s', %p",
		 relation->rd_rel->relkind, RelationGetRelationName(relation),
		 snapshot);
#endif   /* !defined(HEAPDEBUGALL) */

	if (!ItemPointerIsValid(tid))
	{
		Assert(!PointerIsValid(tid));
		tid = NULL;
	}

	tuple->t_tableOid = relation->rd_id;

	/*
	 * return null immediately if relation is empty
	 */
	if ((pages = relation->rd_nblocks) == 0)
	{
		if (BufferIsValid(*buffer))
			ReleaseBuffer(*buffer);
		*buffer = InvalidBuffer;
		tuple->t_datamcxt = NULL;
		tuple->t_data = NULL;
		return;
	}

	/*
	 * calculate next starting lineoff, given scan direction
	 */
	if (!dir)
	{
		/*
		 * ``no movement'' scan direction: refetch same tuple
		 */
		if (tid == NULL)
		{
			if (BufferIsValid(*buffer))
				ReleaseBuffer(*buffer);
			*buffer = InvalidBuffer;
			tuple->t_datamcxt = NULL;
			tuple->t_data = NULL;
			return;
		}

		*buffer = ReleaseAndReadBuffer(*buffer,
					       relation,
					       ItemPointerGetBlockNumber(tid));
		if (!BufferIsValid(*buffer))
			elog(ERROR, "streamheapgettup: failed ReadBuffer");

		LockBuffer(*buffer, BUFFER_LOCK_SHARE);

		dp = (Page) BufferGetPage(*buffer);
		lineoff = ItemPointerGetOffsetNumber(tid);
		lpp = PageGetItemId(dp, lineoff);

		tuple->t_datamcxt = NULL;
		tuple->t_data = (HeapTupleHeader) PageGetItem((Page) dp, lpp);
		tuple->t_len = ItemIdGetLength(lpp);
		LockBuffer(*buffer, BUFFER_LOCK_UNLOCK);
		return;

	}
	else if (dir < 0)
	{
		/*
		 * reverse scan direction
		 */
		if (tid == NULL)
		{
			page = pages - 1;	/* final page */
		}
		else
		{
			page = ItemPointerGetBlockNumber(tid);		/* current page */
		}

		Assert(page < pages);

		*buffer = ReleaseAndReadBuffer(*buffer,
									   relation,
									   page);
		if (!BufferIsValid(*buffer))
			elog(ERROR, "streamheapgettup: failed ReadBuffer");

		LockBuffer(*buffer, BUFFER_LOCK_SHARE);

		dp = (Page) BufferGetPage(*buffer);
		lines = PageGetMaxOffsetNumber(dp);
		if (tid == NULL)
		{
			lineoff = lines;	/* final offnum */
		}
		else
		{
			lineoff =			/* previous offnum */
				OffsetNumberPrev(ItemPointerGetOffsetNumber(tid));
		}
		/* page and lineoff now reference the physically previous tid */

	}
	else
	{

		/*
		 * forward scan direction
		 */
		if (tid == NULL)
		{
			page = 0;			/* first page */
			lineoff = FirstOffsetNumber;		/* first offnum */
		}
		else
		{
			page = ItemPointerGetBlockNumber(tid);		/* current page */
			lineoff =			/* next offnum */
				OffsetNumberNext(ItemPointerGetOffsetNumber(tid));

		}

		Assert(page < pages);

		*buffer = ReleaseAndReadBuffer(*buffer,
									   relation,
									   page);
		if (!BufferIsValid(*buffer))
			elog(ERROR, "streamheapgettup: failed ReadBuffer");

		LockBuffer(*buffer, BUFFER_LOCK_SHARE);

		dp = (Page) BufferGetPage(*buffer);
		lines = PageGetMaxOffsetNumber(dp);
		/* page and lineoff now reference the physically next tid */

	}

	/* 'dir' is now non-zero */

	/*
	 * calculate line pointer and number of remaining items to check on
	 * this page.
	 */
	lpp = PageGetItemId(dp, lineoff);
	if (dir < 0)
		linesleft = lineoff - 1;
	else
		linesleft = lines - lineoff;

	/*
	 * advance the scan until we find a qualifying tuple or run out of
	 * stuff to scan
	 */
	
	for (;;)
	{
		while (linesleft >= 0)
		{
			if (ItemIdIsUsed(lpp))
			{
				bool		valid;

				tuple->t_datamcxt = NULL;
				tuple->t_data = (HeapTupleHeader) PageGetItem((Page) dp, lpp);
				tuple->t_len = ItemIdGetLength(lpp);
				ItemPointerSet(&(tuple->t_self), page, lineoff);

				/*
				 * if current tuple qualifies, return it.
				 */
				HeapTupleSatisfies(tuple, relation, *buffer, (PageHeader) dp,
								   snapshot, nkeys, key, valid);
				if (valid)
				{
					LockBuffer(*buffer, BUFFER_LOCK_UNLOCK);
					return;
				}
			}

			/*
			 * otherwise move to the next item on the page
			 */
			--linesleft;
			if (dir < 0)
			{
				--lpp;			/* move back in this page's ItemId array */
				--lineoff;
			}
			else
			{
				++lpp;			/* move forward in this page's ItemId
								 * array */
				++lineoff;
			}
		}

		/*
		 * if we get here, it means we've exhausted the items on this page
		 * and it's time to move to the next.
		 */
		LockBuffer(*buffer, BUFFER_LOCK_UNLOCK);

		/*
		 * return NULL if we've exhausted all the pages
		 */
		if ((dir < 0) ? (page == 0) : (page + 1 >= pages))
		{
			if (BufferIsValid(*buffer))
				ReleaseBuffer(*buffer);

			*buffer = InvalidBuffer;
			tuple->t_datamcxt = NULL;
			tuple->t_data = NULL;

			return;
		}

		page = (dir < 0) ? (page - 1) : (page + 1);

		Assert(page < pages);

		*buffer = 
		  (relation->rd_rel->relkind == RELKIND_UNARCHIVEDSTREAM_SPILL) ?
		  ReleaseReadAndUndirtyBuffer(*buffer, relation, page, heapScanID) :
		  ReleaseAndReadBuffer(*buffer, relation, page);

		if (!BufferIsValid(*buffer))
			elog(ERROR, "streamheapgettup: failed ReadBuffer");

		LockBuffer(*buffer, BUFFER_LOCK_SHARE);
		dp = (Page) BufferGetPage(*buffer);
		lines = PageGetMaxOffsetNumber((Page) dp);
		linesleft = lines - 1;
		if (dir < 0)
		{
			lineoff = lines;
			lpp = PageGetItemId(dp, lines);
		}
		else
		{
			lineoff = FirstOffsetNumber;
			lpp = PageGetItemId(dp, FirstOffsetNumber);
		}
	}
}


#if defined(DISABLE_COMPLEX_MACRO)
/*
 * This is formatted so oddly so that the correspondence to the macro
 * definition in access/heapam.h is maintained.
 */
Datum
streamfastgetattr(HeapTuple tup, int attnum, TupleDesc tupleDesc,
				  bool *isnull)
{
	return (
			(attnum) > 0 ?
			(
			 ((isnull) ? (*(isnull) = false) : (dummyret) NULL),
			 HeapTupleNoNulls(tup) ?
			 (
			  (tupleDesc)->attrs[(attnum) - 1]->attcacheoff >= 0 ?
			  (
			   fetchatt((tupleDesc)->attrs[(attnum) - 1],
						(char *) (tup)->t_data + (tup)->t_data->t_hoff +
						(tupleDesc)->attrs[(attnum) - 1]->attcacheoff)
			   )
			  :
			  nocachegetattr((tup), (attnum), (tupleDesc), (isnull))
			  )
			 :
			 (
			  att_isnull((attnum) - 1, (tup)->t_data->t_bits) ?
			  (
			   ((isnull) ? (*(isnull) = true) : (dummyret) NULL),
			   (Datum) NULL
			   )
			  :
			  (
			   nocachegetattr((tup), (attnum), (tupleDesc), (isnull))
			   )
			  )
			 )
			:
			(
			 (Datum) NULL
			 )
		);
}
#endif   /* defined(DISABLE_COMPLEX_MACRO) */



/* ----------------------------------------------------------------
 *					 heap access method interface
 * ----------------------------------------------------------------
 */





/* ----------------
 *		streamheap_beginscan	- begin relation scan
 * ----------------
 */
HeapScanDesc
streamheap_beginscan(Relation relation, Snapshot snapshot,
					 int nkeys, ScanKey key)
{
	HeapScanDesc scan;

	/*
	 * sanity checks
	 */
	if (!RelationIsValid(relation))
		elog(ERROR, "heap_beginscan: !RelationIsValid(relation)");

	/*
	 * increment relation ref count while scanning relation
	 *
	 * This is just to make really sure the relcache entry won't go away
	 * while the scan has a pointer to it.	Caller should be holding the
	 * rel open anyway, so this is redundant in all normal scenarios...
	 */
	RelationIncrementReferenceCount(relation);

	/* XXX someday assert SelfTimeQual if relkind == RELKIND_UNCATALOGED */
	if (relation->rd_rel->relkind == RELKIND_UNCATALOGED)
		snapshot = SnapshotSelf;

	/*
	 * allocate and initialize scan descriptor
	 */
	scan = (HeapScanDesc) palloc(sizeof(HeapScanDescData));

	scan->rs_rd = relation;

	/* Allow the backend processes to see all tuples */
	scan->rs_snapshot = SnapshotAny;
	scan->rs_nkeys = (short) nkeys;

	pgstat_initstats(&scan->rs_pgstat_info, relation);

	/*
	 * we do this here instead of in streaminitscan() because heap_rescan
	 * also calls streaminitscan() and we don't want to allocate memory
	 * again
	 */
	if (nkeys)
		scan->rs_key = (ScanKey) palloc(sizeof(ScanKeyData) * nkeys);
	else
		scan->rs_key = NULL;

	streaminitscan(scan, key);

	return scan;
}

/* ----------------
 *		streamheap_rescan		- restart a stream scan
 * ----------------
 */
void
streamheap_rescan(HeapScanDesc scan, ScanKey key)
{
	/*
	 * unpin scan buffers
	 */
	if (BufferIsValid(scan->rs_cbuf))
		ReleaseBuffer(scan->rs_cbuf);

	/*
	 * reinitialize scan descriptor
	 */
	streaminitscan(scan, key);

	pgstat_reset_heap_scan(&scan->rs_pgstat_info);
}

/* ----------------
 *		streamheap_endscan	- end stream scan
 *
 *		See how to integrate with index scans.
 *		Check handling if reldesc caching.
 * ----------------
 */
void
streamheap_endscan(HeapScanDesc scan)
{

	/* Note: no locking manipulations needed */

	/*
	 * unpin scan buffers
	 */
	if (BufferIsValid(scan->rs_cbuf))
		ReleaseBuffer(scan->rs_cbuf);

	/*
	 * decrement relation reference count and free scan descriptor storage
	 */
	RelationDecrementReferenceCount(scan->rs_rd);

	if (scan->rs_key)
		pfree(scan->rs_key);


	if (ScansUnarchivedStreamSpill(scan))
	{
	  MemoryContext old;
	  HeapProgressLookupEnt* p_ent;
	  BlockNumber curBlock = 0, minBlock = 0, minBlock2 = 0;
	  int numOfMin, numOfMin2;

	  /* 
	   * mark the spot in progress array as unused
	   */

	  old = MemoryContextSwitchTo(TopSharedMemoryContext);
	  SpinLockAcquire(heapScanProgressTableSpinLock);

	  p_ent = ProgressArrayLookup(scan->rs_rd);

	  Assert(p_ent);

	  /* get the current and the min block numbers */
	  curBlock = ProgressArrayGetValue(p_ent->progress, scan->heapScanID);
	  MinOfProgressArray(p_ent->progress, minBlock, numOfMin);

	  /* mark the array cell unused */
	  ProgressArraySetValue(p_ent->progress, scan->heapScanID, -1);

	  /* if we're the single slowest scan module, what page is the 
	   * next-slowest scan module on?
	   */
	  if (curBlock == minBlock && numOfMin == 1)
	  {
	    MinOfProgressArray(p_ent->progress, minBlock2, numOfMin2);
	  }

	  /* if no one else is using the the relation, then delete the
	   * entry from the hash table and free the progress array
	   */
	  if (ProgressArrayUnused(p_ent->progress))
	  {
	    ProgressArrayDelete(scan->rs_rd);
	    pfree(p_ent->progress);
	  }

	  SpinLockRelease(heapScanProgressTableSpinLock);
	  MemoryContextSwitchTo(old);

	  /* If we're the single slowest scan module, we need to undirty every
	   * page from curBlock to minBlock2.
	   
	   * We don't need to hold the SpinLock while we
	   * do this because the second-slowest scan module is now the slowest
	   * scan module, and will take care of undirtying the pages from its
	   * current page and onward.
	   */

	  if (curBlock == minBlock && numOfMin == 1 && curBlock != minBlock2)
	  {
	    /* if there are no more scan modules, then only dirty curBlock 
	     * TODO: We should actually undirty ALL blocks, but that's kind of
	     * pointless, since new data will be coming in anyway
	     */
	    if (minBlock2 == -1)
	    {
	      minBlock2 = curBlock+1;
	    }

	    UndirtyBuffers(scan->rs_rd->rd_node, curBlock, minBlock2);
	  }	    
	}

	pfree(scan);
}




/* ----------------
 *		streamheap_getnext	- retrieve next tuple in scan
 *
 *		Fix to work with index relations.
 *		We don't return the buffer anymore, but you can get it from the
 *		returned HeapTuple.
 * ----------------
 */

#ifdef HEAPDEBUGALL
#define HEAPDEBUG_1 \
elog(DEBUG, "heap_getnext([%s,nkeys=%d],backw=%d) called", \
	 RelationGetRelationName(scan->rs_rd), scan->rs_nkeys, backw)

#define HEAPDEBUG_2 \
	 elog(DEBUG, "heap_getnext returning EOS")

#define HEAPDEBUG_3 \
	 elog(DEBUG, "heap_getnext returning tuple");
#else
#define HEAPDEBUG_1
#define HEAPDEBUG_2
#define HEAPDEBUG_3
#endif   /* !defined(HEAPDEBUGALL) */


HeapTuple
streamheap_getnext(HeapScanDesc scan, ScanDirection direction)
{

	/*
	 * increment access statistics
	 */
	IncrHeapAccessStat(local_getnext);
	IncrHeapAccessStat(global_getnext);

	/* Note: no locking manipulations needed */

	/*
	 * argument checks
	 */
	if (scan == NULL)
		elog(ERROR, "heap_getnext: NULL relscan");

	HEAPDEBUG_1;				/* heap_getnext( info ) */

	/* prevent the wraparound of the scan */
	if (HeapTupleIsValid(scan->rs_ctup.t_data))
	{
	  /* get the starting blockNumber */
	  if (ScansUnarchivedStreamSpill(scan) &&
	      !ItemPointerIsValid(&scan->rs_mctid))
	  {
	    /* t_data is valid but rs_mctid is not, so this is right after reading
	     * the first tuple.  Set the block number in progress array to be
	     * the block number of t_data.
	     */

	    MemoryContext old = MemoryContextSwitchTo(TopSharedMemoryContext);
	    HeapProgressLookupEnt* p_ent;

	    SpinLockAcquire(heapScanProgressTableSpinLock);

	    p_ent = ProgressArrayLookup(scan->rs_rd);

	    Assert(p_ent);

	    /* set the current blockNumber to be the blockNumber that
	     * scan->rs_ctup belongs to
	     */
	    ProgressArraySetValue(p_ent->progress, scan->heapScanID,
				  ItemPointerGetBlockNumber(&scan->rs_ctup.t_self));

	    SpinLockRelease(heapScanProgressTableSpinLock);
	    MemoryContextSwitchTo(old);
	  }

	  scan->rs_mctid = scan->rs_ctup.t_self;
	}
	else if (ItemPointerIsValid(&scan->rs_mctid))
	{
		scan->rs_ctup.t_self = scan->rs_mctid;
		scan->rs_ctup.t_datamcxt = NULL;
		scan->rs_ctup.t_data = (HeapTupleHeader) 0x1;	/* for heapgettup */
	}

	/* Since we are no longer transactional, we need to */
	/* recompute the number of blocks every time. */
	scan->rs_rd->rd_nblocks = RelationGetNumberOfBlocks(scan->rs_rd);

	/* if (BufferIsValid(scan->rs_cbuf)) */
	/* ReleaseBuffer(scan->rs_cbuf); */
	/* scan->rs_cbuf = InvalidBuffer; */

	streamheapgettup(scan->rs_rd,
					 (int) direction,
					 &(scan->rs_ctup),
					 &(scan->rs_cbuf),
					 scan->rs_snapshot,
					 scan->rs_nkeys,
					 scan->rs_key,
			                 scan->heapScanID);

	if (scan->rs_ctup.t_data == NULL && !BufferIsValid(scan->rs_cbuf))
	{
		HEAPDEBUG_2;			/* heap_getnext returning EOS */
		/* elog(DEBUG, "RETURNING NULL\n"); */
		return NULL;
	}

	pgstat_count_heap_scan(&scan->rs_pgstat_info);

	/*
	 * if we get here it means we have a new current scan tuple, so point
	 * to the proper return buffer and return the tuple.
	 */

	HEAPDEBUG_3;				/* heap_getnext returning tuple */

	if (scan->rs_ctup.t_data != NULL)
		pgstat_count_heap_getnext(&scan->rs_pgstat_info);

	return ((scan->rs_ctup.t_data == NULL) ? NULL : &(scan->rs_ctup));
}

/* ----------------
 *		streamheap_insert		- insert tuple into a streamheap
 *
 * The new tuple is stamped with current transaction ID and the specified
 * command ID.
 *
 *		The thing to note here is that we have to ensure
 *		that the insert is append-only (do not use free
 *		space in earlier blocks
 * ----------------
 */
Oid
streamheap_insert(Relation relation, HeapTuple tup, CommandId cid)
{
	Buffer		buffer;

	if (relation->rd_rel->relhasoids)
	{
#ifdef NOT_USED
		/* this is redundant with an Assert in HeapTupleSetOid */
		Assert(tup->t_data->t_infomask & HEAP_HASOID);
#endif

		/*
		 * If the object id of this tuple has already been assigned, trust
		 * the caller.	There are a couple of ways this can happen.  At
		 * initial db creation, the backend program sets oids for tuples.
		 * When we define an index, we set the oid.  Finally, in the
		 * future, we may allow users to set their own object ids in order
		 * to support a persistent object store (objects need to contain
		 * pointers to one another).
		 */

	}
	else
	{
		/* check there is not space for an OID */
		Assert(!(tup->t_data->t_infomask & HEAP_HASOID));
	}

	tup->t_data->t_infomask &= ~(HEAP_XACT_MASK);
	tup->t_data->t_infomask |= HEAP_XMAX_INVALID;
	HeapTupleHeaderSetXmin(tup->t_data, GetCurrentTransactionId());
	HeapTupleHeaderSetCmin(tup->t_data, cid);
	tup->t_tableOid = relation->rd_id;

#ifdef TUPLE_TOASTER_ACTIVE

	/*
	 * If the new tuple is too big for storage or contains already toasted
	 * attributes from some other relation, invoke the toaster.
	 */
	if (HeapTupleHasExtended(tup) ||
		(MAXALIGN(tup->t_len) > TOAST_TUPLE_THRESHOLD))
		heap_tuple_toast_attrs(relation, tup, NULL);
#endif

	/* Find buffer to insert this tuple into */

	/*
	 * buffer = StreamGetBufferForTuple(relation, tup->t_len,
	 * InvalidBuffer);
	 */
	buffer = StreamGetBufferForTuple(relation, tup->t_len);

	/* NO ELOG(ERROR) from here till changes are logged */
	START_CRIT_SECTION();

	StreamPutHeapTuple(relation, buffer, tup);

	pgstat_count_heap_insert(&relation->pgstat_info);

	/* XLOG stuff */
	if ((!relation->rd_istemp) && 0)
	{
		xl_heap_insert xlrec;
		xl_heap_header xlhdr;
		XLogRecPtr	recptr;
		XLogRecData rdata[3];
		Page		page = BufferGetPage(buffer);
		uint8		info = XLOG_HEAP_INSERT;

		xlrec.target.node = relation->rd_node;
		xlrec.target.tid = tup->t_self;
		rdata[0].buffer = InvalidBuffer;
		rdata[0].data = (char *) &xlrec;
		rdata[0].len = SizeOfHeapInsert;
		rdata[0].next = &(rdata[1]);

		xlhdr.t_natts = tup->t_data->t_natts;
		xlhdr.t_infomask = tup->t_data->t_infomask;
		xlhdr.t_hoff = tup->t_data->t_hoff;

		/*
		 * note we mark rdata[1] as belonging to buffer; if XLogInsert
		 * decides to write the whole page to the xlog, we don't need to
		 * store xl_heap_header in the xlog.
		 */
		rdata[1].buffer = buffer;
		rdata[1].data = (char *) &xlhdr;
		rdata[1].len = SizeOfHeapHeader;
		rdata[1].next = &(rdata[2]);

		rdata[2].buffer = buffer;
		/* PG73FORMAT: write bitmap [+ padding] [+ oid] + data */
		rdata[2].data = (char *) tup->t_data + offsetof(HeapTupleHeaderData, t_bits);
		rdata[2].len = tup->t_len - offsetof(HeapTupleHeaderData, t_bits);
		rdata[2].next = NULL;

		/*
		 * If this is the single and first tuple on page, we can reinit
		 * the page instead of restoring the whole thing.  Set flag, and
		 * hide buffer references from XLogInsert.
		 */
		if (ItemPointerGetOffsetNumber(&(tup->t_self)) == FirstOffsetNumber &&
			PageGetMaxOffsetNumber(page) == FirstOffsetNumber)
		{
			info |= XLOG_HEAP_INIT_PAGE;
			rdata[1].buffer = rdata[2].buffer = InvalidBuffer;
		}

		recptr = XLogInsert(RM_HEAP_ID, info, rdata);

		PageSetLSN(page, recptr);
		PageSetSUI(page, ThisStartUpID);
	}
	else
	{
		/* No XLOG record, but still need to flag that XID exists on disk */
		MyXactMadeTempRelUpdate = true;
	}
	END_CRIT_SECTION();

	LockBuffer(buffer, BUFFER_LOCK_UNLOCK);
	WriteBuffer(buffer);

	/*
	 * If tuple is cachable, mark it for rollback from the caches in case
	 * we abort.  Note it is OK to do this after WriteBuffer releases the
	 * buffer, because the "tup" data structure is all in local memory,
	 * not in the shared buffer.
	 */
	CacheInvalidateHeapTuple(relation, tup);

	return HeapTupleGetOid(tup);
}

/* ----------------
 *		streamheap_markpos	- mark scan position
 *
 *		Note:
 *				Should only one mark be maintained per scan at one time.
 *		Check if this can be done generally--say calls to get the
 *		next/previous tuple and NEVER pass struct scandesc to the
 *		user AM's.  Now, the mark is sent to the executor for safekeeping.
 *		Probably can store this info into a GENERAL scan structure.
 *
 *		May be best to change this call to store the marked position
 *		(up to 2?) in the scan structure itself.
 *		Fix to use the proper caching structure.
 * ----------------
 */
void
streamheap_markpos(HeapScanDesc scan)
{
	/* Note: no locking manipulations needed */

	if (scan->rs_ctup.t_data != NULL)
		scan->rs_mctid = scan->rs_ctup.t_self;
	else
		ItemPointerSetInvalid(&scan->rs_mctid);
}

/* ----------------
 *		streamheap_restrpos - restore position to marked location
 *
 *		Note:  there are bad side effects here.  If we were past the end
 *		of a relation when heapmarkpos is called, then if the relation is
 *		extended via insert, then the next call to heaprestrpos will set
 *		cause the added tuples to be visible when the scan continues.
 *		Problems also arise if the TID's are rearranged!!!
 *
 * XXX	might be better to do direct access instead of
 *		using the generality of streamheapgettup().
 *
 * XXX It is very possible that when a scan is restored, that a tuple
 * XXX which previously qualified may fail for time range purposes, unless
 * XXX some form of locking exists (ie., portals currently can act funny.
 * ----------------
 */
void
streamheap_restrpos(HeapScanDesc scan)
{
	/* XXX no amrestrpos checking that ammarkpos called */

	/* Note: no locking manipulations needed */

	/*
	 * unpin scan buffers
	 */
	if (BufferIsValid(scan->rs_cbuf))
		ReleaseBuffer(scan->rs_cbuf);
	scan->rs_cbuf = InvalidBuffer;

	if (!ItemPointerIsValid(&scan->rs_mctid))
	{
		scan->rs_ctup.t_datamcxt = NULL;
		scan->rs_ctup.t_data = NULL;
	}
	else
	{
		scan->rs_ctup.t_self = scan->rs_mctid;
		scan->rs_ctup.t_datamcxt = NULL;
		scan->rs_ctup.t_data = (HeapTupleHeader) 0x1;	/* for heapgettup */
		streamheapgettup(scan->rs_rd,
						 0,
						 &(scan->rs_ctup),
						 &(scan->rs_cbuf),
						 scan->rs_snapshot,
						 0,
						 (ScanKey) NULL,
				                 scan->heapScanID);
	}
}

void
streamheap_redo(XLogRecPtr lsn, XLogRecord *record)
{
	elog(PANIC, "gist_redo: unimplemented");
}

void
streamheap_undo(XLogRecPtr lsn, XLogRecord *record)
{
	elog(PANIC, "gist_undo: unimplemented");
}

void
streamheap_desc(char *buf, uint8 xl_info, char *rec)
{
}

HeapProgressArray findOrCreateProgressArray(Relation relation)
{

  HeapProgressLookupEnt *p_ent;

  bool found;
  int i;

  p_ent = ProgressArrayLookupOrCreate(relation, found);

  if(!p_ent)
  {    
    elog(ERROR, "heapScanProgressTable ran out of memory!");
  }

  if(!found)
  {
    /* This is the first scan module to use that relation, so we need
     * to allocate the progress array for it
     */

    p_ent->progress = CreateNewProgressArray();

    /* TODO: find a better way to initialize */
    for(i=0; i<PROGRESS_ARRAY_SIZE; i++)
    {
      (p_ent->progress)[i] = -1;
    }

  }

  return p_ent->progress;
}

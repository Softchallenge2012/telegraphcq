/*-------------------------------------------------------------------------
 *
 * shio.c
 *	 TelegraphCQ streamheap access method input/output code.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Id: shio.c,v 1.7 2003/07/17 05:18:44 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/streamheapam.h"
#include "access/heapam.h"
#include "access/shio.h"
#include "storage/freespace.h"


/*
 * StreamPutHeapTuple - place tuple at specified page
 *
 * !!! ELOG(ERROR) IS DISALLOWED HERE !!!
 *
 * Note - caller must hold BUFFER_LOCK_EXCLUSIVE on the buffer.
 */
void
StreamPutHeapTuple(Relation relation,
				   Buffer buffer,
				   HeapTuple tuple)
{
	Page		pageHeader;
	OffsetNumber offnum;
	ItemId		itemId;
	Item		item;

	/* Add the tuple to the page */
	pageHeader = BufferGetPage(buffer);

	offnum = PageAddItem(pageHeader, (Item) tuple->t_data,
						 tuple->t_len, InvalidOffsetNumber, LP_USED);

	if (offnum == InvalidOffsetNumber)
		elog(PANIC, "RelationPutHeapTuple: failed to add tuple");

	/* Update tuple->t_self to the actual position where it was stored */
	ItemPointerSet(&(tuple->t_self), BufferGetBlockNumber(buffer), offnum);

	/* Insert the correct position into CTID of the stored tuple, too */
	itemId = PageGetItemId(pageHeader, offnum);
	item = PageGetItem(pageHeader, itemId);
	((HeapTupleHeader) item)->t_ctid = tuple->t_self;
}

/*
 * StreamGetBufferForTuple
 *
 *	Returns pinned and exclusive-locked buffer of a page in given relation
 *	with free space >= given len.
 *
 *	NOTE: otherbuffer is relic from heapam, not used here.
 *
 *	Note that we use LockPage(rel, 0) to lock relation for extension.
 *	We can do this as long as in all other places we use page-level locking
 *	for indices only. Alternatively, we could define pseudo-table as
 *	we do for transactions with XactLockTable.
 *
 *	ELOG(ERROR) is allowed here, so this routine *must* be called
 *	before any (unlogged) changes are made in buffer pool.
 */
Buffer
StreamGetBufferForTuple(Relation relation, Size len)
/*StreamGetBufferForTuple(Relation relation, Size len
						Buffer otherBuffer)*/
{
	Buffer		buffer = InvalidBuffer;
	Page		pageHeader;
	Size		pageFreeSpace;
	BlockNumber targetBlock,
				otherBlock;
	bool		needLock;

	len = MAXALIGN(len);		/* be conservative */

	/*
	 * If we're gonna fail for oversize tuple, do it right away
	 */
	if (len > MaxTupleSize)
		elog(ERROR, "Tuple is too big: size %lu, max size %ld",
			 (unsigned long) len, MaxTupleSize);

	/*
	 * if (otherBuffer != InvalidBuffer) elog(ERROR,
	 * "StreamGetBufferForTuple: Other Buffer %d not InvalidBuffer",
	 * otherBuffer);
	 */

	otherBlock = InvalidBlockNumber;

	/*
	 * We first try to put the tuple on the same page we last inserted a
	 * tuple on, as cached in the relcache entry.
	 */
	targetBlock = relation->rd_targblock;

	/*
	 * That failed...try the last page before we give up and extend. This
	 * avids one-tuple-per-page syndrome during bootstrapping or in a
	 * recentl-started ststem.
	 */
	if (targetBlock == InvalidBlockNumber)
	{
		BlockNumber nblocks = RelationGetNumberOfBlocks(relation);

		if (nblocks > 0)
			targetBlock = nblocks - 1;
	}


	if (targetBlock != InvalidBlockNumber)
	{
		/*
		 * We must be on the last page in the relation now
		 */

		/* Read and exclusive-lock the target block  */
		buffer = ReadBuffer(relation, targetBlock);
		LockBuffer(buffer, BUFFER_LOCK_EXCLUSIVE);

		/*
		 * Now we can check to see if there's enough free space here. If
		 * so, we're done.
		 */
		pageHeader = (Page) BufferGetPage(buffer);
		pageFreeSpace = PageGetFreeSpace(pageHeader);
		if (len <= pageFreeSpace)
		{
			/* use this page as future insert target, too */
			relation->rd_targblock = targetBlock;
			return buffer;
		}

		/*
		 * Not enough space, so we must give up our page locks and pin (if
		 * any) and prepare to look elsewhere.	We don't care which order
		 * we unlock the two buffers in, so this can be slightly simpler
		 * than the code above.
		 */
		LockBuffer(buffer, BUFFER_LOCK_UNLOCK);
		/*
		 * tcqoc: temporary fix to ensure access method does not take up
		 * the entire buffer pool with pinned buffers
		 */
		/* if (otherBuffer == InvalidBuffer) */
		ReleaseBuffer(buffer);
	}

	/*
	 * Have to extend the relation.
	 *
	 * We have to use a lock to ensure no one else is extending the rel at
	 * the same time, else we will both try to initialize the same new
	 * page.
	 */
	needLock = !(relation->rd_isnew || relation->rd_istemp);

	if (needLock)
		LockPage(relation, 0, ExclusiveLock);

	/*
	 * XXX This does an lseek - rather expensive - but at the moment it is
	 * the only way to accurately determine how many blocks are in a
	 * relation.  Is it worth keeping an accurate file length in shared
	 * memory someplace, rather than relying on the kernel to do it for
	 * us?
	 */
	buffer = ReadBuffer(relation, P_NEW);

	/*
	 * Release the file-extension lock; it's now OK for someone else to
	 * extend the relation some more.
	 */
	if (needLock)
		UnlockPage(relation, 0, ExclusiveLock);

	/*
	 * We can be certain that locking the otherBuffer first is OK, since
	 * it must have a lower page number.
	 */

	/*
	 * if (otherBuffer != InvalidBuffer) LockBuffer(otherBuffer,
	 * BUFFER_LOCK_EXCLUSIVE);
	 */

	/*
	 * We need to initialize the empty new page.
	 */
	LockBuffer(buffer, BUFFER_LOCK_EXCLUSIVE);
	pageHeader = (Page) BufferGetPage(buffer);
	Assert(PageIsNew((PageHeader) pageHeader));
	PageInit(pageHeader, BufferGetPageSize(buffer), 0);

	if (len > PageGetFreeSpace(pageHeader))
	{
		/* We should not get here given the test at the top */
		elog(PANIC, "Tuple is too big: size %lu", (unsigned long) len);
	}

	/*
	 * Remember the new page as our target for future insertions.
	 *
	 * XXX should we enter the new page into the free space map immediately,
	 * or just keep it for this backend's exclusive use in the short run
	 * (until VACUUM sees it)?	Seems to depend on whether you expect the
	 * current backend to make more insertions or not, which is probably a
	 * good bet most of the time.  So for now, don't add it to FSM yet.
	 */
	relation->rd_targblock = BufferGetBlockNumber(buffer);

	return buffer;
}

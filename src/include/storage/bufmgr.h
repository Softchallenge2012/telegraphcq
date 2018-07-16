/*-------------------------------------------------------------------------
 *
 * bufmgr.h
 *	  POSTGRES buffer manager definitions.
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: bufmgr.h,v 1.3 2004/03/24 08:11:29 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef BUFMGR_H
#define BUFMGR_H

#include "access/xlogdefs.h"
#include "storage/buf.h"
#include "storage/lock.h"
#include "storage/relfilenode.h"
#include "utils/rel.h"

typedef void *Block;

/* in globals.c ... this duplicates miscadmin.h */
extern DLLIMPORT int NBuffers;

/* in buf_init.c */
extern DLLIMPORT Block *BufferBlockPointers;
extern long *PrivateRefCount;

/* in localbuf.c */
extern DLLIMPORT int NLocBuffer;
extern DLLIMPORT Block *LocalBufferBlockPointers;
extern long *LocalRefCount;

/* special pageno for bget */
#define P_NEW	InvalidBlockNumber		/* grow the file to get a new page */

/*
 * Buffer context lock modes
 */
#define BUFFER_LOCK_UNLOCK		0
#define BUFFER_LOCK_SHARE		1
#define BUFFER_LOCK_EXCLUSIVE	2

/*
 * These routines are beaten on quite heavily, hence the macroization.
 */

#define BAD_BUFFER_ID(bid) ((bid) < 1 || (bid) > NBuffers)
#define INVALID_DESCRIPTOR (-3)

#define UnlockAndReleaseBuffer(buffer)	\
( \
	LockBuffer(buffer, BUFFER_LOCK_UNLOCK), \
	ReleaseBuffer(buffer) \
)

#define UnlockAndWriteBuffer(buffer)	\
( \
	LockBuffer(buffer, BUFFER_LOCK_UNLOCK), \
	WriteBuffer(buffer) \
)

/*
 * BufferIsValid
 *		True iff the given buffer number is valid (either as a shared
 *		or local buffer).
 *
 * Note: For a long time this was defined the same as BufferIsPinned,
 * that is it would say False if you didn't hold a pin on the buffer.
 * I believe this was bogus and served only to mask logic errors.
 * Code should always know whether it has a buffer reference,
 * independently of the pin state.
 */
#define BufferIsValid(bufnum) \
( \
	BufferIsLocal(bufnum) ? \
		((bufnum) >= -NLocBuffer) \
	: \
		(! BAD_BUFFER_ID(bufnum)) \
)

/*
 * BufferIsPinned
 *		True iff the buffer is pinned (also checks for valid buffer number).
 *
 *		NOTE: what we check here is that *this* backend holds a pin on
 *		the buffer.  We do not care whether some other backend does.
 */
#define BufferIsPinned(bufnum) \
( \
	BufferIsLocal(bufnum) ? \
		((bufnum) >= -NLocBuffer && LocalRefCount[-(bufnum) - 1] > 0) \
	: \
	( \
		BAD_BUFFER_ID(bufnum) ? \
			false \
		: \
			(PrivateRefCount[(bufnum) - 1] > 0) \
	) \
)

/*
 * IncrBufferRefCount
 *		Increment the pin count on a buffer that we have *already* pinned
 *		at least once.
 *
 *		This macro cannot be used on a buffer we do not have pinned,
 *		because it doesn't change the shared buffer state.  Therefore the
 *		Assert checks are for refcount > 0.  Someone got this wrong once...
 */
#define IncrBufferRefCount(buffer) \
( \
	BufferIsLocal(buffer) ? \
	( \
		(void) AssertMacro((buffer) >= -NLocBuffer), \
		(void) AssertMacro(LocalRefCount[-(buffer) - 1] > 0), \
		(void) LocalRefCount[-(buffer) - 1]++ \
	) \
	: \
	( \
		(void) AssertMacro(!BAD_BUFFER_ID(buffer)), \
		(void) AssertMacro(PrivateRefCount[(buffer) - 1] > 0), \
		(void) PrivateRefCount[(buffer) - 1]++ \
	) \
)

/*
 * BufferGetBlock
 *		Returns a reference to a disk page image associated with a buffer.
 *
 * Note:
 *		Assumes buffer is valid.
 */
#define BufferGetBlock(buffer) \
( \
	AssertMacro(BufferIsValid(buffer)), \
	BufferIsLocal(buffer) ? \
		LocalBufferBlockPointers[-(buffer) - 1] \
	: \
		BufferBlockPointers[(buffer) - 1] \
)

/*
 * prototypes for functions in bufmgr.c
 */
extern Buffer ReadBuffer(Relation reln, BlockNumber blockNum);
extern int	ReleaseBuffer(Buffer buffer);
extern void WriteBuffer(Buffer buffer);
extern void WriteNoReleaseBuffer(Buffer buffer);
extern Buffer release_and_read_buffer(Buffer buffer, Relation relation,
				    BlockNumber blockNum, bool attemptUndirty,
				      int id);
extern void undirty_buffers(RelFileNode rnode, BlockNumber start, 
			    BlockNumber end, bool hasLock);
extern void InitBufferPool(void);
extern void InitBufferPoolAccess(void);
extern char *ShowBufferUsage(void);
extern void ResetBufferUsage(void);
extern void AtEOXact_Buffers(bool isCommit);
extern void FlushBufferPool(void);
extern BlockNumber BufferGetBlockNumber(Buffer buffer);
extern BlockNumber RelationGetNumberOfBlocks(Relation relation);
extern void RelationUpdateNumberOfBlocks(Relation relation);
extern int	FlushRelationBuffers(Relation rel, BlockNumber firstDelBlock);
extern void DropRelationBuffers(Relation rel);
extern void DropRelFileNodeBuffers(RelFileNode rnode, bool istemp);
extern void DropBuffers(Oid dbid);

#ifdef NOT_USED
extern void PrintPinnedBufs(void);
#endif
extern int	BufferShmemSize(void);
extern RelFileNode BufferGetFileNode(Buffer buffer);

extern void SetBufferCommitInfoNeedsSave(Buffer buffer);

extern void UnlockBuffers(void);
extern void LockBuffer(Buffer buffer, int mode);
extern void LockBufferForCleanup(Buffer buffer);

extern void AbortBufferIO(void);

extern void BufmgrCommit(void);
extern void BufferSync(void);

extern void InitLocalBuffer(void);

/* Progress Array Stuff */
extern bool ProgressArrayUnused(int* array);
extern void UndirtyBuffers(RelFileNode rnode, BlockNumber start, BlockNumber end);

#define CreateNewProgressArray() \
( \
    (HeapProgressArray)palloc(sizeof(int)*PROGRESS_ARRAY_SIZE) \
)

#define ProgressArrayLookupOrCreate(relation,found) \
( \
    (HeapProgressLookupEnt*)hash_search(*heapScanProgressTable, \
                                    (void*) (&(RelationGetRelid((relation)))), \
                                    HASH_ENTER,\
                                    &(found)) \
)

#define ProgressArrayLookup(relation) \
( \
    (HeapProgressLookupEnt*)hash_search(*heapScanProgressTable, \
                                    (void*) (&(RelationGetRelid((relation)))), \
                                    HASH_FIND,\
                                    NULL) \
)

#define ProgressArrayDelete(relation) \
( \
    (HeapProgressLookupEnt*)hash_search(*heapScanProgressTable, \
                                    (void*) (&(RelationGetRelid((relation)))), \
                                    HASH_REMOVE,\
                                    NULL) \
)

#define ProgressArrayGetValue(array,id) \
( \
    (array)[(id)] \
)

#define ProgressArraySetValue(array,id,value) \
( \
    (array)[(id)] = (value) \
)


#undef ReleaseAndReadBuffer
/* 
 * ReleaseAndReadBuffer -- combine ReleaseBuffer() and ReadBuffer()
 *		to save a lock release/acquire.
 *
 * Also, if the passed buffer is valid and already contains the desired block
 * number, we simply return it without ever acquiring the lock at all.
 * Since the passed buffer must be pinned, it's OK to examine its block
 * number without getting the lock first.
 *
 * Note: it is OK to pass buffer = InvalidBuffer, indicating that no old
 * buffer actually needs to be released.  This case is the same as ReadBuffer,
 * but can save some tests in the caller.
 *
 * Also note: while it will work to call this routine with blockNum == P_NEW,
 * it's best to avoid doing so, since that would result in calling
 * smgrnblocks() while holding the bufmgr lock, hence some loss of
 * concurrency.
 */

#define ReleaseAndReadBuffer(buffer, relation, blockNum) \
( \
  release_and_read_buffer((buffer), (relation), (blockNum), false, -1) \
)


/* 
 * ReleaseReadAndUndirtyBuffer -- does ReleaseAndReadBuffer, and also undirties
 * the given buffer if the given blockNum causes us to return a buffer different
 * from the given buffer.  Used when the backend is reading tuples for an
 * unarchived spill stream to save unnecessary I/Os.
 */
#define ReleaseReadAndUndirtyBuffer(buffer, relation, blockNum, id) \
( \
  release_and_read_buffer((buffer), (relation), (blockNum), true, (id)) \
)

/* finds the min value of progress array */
#define MinOfProgressArray(array, min, numOfMin) \
do {\
   int i=0; \
   numOfMin = 0; \
   min = INT_MAX; \
   for(i=0; i<PROGRESS_ARRAY_SIZE; i++) \
   { \
      if ((array)[i] >= 0) \
      { \
        if((array)[i] == (min)) \
        { \
          (numOfMin) += 1; \
        } \
        else if ((array)[i] < (min)) \
        { \
          (min) = (array)[i]; \
          (numOfMin) = 1; \
        } \
      } \
   } \
   if ((min) == INT_MAX) (min) = -1; \
} while (0)

/*
 * UndirtyBuffers
 * undirties buffers for relation rnode, for blocks from start(inclusive) to 
 * end(exclusive)
 */
#define UndirtyBuffers(rnode, start, end) \
( \
  undirty_buffers(rnode, start, end, false) \
)


#define ProgressArrayFindUnusedID(array,id) \
do { \
 int i = 0; \
 (id) = -1;\
 for (i=0; i<PROGRESS_ARRAY_SIZE; i++) \
  { \
    if ((array)[i] == -1) \
    { \
      (id) = i;\
      break; \
    }\
  }\
} while(0)

#endif

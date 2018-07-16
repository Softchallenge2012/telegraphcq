/*-------------------------------------------------------------------------
 *
 * stem.h
 *	  internal structures for SteMs
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: stem.h,v 1.8 2003/07/16 09:42:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef STEM_H
#define STEM_H

#include "executor/intermediateTuple.h"

/* ----------------------------------------------------------------
 *
 * Much of the following description for hash tables should still apply
 * for SteMHashTables. The biggest, and probably the only, difference between
 * these two is that SteMHashTables will store IntermediateHeapTuples instead
 * of HeapTuples.
 *
 *
 *				hash-join hash table structures
 *
 * Each active hashjoin has a HashJoinTable control block which is
 * palloc'd in the executor's per-query context.  All other storage needed
 * for the hashjoin is kept in private memory contexts, two for each hashjoin.
 * This makes it easy and fast to release the storage when we don't need it
 * anymore.
 *
 * The hashtable contexts are made children of the per-query context, ensuring
 * that they will be discarded at end of statement even if the join is
 * aborted early by an error.  (Likewise, any temporary files we make will
 * be cleaned up by the virtual file manager in event of an error.)
 *
 * Storage that should live through the entire join is allocated from the
 * "hashCxt", while storage that is only wanted for the current batch is
 * allocated in the "batchCxt".  By resetting the batchCxt at the end of
 * each batch, we free all the per-batch storage reliably and without tedium.
 * ----------------------------------------------------------------
 */

typedef struct SteMHashJoinTupleData
{
	struct SteMHashJoinTupleData *next; /* link to next tuple in same
										 * bucket */

	int			bucket_no;		/** Useful for moving tuples in and out of memory */

	IntermediateHeapTupleData htup;
}	SteMHashJoinTupleData;

typedef SteMHashJoinTupleData *SteMHashJoinTuple;

typedef struct SteMHashTableData
{
	int			nbuckets;		/* buckets in use during this batch */
	int			totalbuckets;	/* total number of (virtual) buckets */
	SteMHashJoinTuple *buckets; /* buckets[i] is head of list of tuples */
	/* buckets array is per-batch storage, as are all the tuples */

	int			nbatch;			/* number of batches; 0 means 1-pass join */
	int			curbatch;		/* current batch #, or 0 during 1st pass */

	/*
	 * all these arrays are allocated for the life of the hash join, but
	 * only if nbatch > 0:
	 */
	BufFile   **innerBatchFile; /* buffered virtual temp file per batch */
	BufFile   **outerBatchFile; /* buffered virtual temp file per batch */
	long	   *outerBatchSize; /* count of tuples in each outer batch
								 * file */
	long	   *innerBatchSize; /* count of tuples in each inner batch
								 * file */

	/*
	 * Info about the datatype being hashed.  We assume that the inner and
	 * outer sides of the hash are the same type, or at least
	 * binary-compatible types.
	 */
	int16		typLen;
	bool		typByVal;

	/*
	 * We will use three different contexts for state, interemediate
	 * tuples and base table tuples. Both of these live through the
	 * existence of the SteM.
	 */
	MemoryContext stemContext;	/* context for stuff like hashtable */
	MemoryContext itupleContext;	/* context for intermediate tuples */
	MemoryContext btupleContext;	/* context for base table tuples */
}	SteMHashTableData;

typedef SteMHashTableData *SteMHashJoinTable;

#endif   /* STEM_H */

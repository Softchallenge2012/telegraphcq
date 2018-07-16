/*-------------------------------------------------------------------------
 *
 * executor.h
 *	  support for the POSTGRES executor module
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: aggHash.h,v 1.3 2003/07/16 09:42:50 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef AGGHASH_H
#define AGGHASH_H

typedef struct hash_cols
{

	int			numCols;
	AttrNumber *keyColIdx;		/* Indexes into the target list */

}	hash_cols;

/* ----------------------------------------------------------------
 *								 Tuple Hash Tables
 *
 * All-in-memory tuple hash tables are used for a number of purposes.
 * ----------------------------------------------------------------
 */
typedef struct TupleHashEntryData *TupleHashEntry;
typedef struct TupleHashTableData *TupleHashTable;

typedef struct TupleHashEntryData
{
	TupleHashEntry next;		/* next entry in same hash bucket */
	uint32		hashkey;		/* exact hash key of this entry */
	HeapTuple	firstTuple;		/* copy of first tuple in this group */
	/* there may be additional data beyond the end of this struct */
}	TupleHashEntryData; /* VARIABLE LENGTH STRUCT */

typedef struct TupleHashTableData
{
	int			numCols;		/* number of columns in lookup key */
	AttrNumber *keyColIdx;		/* attr numbers of key columns */
	FmgrInfo   *eqfunctions;	/* lookup data for comparison functions */
	MemoryContext tablecxt;		/* memory context containing table */
	MemoryContext tempcxt;		/* context for function evaluations */
	Size		entrysize;		/* actual size to make each hash entry */
	int			nbuckets;		/* number of buckets in hash table */
	int			nentries;		/* number of entries in the hashtable */
	TupleHashEntry buckets[1];	/* VARIABLE LENGTH ARRAY */
}	TupleHashTableData; /* VARIABLE LENGTH STRUCT */

typedef struct
{
	TupleHashEntry next_entry;	/* next entry in current chain */
	int			next_bucket;	/* next chain */
}	TupleHashIterator;

#define ResetTupleHashIterator(iter) \
		((iter)->next_entry = NULL, \
		 (iter)->next_bucket = 0)


extern uint32 ComputeHashFunc(Datum key, int typLen, bool byVal);

extern int GetTupleHashKey(TupleHashTable hashtable,
				TupleTableSlot *slot);

extern int GetTupleHashKey_orig(hash_cols * hcols,
					 TupleTableSlot *slot);

extern TupleHashTable BuildTupleHashTable(int numCols, AttrNumber *keyColIdx,
					FmgrInfo *eqfunctions,
					int nbuckets, Size entrysize,
					MemoryContext tablecxt, MemoryContext tempcxt);
extern TupleHashEntry LookupTupleHashEntry(TupleHashTable hashtable,
					 TupleTableSlot *slot,
					 bool *isnew);
extern TupleHashEntry ScanTupleHashTable(TupleHashTable hashtable,
				   TupleHashIterator * state);

extern TupleHashEntry InsertTupleHashEntry(TupleHashTable hashtable,
					 TupleHashEntry nentry);

#define TupleHashSize(hashtable) \
(sizeof(TupleHashTableData) + \
 ((hashtable)->nbuckets - 1) * sizeof(TupleHashEntry) + \
 (hashtable)->nentries * (hashtable)->entrysize)


#endif   /* AGGHASH_H */

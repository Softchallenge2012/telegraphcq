/*-------------------------------------------------------------------------
 *
 * aggHash.c
 *	  executor utility routines for grouping, hashing, and aggregation
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/executor/aggHash.c,v 1.4 2004/03/05 21:10:15 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/hash.h"
#include "access/heapam.h"
#include "executor/executor.h"
#include "executor/nodeGroup.h" /* for tuplesMatch */
#include "executor/aggHash.h"
#include "parser/parse_oper.h"
#include "utils/memutils.h"


/*****************************************************************************
 *		Utility routines for hashing
 *****************************************************************************/

/*
 * ComputeHashFunc
 *
 *		the hash function for hash joins (also used for hash aggregation)
 *
 *		XXX this probably ought to be replaced with datatype-specific
 *		hash functions, such as those already implemented for hash indexes.
 */
uint32
ComputeHashFunc(Datum key, int typLen, bool byVal)
{
	unsigned char *k;

#ifdef HASH_DEBUG_MSGS
	elog(DEBUG, "ComputeHashFunc: key=%p, typLen=%d, byVal=%d", key, typLen, (int) byVal);
#endif

	if (byVal)
	{
		/*
		 * If it's a by-value data type, just hash the whole Datum value.
		 * This assumes that datatypes narrower than Datum are
		 * consistently padded (either zero-extended or sign-extended, but
		 * not random bits) to fill Datum; see the XXXGetDatum macros in
		 * postgres.h. NOTE: it would not work to do hash_any(&key, len)
		 * since this would get the wrong bytes on a big-endian machine.
		 */
		k = (unsigned char *) &key;
		typLen = sizeof(Datum);
	}
	else
	{
		if (typLen > 0)
		{
			/* fixed-width pass-by-reference type */
			k = (unsigned char *) DatumGetPointer(key);
		}
		else if (typLen == -1)
		{
			/*
			 * It's a varlena type, so 'key' points to a "struct varlena".
			 * NOTE: VARSIZE returns the "real" data length plus the
			 * sizeof the "vl_len" attribute of varlena (the length
			 * information). 'key' points to the beginning of the varlena
			 * struct, so we have to use "VARDATA" to find the beginning
			 * of the "real" data.	Also, we have to be careful to detoast
			 * the datum if it's toasted.  (We don't worry about freeing
			 * the detoasted copy; that happens for free when the
			 * per-tuple memory context is reset in ExecHashGetBucket.)
			 */
			struct varlena *vkey = PG_DETOAST_DATUM(key);

			typLen = VARSIZE(vkey) - VARHDRSZ;
			k = (unsigned char *) VARDATA(vkey);
		}
		else if (typLen == -2)
		{
			/* It's a null-terminated C string */
			typLen = strlen(DatumGetCString(key)) + 1;
			k = (unsigned char *) DatumGetPointer(key);
		}
		else
		{
			elog(ERROR, "ComputeHashFunc: Invalid typLen %d", typLen);
			k = NULL;			/* keep compiler quiet */
		}
	}

#ifdef HASH_DEBUG_MSGS
	elog(DEBUG, "calling hash_any");
#endif
	return DatumGetUInt32(hash_any(k, typLen));
}


/*
 * GetTupleHashBucket
 *
 * Must be called in the hashtable's tempcxt memory context.
 */
int
GetTupleHashKey(TupleHashTable hashtable, TupleTableSlot *slot)
{
	hash_cols	hcols;

	hcols.numCols = hashtable->numCols;
	hcols.keyColIdx = hashtable->keyColIdx;

	/*
	 * Determines which hash function to use. The original also handles
	 * variable length fields. The new one can be made to, but currently
	 * does not. The new can generates different hash functions from some
	 * family with different seed values.
	 */

#ifdef USE_NEW_KEY_HASH
	return calc_composite_key_hash(&hcols, slot, 179);
#else
	return GetTupleHashKey_orig(&hcols, slot);
#endif

}

/*
 * This is the original function which computed the hash of
 * a compound key, i.e. grouping column.
 */
int
GetTupleHashKey_orig(hash_cols * hcols, TupleTableSlot *slot)
{
	HeapTuple	tuple = slot->val;
	TupleDesc	tupdesc = slot->ttc_tupleDescriptor;
	int			numCols = hcols->numCols;
	AttrNumber *keyColIdx = hcols->keyColIdx;
	uint32		hashkey = 0;
	int			i;

#ifdef HASH_DEBUG_MSGS
	elog(DEBUG, "GetTupleHashKey: numCols=%d", numCols);
#endif

	for (i = 0; i < numCols; i++)
	{
		AttrNumber	att = keyColIdx[i];
		Datum		attr;
		bool		isNull;

		/* rotate hashkey left 1 bit at each step */
		hashkey = (hashkey << 1) | ((hashkey & 0x80000000) ? 1 : 0);

#ifdef HASH_DEBUG_MSGS
		elog(DEBUG, "heap_getattr: tuple=%p, att=%d", tuple, (int) att);
/*		elog(DEBUG, "tempcxt = %p", hashtable->tempcxt); */
		elog(DEBUG, "tuple->t_datamcxt = %p", tuple->t_datamcxt);
		elog(DEBUG, "tupdesc->natts = %d", (int) tupdesc->natts);
		elog(DEBUG, "attname = %s", (int) tupdesc->attrs[att - 1]->attname.data);
		elog(DEBUG, "attlen = %d", (int) tupdesc->attrs[att - 1]->attlen);
		elog(DEBUG, "attbyval = %d", (int) tupdesc->attrs[att - 1]->attbyval);
		/* attr = (Datum)10; */
#endif   /* HASH_DEBUG_MSGS */

		attr = heap_getattr(tuple, att, tupdesc, &isNull);
#ifdef HASH_DEBUG_MSGS
		elog(DEBUG, "got attr datum, attr=%p", attr);
#endif
		if (isNull)
			continue;			/* treat nulls as having hash key 0 */
		hashkey ^= ComputeHashFunc(attr,
								   (int) tupdesc->attrs[att - 1]->attlen,
								   tupdesc->attrs[att - 1]->attbyval);
	}
#ifdef HASH_DEBUG_MSGS
	elog(DEBUG, "Returning hashkey %d", (int) hashkey);
#endif
	return hashkey;
}


/*****************************************************************************
 *		Utility routines for all-in-memory hash tables
 *
 * These routines build hash tables for grouping tuples together (eg, for
 * hash aggregation).  There is one entry for each not-distinct set of tuples
 * presented.
 *****************************************************************************/

/*
 * Construct an empty TupleHashTable
 *
 *	numCols, keyColIdx: identify the tuple fields to use as lookup key
 *	eqfunctions: equality comparison functions to use
 *	nbuckets: number of buckets to make
 *	entrysize: size of each entry (at least sizeof(TupleHashEntryData))
 *	tablecxt: memory context in which to store table and table entries
 *	tempcxt: short-lived context for evaluation hash and comparison functions
 *
 * The eqfunctions array may be made with execTuplesMatchPrepare().
 *
 * Note that keyColIdx and eqfunctions must be allocated in storage that
 * will live as long as the hashtable does.
 */
TupleHashTable
BuildTupleHashTable(int numCols, AttrNumber *keyColIdx,
					FmgrInfo *eqfunctions,
					int nbuckets, Size entrysize,
					MemoryContext tablecxt, MemoryContext tempcxt)
{
	TupleHashTable hashtable;
	Size		tabsize;

	Assert(nbuckets > 0);
	Assert(entrysize >= sizeof(TupleHashEntryData));

	tabsize = sizeof(TupleHashTableData) +
		(nbuckets - 1) * sizeof(TupleHashEntry);
	hashtable = (TupleHashTable) MemoryContextAlloc(tablecxt, tabsize);
	MemSet(hashtable, 0, tabsize);

	hashtable->numCols = numCols;
	hashtable->keyColIdx = keyColIdx;
	hashtable->eqfunctions = eqfunctions;
	hashtable->tablecxt = tablecxt;
	hashtable->tempcxt = tempcxt;
	hashtable->entrysize = entrysize;
	hashtable->nbuckets = nbuckets;
	hashtable->nentries = 0;

	return hashtable;
}

/*
 * Find or create a hashtable entry for the tuple group containing the
 * given tuple.
 *
 * If isnew is NULL, we do not create new entries; we return NULL if no
 * match is found.
 *
 * If isnew isn't NULL, then a new entry is created if no existing entry
 * matches.  On return, *isnew is true if the entry is newly created,
 * false if it existed already.  Any extra space in a new entry has been
 * zeroed.
 */
TupleHashEntry
LookupTupleHashEntry(TupleHashTable hashtable, TupleTableSlot *slot,
					 bool *isnew)
{
	int			numCols = hashtable->numCols;
	AttrNumber *keyColIdx = hashtable->keyColIdx;
	HeapTuple	tuple = slot->val;
	TupleDesc	tupdesc = slot->ttc_tupleDescriptor;
	uint32		hashkey;
	int			bucketno;
	TupleHashEntry entry;
	MemoryContext oldContext;

	/* Need to run the hash function in short-lived context */
	oldContext = MemoryContextSwitchTo(hashtable->tempcxt);

	hashkey = GetTupleHashKey(hashtable, slot);
	bucketno = hashkey % (uint32) hashtable->nbuckets;
#ifdef HASH_DEBUG_MSGS
	elog(DEBUG, "hashkey = %d, bucketno = %d", (int) hashkey, bucketno);
#endif

	for (entry = hashtable->buckets[bucketno];
		 entry != NULL;
		 entry = entry->next)
	{
		/* Quick check using hashkey */
		if (entry->hashkey != hashkey)
			continue;
		if (execTuplesMatch(entry->firstTuple,
							tuple,
							tupdesc,
							numCols, keyColIdx,
							hashtable->eqfunctions,
							hashtable->tempcxt))
		{
			if (isnew)
				*isnew = false;
			MemoryContextSwitchTo(oldContext);
			return entry;
		}
	}

	/* Not there, so build a new one if requested */
	if (isnew)
	{
		MemoryContextSwitchTo(hashtable->tablecxt);

		entry = (TupleHashEntry) palloc(hashtable->entrysize);
		MemSet(entry, 0, hashtable->entrysize);

		entry->hashkey = hashkey;
		entry->firstTuple = heap_copytuple(tuple);

		entry->next = hashtable->buckets[bucketno];
		hashtable->buckets[bucketno] = entry;

		*isnew = true;
		++hashtable->nentries;
	}

	MemoryContextSwitchTo(oldContext);

	return entry;
}

/*
 * Walk through all the entries of a hash table, in no special order.
 * Returns NULL when no more entries remain.
 *
 * Iterator state must be initialized with ResetTupleHashIterator() macro.
 */
TupleHashEntry
ScanTupleHashTable(TupleHashTable hashtable, TupleHashIterator * state)
{
	TupleHashEntry entry;

	entry = state->next_entry;
	while (entry == NULL)
	{
		if (state->next_bucket >= hashtable->nbuckets)
		{
			/* No more entries in hashtable, so done */
			return NULL;
		}
		entry = hashtable->buckets[state->next_bucket++];
	}
	state->next_entry = entry->next;

	return entry;
}

/* Inserts the given entry into the table. Return NULL if the entry is
   inserted, otherwise returns the entry that exists. Used for
   un-checkpointing. */

TupleHashEntry
InsertTupleHashEntry(TupleHashTable hashtable, TupleHashEntry nentry)
{

	int			bucketno;
	uint32		hashkey;
	
#ifdef NOT_USED
	TupleHashEntry entry;
#endif

	hashkey = nentry->hashkey;
	bucketno = hashkey % (uint32) hashtable->nbuckets;

#ifdef NOT_USED
	/* Why do this check? */
	for (entry = hashtable->buckets[bucketno];
		 entry != NULL;
		 entry = entry->next)
	{

		if (entry->hashkey == hashkey)
			return entry;

	}
#endif

	/* We have a new entry, add it to the top of the hashtable. */
	nentry->next = hashtable->buckets[bucketno];
	hashtable->buckets[bucketno] = nentry;

	hashtable->nentries++;
		
	return NULL;
}

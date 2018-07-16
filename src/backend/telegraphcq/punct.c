
/*-------------------------------------------------------------------------
 *
 * punct.c
 *
 * Functions dealing with punctuation tuples.  
 *
 * Portions Copyright (c) 2005, Regents of the University of California
 *
 * IDENTIFICATION
 *           $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/punct.c,v 1.5 2005/07/27 20:04:12 phred Exp $
 *
 *-------------------------------------------------------------------------
 */


#include "postgres.h"

#include "telegraphcq/punct.h"

/*******************************************************************************
 * PROTOTYPES OF INTERNAL FUNCTIONS
 ******************************************************************************/
void checkAndFreePunctIHT(IntermediateHeapTuple iht, int sourceid); 


/* FUNCTION form_punct_tup
 * ARGUMENTS: <tupleDescriptor> is the descriptor for the stream and <ts> is 
 *      the timestamp to the attach to the descriptor.
 * POSTCONDITIONS: Creates a new punctuation tuple for the indicated stream.
 */
HeapTuple form_punct_tup(TupleDesc tupleDescriptor, Timestamp ts)
{
    return form_punct_tup_no_leaks(tupleDescriptor, ts, CurrentMemoryContext);
}

/* form_punct_tup() leaks memory unless you're clever about memory contexts.
 * This version takes a temporary memory context as an argument and allocates
 * temporary stuff into there.  tmpcxt should not be reset while this function
 * is executing. */
HeapTuple form_punct_tup_no_leaks(TupleDesc tupleDescriptor, Timestamp ts,
        MemoryContext tmpcxt)
{
    static Datum vals[PUNCT_INIT_SZ];
    static char nulls[PUNCT_INIT_SZ];
    int			numberOfAttributes = tupleDescriptor->natts;

    int         tsattr = tupleDescriptor->constr->ts_attrnum;
        /* The location of the timestamp field. 0-based. */
    int         i = -1;
    HeapTuple   ret = NULL;
    MemoryContext origcxt = NULL;

    /* Sanity checks. */
    Assert(numberOfAttributes < PUNCT_INIT_SZ);
    Assert(tsattr >= 0 && tsattr < numberOfAttributes);

    /* Initialize our arrays. */
    for (i = 0; i < numberOfAttributes; i++) {
        vals[i] = (Datum)(0UL);
        nulls[i] = 'n';
    }
    nulls[numberOfAttributes] = '\0';

    /* Set up the timestamp parts. */
    origcxt = MemoryContextSwitchTo(tmpcxt);
    { /* Begin temp context */
        vals[tsattr] = TimestampGetDatum(ts);
    } /* End temp context */
    MemoryContextSwitchTo(origcxt);
    nulls[tsattr] = ' ';

    /*
    elog(LOG, "form_punct_tup(): Setting field %d of %d to '%s'.",
            tsattr + 1, numberOfAttributes,
            DatumGetCString(DirectFunctionCall1(timestamp_out,
                    vals[tsattr]))
    );
    elog(LOG, "form_punct_tup(): Nulls string is '%s'", nulls);
    */

    ret = heap_formtuple(tupleDescriptor, vals, nulls);

    /* Set the "is punctuation" flag in the tuple header. */
    ret->t_data->t_infomask |= HEAP_ISPUNCT;

    return ret;
}


/* FUNCTION form_punct_tup_no_ts
 * ARGUMENTS: <tupleDescriptor> is the descriptor for the stream.
 * POSTCONDITIONS: Creates a non-timestamped punctuation tuple for the 
 *      indicated stream.
 */
HeapTuple form_punct_tup_no_ts(TupleDesc tupleDescriptor)
{
    static Datum vals[PUNCT_INIT_SZ];
    static char nulls[PUNCT_INIT_SZ];
    int			numberOfAttributes = tupleDescriptor->natts;

    int         i = -1;
    HeapTuple   ret = NULL;

    /* Sanity checks. */
    Assert(numberOfAttributes < PUNCT_INIT_SZ);

    /* Initialize our arrays. */
    for (i = 0; i < numberOfAttributes; i++) {
        vals[i] = (Datum)(0UL);
        nulls[i] = 'n';
    }
    nulls[numberOfAttributes] = '\0';

    ret = heap_formtuple(tupleDescriptor, vals, nulls);

    /* Set the "is punctuation" flag in the tuple header. */
    ret->t_data->t_infomask |= HEAP_ISPUNCT;

    return ret;
}


/* FUNCTION ihtIsPunct
 * POSTCONDITIONS: Returns TRUE if <iht> contains a punctuation tuple.
 */
bool ihtIsPunct(IntermediateHeapTuple iht)
{
    int i;
    Assert(iht != NULL);

    /* Check all the base tuples in the IHT */
    for (i = 0; i < __MAX_RELATIONS__; i++) {
        HeapTuple ht = iht->t_base_tuples[i];
        if (NULL != ht && HeapTupleIsPunct(ht)) {
            return true;
        }
    }

    return false;
}

void checkAndFreePunctIHT(IntermediateHeapTuple iht, int sourceid) 
{
    HeapTuple ht;
    
    Assert(iht != NULL);

    ht = iht->t_base_tuples[sourceid];
    Assert(NULL != ht);
    
    if (HeapTupleIsPunct(ht)) 
    {
	heap_freetuple(ht);
    }
}


bool slotContainsPunct(TupleTableSlot *slot_p) {
    Assert(slot_p != NULL);

    if ((slot_p->ttc_isIntermediateTuple 
                && ihtIsPunct((IntermediateHeapTuple)slot_p->val))
            ||
        (!slot_p->ttc_isIntermediateTuple && HeapTupleIsPunct(slot_p->val)))
    {
        return true;
    } else {
        return false;
    }

}


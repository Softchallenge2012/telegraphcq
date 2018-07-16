
/*
 * $RCSfile: punct.h,v $
 * $Revision: 1.4 $
 * $Date: 2005/06/13 02:59:29 $
 */

/*******************************************************************************
 * FILENAME: punct.h
 * AUTHOR(s): Fred Reiss <phred@cs.berkeley.edu>
 * DESCRIPTION: Function prototypes for punctuation tuples.
 *
 * Portions Copyright (c) 2005, Regents of the University of California
 ******************************************************************************/

#ifndef PUNCT_H
#define PUNCT_H

#include "postgres.h"
    /* Basic Postgres includes. */
    
#include "access/htup.h"
#include "access/heapam.h"
    /* Heap access method and heap tuples. */

#include "executor/intermediateTuple.h"
    /* IntermediateHeapTuple internal data structures */

#include "executor/tuptable.h"
    /* TupleTableSlot struct */

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/
#define PUNCT_INIT_SZ 128
    /* Size of the static initialization array in form_punct_tup */

/*******************************************************************************
 * PROTOTYPES OF EXTERNAL FUNCTIONS
 *
 * See src/backend/telegraphcq/punct.c for function comments.
 ******************************************************************************/

extern HeapTuple form_punct_tup(TupleDesc tupleDescriptor, Timestamp ts);
extern HeapTuple form_punct_tup_no_leaks(TupleDesc tupleDescriptor, 
        Timestamp ts, MemoryContext tmpcxt);
extern HeapTuple form_punct_tup_no_ts(TupleDesc tupleDescriptor);


extern bool ihtIsPunct(IntermediateHeapTuple iht);

/* Use the HeapTupleIsPunct() macro to check the punctuation status of a base
 * tuple. */

extern bool slotContainsPunct(TupleTableSlot *slot_p);

#endif /* PUNCT_H */


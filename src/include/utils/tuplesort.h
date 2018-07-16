/*-------------------------------------------------------------------------
 *
 * tuplesort.h
 *	  Generalized tuple sorting routines.
 *
 * This module handles sorting of heap tuples, index tuples, or single
 * Datums (and could easily support other kinds of sortable objects,
 * if necessary).  It works efficiently for both small and large amounts
 * of data.  Small amounts are sorted in-memory using qsort().	Large
 * amounts are sorted using temporary files and a standard external sort
 * algorithm.
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: tuplesort.h,v 1.2 2003/04/29 21:25:11 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef TUPLESORT_H
#define TUPLESORT_H

#include "access/htup.h"
#include "access/itup.h"
#include "fmgr.h"
#include "utils/logtape.h"

/*
 * This routine selects an appropriate sorting function to implement
 * a sort operator as efficiently as possible.
 */
typedef enum
{
	SORTFUNC_LT,				/* raw "<" operator */
	SORTFUNC_REVLT,				/* raw "<" operator, but reverse NULLs */
	SORTFUNC_CMP,				/* -1 / 0 / 1 three-way comparator */
	SORTFUNC_REVCMP				/* 1 / 0 / -1 (reversed) 3-way comparator */
} SortFunctionKind;

/* Move here from tuplesort.c */

#define MAXTAPES		7		/* Knuth's T */
#define TAPERANGE		(MAXTAPES-1)	/* Knuth's P */

typedef enum
{
	TSS_INITIAL,				/* Loading tuples; still within memory
								 * limit */
	TSS_BUILDRUNS,				/* Loading tuples; writing to tape */
	TSS_SORTEDINMEM,			/* Sort completed entirely in memory */
	TSS_SORTEDONTAPE,			/* Sort completed, final run is on tape */
	TSS_FINALMERGE				/* Performing final merge on-the-fly */
} TupSortStatus;

/* Tuplesortstate is an opaque type whose details are not known outside tuplesort.c. */

typedef struct Tuplesortstate Tuplesortstate;

struct Tuplesortstate
{
	TupSortStatus status;		/* enumerated value as shown above */
	bool		randomAccess;	/* did caller request random access? */
	long		availMem;		/* remaining memory available, in bytes */
	LogicalTapeSet *tapeset;	/* logtape.c object for tapes in a temp
								 * file */

	/*
	 * These function pointers decouple the routines that must know what
	 * kind of tuple we are sorting from the routines that don't need to
	 * know it. They are set up by the tuplesort_begin_xxx routines.
	 *
	 * Function to compare two tuples; result is per qsort() convention, ie:
	 *
	 * <0, 0, >0 according as a<b, a=b, a>b.
	 */
	int			(*comparetup) (Tuplesortstate *state, const void *a, const void *b);

	/*
	 * Function to copy a supplied input tuple into palloc'd space. (NB:
	 * we assume that a single pfree() is enough to release the tuple
	 * later, so the representation must be "flat" in one palloc chunk.)
	 * state->availMem must be decreased by the amount of space used.
	 */
	void	   *(*copytup) (Tuplesortstate *state, void *tup);

	/*
	 * Function to write a stored tuple onto tape.	The representation of
	 * the tuple on tape need not be the same as it is in memory;
	 * requirements on the tape representation are given below.  After
	 * writing the tuple, pfree() it, and increase state->availMem by the
	 * amount of memory space thereby released.
	 */
	void		(*writetup) (Tuplesortstate *state, int tapenum, void *tup);

	/*
	 * Function to read a stored tuple from tape back into memory. 'len'
	 * is the already-read length of the stored tuple.	Create and return
	 * a palloc'd copy, and decrease state->availMem by the amount of
	 * memory space consumed.
	 */
	void	   *(*readtup) (Tuplesortstate *state, int tapenum, unsigned int len);

	/*
	 * Obtain memory space occupied by a stored tuple.	(This routine is
	 * only needed in the FINALMERGE case, since copytup, writetup, and
	 * readtup are expected to adjust availMem appropriately.)
	 */
	unsigned int (*tuplesize) (Tuplesortstate *state, void *tup);

	/*
	 * This array holds pointers to tuples in sort memory.	If we are in
	 * state INITIAL, the tuples are in no particular order; if we are in
	 * state SORTEDINMEM, the tuples are in final sorted order; in states
	 * BUILDRUNS and FINALMERGE, the tuples are organized in "heap" order
	 * per Algorithm H.  (Note that memtupcount only counts the tuples
	 * that are part of the heap --- during merge passes, memtuples[]
	 * entries beyond TAPERANGE are never in the heap and are used to hold
	 * pre-read tuples.)  In state SORTEDONTAPE, the array is not used.
	 */
	void	  **memtuples;		/* array of pointers to palloc'd tuples */
	int			memtupcount;	/* number of tuples currently present */
	int			memtupsize;		/* allocated length of memtuples array */

	/*
	 * While building initial runs, this array holds the run number for
	 * each tuple in memtuples[].  During merge passes, we re-use it to
	 * hold the input tape number that each tuple in the heap was read
	 * from, or to hold the index of the next tuple pre-read from the same
	 * tape in the case of pre-read entries.  This array is never
	 * allocated unless we need to use tapes.  Whenever it is allocated,
	 * it has the same length as memtuples[].
	 */
	int		   *memtupindex;	/* index value associated with
								 * memtuples[i] */

	/*
	 * While building initial runs, this is the current output run number
	 * (starting at 0).  Afterwards, it is the number of initial runs we
	 * made.
	 */
	int			currentRun;

	/*
	 * These variables are only used during merge passes.  mergeactive[i]
	 * is true if we are reading an input run from (actual) tape number i
	 * and have not yet exhausted that run.  mergenext[i] is the memtuples
	 * index of the next pre-read tuple (next to be loaded into the heap)
	 * for tape i, or 0 if we are out of pre-read tuples.  mergelast[i]
	 * similarly points to the last pre-read tuple from each tape.
	 * mergeavailmem[i] is the amount of unused space allocated for tape
	 * i. mergefreelist and mergefirstfree keep track of unused locations
	 * in the memtuples[] array.  memtupindex[] links together pre-read
	 * tuples for each tape as well as recycled locations in
	 * mergefreelist. It is OK to use 0 as a null link in these lists,
	 * because memtuples[0] is part of the merge heap and is never a
	 * pre-read tuple.
	 */
	bool		mergeactive[MAXTAPES];	/* Active input run source? */
	int			mergenext[MAXTAPES];	/* first preread tuple for each
										 * source */
	int			mergelast[MAXTAPES];	/* last preread tuple for each
										 * source */
	long		mergeavailmem[MAXTAPES];		/* availMem for prereading
												 * tapes */
	long		spacePerTape;	/* actual per-tape target usage */
	int			mergefreelist;	/* head of freelist of recycled slots */
	int			mergefirstfree; /* first slot never used in this merge */

	/*
	 * Variables for Algorithm D.  Note that destTape is a "logical" tape
	 * number, ie, an index into the tp_xxx[] arrays.  Be careful to keep
	 * "logical" and "actual" tape numbers straight!
	 */
	int			Level;			/* Knuth's l */
	int			destTape;		/* current output tape (Knuth's j, less 1) */
	int			tp_fib[MAXTAPES];		/* Target Fibonacci run counts
										 * (A[]) */
	int			tp_runs[MAXTAPES];		/* # of real runs on each tape */
	int			tp_dummy[MAXTAPES];		/* # of dummy runs for each tape
										 * (D[]) */
	int			tp_tapenum[MAXTAPES];	/* Actual tape numbers (TAPE[]) */

	/*
	 * These variables are used after completion of sorting to keep track
	 * of the next tuple to return.  (In the tape case, the tape's current
	 * read position is also critical state.)
	 */
	int			result_tape;	/* actual tape number of finished output */
	int			current;		/* array index (only used if SORTEDINMEM) */
	bool		eof_reached;	/* reached EOF (needed for cursors) */

	/* markpos_xxx holds marked position for mark and restore */
	long		markpos_block;	/* tape block# (only used if SORTEDONTAPE) */
	int			markpos_offset; /* saved "current", or offset in tape
								 * block */
	bool		markpos_eof;	/* saved "eof_reached" */

	/*
	 * These variables are specific to the HeapTuple case; they are set by
	 * tuplesort_begin_heap and used only by the HeapTuple routines.
	 */
	TupleDesc	tupDesc;
	int			nKeys;
	ScanKey		scanKeys;
	SortFunctionKind *sortFnKinds;

	/*
	 * These variables are specific to the IndexTuple case; they are set
	 * by tuplesort_begin_index and used only by the IndexTuple routines.
	 */
	Relation	indexRel;
	ScanKey		indexScanKey;
	bool		enforceUnique;	/* complain if we find duplicate tuples */

	/*
	 * These variables are specific to the Datum case; they are set by
	 * tuplesort_begin_datum and used only by the DatumTuple routines.
	 */
	Oid			datumType;
	Oid			sortOperator;
	FmgrInfo	sortOpFn;		/* cached lookup data for sortOperator */
	SortFunctionKind sortFnKind;
	/* we need typelen and byval in order to know how to copy the Datums. */
	int			datumTypeLen;
	bool		datumTypeByVal;
};

/* End movement from tuplesort.c */

/*
 * We provide two different interfaces to what is essentially the same
 * code: one for sorting HeapTuples and one for sorting IndexTuples.
 * They differ primarily in the way that the sort key information is
 * supplied.
 * Yet a third slightly different interface supports sorting bare Datums.
 */

extern Tuplesortstate *tuplesort_begin_heap(TupleDesc tupDesc,
					 int nkeys,
					 Oid *sortOperators, AttrNumber *attNums,
					 bool randomAccess);
extern Tuplesortstate *tuplesort_begin_index(Relation indexRel,
					  bool enforceUnique,
					  bool randomAccess);
extern Tuplesortstate *tuplesort_begin_datum(Oid datumType,
					  Oid sortOperator,
					  bool randomAccess);

extern void tuplesort_puttuple(Tuplesortstate *state, void *tuple);

extern void tuplesort_putdatum(Tuplesortstate *state, Datum val,
				   bool isNull);

extern void tuplesort_performsort(Tuplesortstate *state);

extern void *tuplesort_gettuple(Tuplesortstate *state, bool forward,
				   bool *should_free);

#define tuplesort_getheaptuple(state, forward, should_free) \
	((HeapTuple) tuplesort_gettuple(state, forward, should_free))
#define tuplesort_getindextuple(state, forward, should_free) \
	((IndexTuple) tuplesort_gettuple(state, forward, should_free))

extern bool tuplesort_getdatum(Tuplesortstate *state, bool forward,
				   Datum *val, bool *isNull);

extern void tuplesort_end(Tuplesortstate *state);

/*
 * These routines may only be called if randomAccess was specified 'true'.
 * Likewise, backwards scan in gettuple/getdatum is only allowed if
 * randomAccess was specified.
 */

extern void tuplesort_rescan(Tuplesortstate *state);
extern void tuplesort_markpos(Tuplesortstate *state);
extern void tuplesort_restorepos(Tuplesortstate *state);

extern void SelectSortFunction(Oid sortOperator,
				   RegProcedure *sortFunction,
				   SortFunctionKind *kind);

/*
 * Apply a sort function (by now converted to fmgr lookup form)
 * and return a 3-way comparison result.  This takes care of handling
 * NULLs and sort ordering direction properly.
 */
extern int32 ApplySortFunction(FmgrInfo *sortFunction, SortFunctionKind kind,
				  Datum datum1, bool isNull1,
				  Datum datum2, bool isNull2);

#endif   /* TUPLESORT_H */

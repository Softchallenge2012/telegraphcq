/*-------------------------------------------------------------------------
 *
 * nodeHashAgg.h
 *
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeHashAgg.h,v 1.2 2003/07/16 09:42:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEHASHAGG_H
#define NODEHASHAGG_H

#include "fmgr.h"
#include "nodes/plannodes.h"
#include "utils/tuplesort.h"
#include "utils/window_agg.h"

/*
 * AggStatePerAggData - per-aggregate working state for the Agg scan
 */
typedef struct AggStatePerAggData
{
	/*
	 * These values are set up during ExecInitAgg() and do not change
	 * thereafter:
	 */

	/* Link to Aggref node this working state is for */
	Aggref	   *aggref;

	/* Oids of transfer functions */
	Oid			transfn_oid;
	Oid			finalfn_oid;	/* may be InvalidOid */

	/*
	 * fmgr lookup data for transfer functions --- only valid when
	 * corresponding oid is not InvalidOid.  Note in particular that
	 * fn_strict flags are kept here.
	 */
	FmgrInfo	transfn;
	FmgrInfo	finalfn;

	/*
	 * These functions allow us to extract and remove the state of the
	 * aggregate to enable state movement.
	 */
	FmgrInfo	getstatefn;
	FmgrInfo	getsizefn;

	/*
	 * Type of input data and Oid of sort operator to use for it; only
	 * set/used when aggregate has DISTINCT flag.  (These are not used
	 * directly by nodeAgg, but must be passed to the Tuplesort object.)
	 */
	Oid			inputType;
	Oid			sortOperator;

	/*
	 * fmgr lookup data for input type's equality operator --- only
	 * set/used when aggregate has DISTINCT flag.
	 */
	FmgrInfo	equalfn;

	/*
	 * initial value from pg_aggregate entry
	 */
	Datum		initValue;
	bool		initValueIsNull;

	/*
	 * We need the len and byval info for the agg's input, result, and
	 * transition data types in order to know how to copy/delete values.
	 */
	int16		inputtypeLen,
				resulttypeLen,
				transtypeLen;
	bool		inputtypeByVal,
				resulttypeByVal,
				transtypeByVal;

	/*
	 * These values are working state that is initialized at the start of
	 * an input tuple group and updated for each input tuple.
	 *
	 * For a simple (non DISTINCT) aggregate, we just feed the input values
	 * straight to the transition function.  If it's DISTINCT, we pass the
	 * input values into a Tuplesort object; then at completion of the
	 * input tuple group, we scan the sorted values, eliminate duplicates,
	 * and run the transition function on the rest.
	 */

	Tuplesortstate *sortstate;	/* sort object, if a DISTINCT agg */

	AggWindowSpec *w_spec;

} AggStatePerAggData;

/*
 * AggStatePerGroupData - per-aggregate-per-group working state
 *
 * These values are working state that is initialized at the start of
 * an input tuple group and updated for each input tuple.
 */
typedef struct AggStatePerGroupData
{
	Datum		transValue;		/* current transition value */
	bool		transValueIsNull;

	bool		noTransValue;	/* true if transValue not set yet */

	/*
	 * Note: noTransValue initially has the same value as
	 * transValueIsNull, and if true both are cleared to false at the same
	 * time.  They are not the same though: if transfn later returns a
	 * NULL, we want to keep that NULL and not auto-replace it with a
	 * later input value. Only the first non-NULL input will be
	 * auto-substituted.
	 */

	char	   *agg_state;		/* Used by windowed aggregates to maintain
								 * transient state. Typically its a
								 * pointer for complex CQ aggregates.
								 *
								 * For windowed aggs, transValue is no longer
								 * used for intermediate state. Rather, it
								 * is used to return intermediate results. */

	int			as_size;		/* Size of agg_state, so I do not have to
								 * worry about alignment issues */

}	AggStatePerGroupData;

/*
 * To implement hashed aggregation, we need a hashtable that stores a
 * representative tuple and an array of AggStatePerGroup structs for each
 * distinct set of GROUP BY column values.	We compute the hash key from
 * the GROUP BY columns.
 */
typedef struct AggHashEntryData *AggHashEntry;

typedef struct AggHashEntryData
{
	TupleHashEntryData shared;	/* common header for hash table entries */
	/* per-aggregate transition status array - must be last! */
	AggStatePerGroupData pergroup[1];	/* VARIABLE LENGTH ARRAY */
}	AggHashEntryData;	/* VARIABLE LENGTH STRUCT */

extern TupleTableSlot *ExecHashAgg(HashAgg * node);
extern bool ExecInitHashAgg(HashAgg * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsHashAgg(HashAgg * node);
extern void ExecEndHashAgg(HashAgg * node);
extern void ExecReScanHashAgg(HashAgg * node, ExprContext *exprCtxt, Plan *parent);

extern void build_hash_table(HashAgg * node);

#endif   /* NODEAGG_H */

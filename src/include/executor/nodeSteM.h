/*-------------------------------------------------------------------------
 *
 * nodeSteM.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeSteM.h,v 1.20 2005/05/04 02:28:39 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef NODESTEM_H
#define NODESTEM_H

#include "nodes/plannodes.h"

/* extern TupleTableSlot *ExecHash(Hash *node); */

extern bool ExecInitSteM(SteM * node, EState *estate, Plan *parent);

extern int	ExecCountSlotsSteM(SteM * node);

extern void ExecEndSteM(SteM * node);

extern SteMHashJoinTable ExecSteMHashTableCreate(SteM * node);

extern void ExecSteMHashTableDestroy(SteMHashJoinTable hashtable);

extern IntermediateHeapTupleData *ExecSteMHashTableInsert(SteM * stem, SteMHashJoinTable hashtable, ExprContext *econtext, Node *hashkey, bool whether_to_copy);

extern int	ExecSteMHashGetBucket(SteM *stem, SteMHashJoinTable hashtable, ExprContext *econtext, Node *hashkey);

extern void ExecSteMSetNewOuter(SteM * stem, ExprContext *econtext);

extern void printSteMHashJoinTable(SteMHashJoinTable shjt, TupleDesc *base_tuple_descs);

extern void ExecSteMInsert(SteM * stem, ExprContext *econtext, bool whether_to_copy);

extern IntermediateHeapTuple ExecScanSteMHashBucket(SteMState * stemstate, List *stemclauses, ExprContext *econtext);

/* extern int ExecHashGetBucket(HashJoinTable hashtable, ExprContext *econtext, Node *hashkey); */
/* extern HeapTuple ExecScanHashBucket(HashJoinState *hjstate, List *hjclauses, ExprContext *econtext); */

extern void ExecSteMHashTableReset(SteMHashJoinTable hashtable, long ntuples);

extern TupleTableSlot *ExecSteM(SteM * stem);

extern TupleTableSlot *ExecSteMProbeGetNext(SteM * stem);

extern TupleTableSlot *ExecSteMPromoteGetNext(SteM * stem);

/* extern void ExecReScanHash(Hash *node, ExprContext *exprCtxt, Plan *parent); */
/* extern void ExecChooseHashTableSize(double ntuples, int tupwidth, int *virtualbuckets, int *physicalbuckets, int *numbatches); */

extern void ExecSteMHashTableDeleteFirstTupleBucket(SteMHashJoinTable hashtable, int bucketNo);

extern IntermediateHeapTuple *SteMGetOldestTuple(SteM * node);
extern IntermediateHeapTuple *SteMGetNewestTuple(SteM * node);
extern void ExecSteMDemoteAllTuples(SteM * stem);

extern bool SteMGetContainsRelation(Eddy * cur, SteM * node, int windowrteidx);

extern void SteMPrintData(SteM * stem);
extern void SteMPrintData_Debug(SteM * stem);
void SteMPrintData_Debug_with_level(SteM * stem, int level);
void		ExecSteMPopulateHashTable(SteM * stem);


void		ExecSteMSetNewOuterFromPendingProbes(SteM * stem);

bool		ExecSteMHasPendingProbes(SteM * stem);


/*
 * Heap data structures required for duplicate elimination
 */
typedef struct SteMHJTupleHeap
{
	SteMHashJoinTuple *shjt_tuples;
	int			last_position;
	int			max_tuples;
}	SteMHJTupleHeap;

SteMHJTupleHeap *InitSteMHJTupleHeap(int num_max);

void		DestroySteMHJTupleHeap(SteMHJTupleHeap * heap);

void		SteMHJTupleHeapInsert(SteMHJTupleHeap * heap, SteMHashJoinTuple shjt);

SteMHashJoinTuple SteMHJTupleHeapRemoveSmallest(SteMHJTupleHeap * heap);

int			IHTCompare(IntermediateHeapTuple iht1, IntermediateHeapTuple iht2);

bool		SteMHJTupleSmallerThan(SteMHashJoinTuple shjt1, SteMHashJoinTuple shjt2);

/* Used by nodeFSteM */
extern bool stem_contains_tuple(SteM * stem, ExprContext *econtext);


/* TBD TBD TBD - Amol: commented code is from the BETA - please check that the
 * right thing has been done
 */

/* extern IntermediateHeapTuple *SteMGetOldestTuple(SteM * node); */
/* extern IntermediateHeapTuple *SteMGetNewestTuple(SteM * node); */
/* extern void ExecSteMDemoteAllTuples(SteM * stem); */

/* extern bool SteMGetContainsRelation(Eddy * cur, SteM * node, int windowrteidx); */

/* extern void SteMPrintData(SteM * stem); */
/* extern void SteMPrintData_Debug(SteM * stem); */
/* extern void SteMPrintData_Debug_with_level(SteM * stem, int level); */
/* void		ExecSteMPopulateHashTable(SteM * stem); */


/* void		ExecSteMSetNewOuterFromPendingProbes(SteM * stem); */

/* bool		ExecSteMHasPendingProbes(SteM * stem); */


/* typedef struct SteMHJTupleHeap */
/* { */
/* 	SteMHashJoinTuple *shjt_tuples; */
/* 	int			last_position; */
/* }	SteMHJTupleHeap; */

/* SteMHJTupleHeap *InitSteMHJTupleHeap(int num_max); */

/* void		DestroySteMHJTupleHeap(SteMHJTupleHeap * heap); */

/* void		SteMHJTupleHeapInsert(SteMHJTupleHeap * heap, SteMHashJoinTuple shjt); */

/* SteMHashJoinTuple SteMHJTupleHeapRemoveSmallest(SteMHJTupleHeap * heap); */

/* int			IHTCompare(IntermediateHeapTuple iht1, IntermediateHeapTuple iht2); */

/* bool		SteMHJTupleSmallerThan(SteMHashJoinTuple shjt1, SteMHashJoinTuple shjt2); */

#endif   /* NODESTEM_H */

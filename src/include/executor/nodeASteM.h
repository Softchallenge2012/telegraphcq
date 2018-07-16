/*-------------------------------------------------------------------------
 *
 * nodeASteM.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeASteM.h,v 1.2 2004/01/22 21:17:31 garrett Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEASTEM_H
#define NODEASTEM_H

#include "nodes/plannodes.h"

#define ASTEM_NOT_AN_INDEX -1
#define ASTEM_NOT_A_TIME = -1

/* Locations of the arguments to <same_stem_expr> */
#define ASTEM_OLDER_TUPLE_VARNO INNER
#define ASTEM_NEWER_TUPLE_VARNO OUTER

/* The maximum number of tuples to en/dequeue per call to ProcessNext. */
#define ASTEM_MAX_WORK_UNITS 1

/* Macro to simplify wrapping around the substems[] array. */
#define ASTEM_NEXT_SS_INDEX(cur) (((cur)+1)%ASTEM_NUM_SUBSTEMS)

/* used to initialize the ASteM */
extern bool ExecInitASteM(ASteM *node, EState *estate, Plan *parent);

/* used to count tuple table slots in executor */
extern int ExecCountSlotsASteM(ASteM *node);

/* used to destroy an ASteM */
extern bool ExecEndASteM(ASteM *node);

/* Provided to be part of Executor Interface, but it doesn't really work... */
extern TupleTableSlot *ExecASteM(ASteM * node);

/* used to build a tuple from the ASteM's leftchild into the ASteM */
extern TupleTableSlot *ExecASteMBuildTupleAuto(ASteM *node);

/* used to build a particular tuple into the AStem (called by previous function) */
extern TupleTableSlot *ExecASteMBuildTuple(ASteM *node, TupleTableSlot *buildTuple);

/* used to set a new probe (a new outer tuple) for the ASteM */
extern bool ExecASteMSetNewProbe(ASteM *node, ExprContext *newcontext);

/* used to get the next output tuple, if a probe is already in place */
extern TupleTableSlot *ExecASteMGetNextOutputTuple(ASteM *node);

/* used to tell if an ASteM has a probe or not */
extern bool ASteMHasProbe(ASteM *node);

/* used to set the timestamp accessor for this ASteM */
extern void SetTimestampAccessorASteM(ASteM * node, Var * ts_accessor);

/* used to add windows to ASteM's */
extern void AddWindowASteM(ASteM * node, Const * window_size);

/* used to clear the window on an ASteM */
extern void ClearWindowASteM(ASteM * node);

/* Used to get a representative stem of the stem's the astem uses for windowing */
extern SteM *GetRepresentativeSteMASteM(ASteM * node);

#endif   /* NODEASTEM_H */

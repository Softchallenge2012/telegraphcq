/*-------------------------------------------------------------------------
 *
 * nodeFSteM.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFSteM.h,v 1.11 2004/07/15 20:39:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFSTEM_H
#define NODEFSTEM_H

#include "nodes/plannodes.h"

#define FSTEM_NOT_AN_INDEX -1
#define FSTEM_NOT_A_TIME = -1

/* Locations of the arguments to <same_stem_expr> */
#define FSTEM_OLDER_TUPLE_VARNO INNER
#define FSTEM_NEWER_TUPLE_VARNO OUTER

/* The maximum number of tuples to en/dequeue per call to ProcessNext. */
#define FSTEM_MAX_WORK_UNITS 1

/* Macro to simplify wrapping around the substems[] array. */
#define FSTEM_NEXT_SS_INDEX(cur) (((cur)+1)%FSTEM_NUM_SUBSTEMS)

extern bool ProcessInitFSteM(FSteM * node, EState *estate, Plan *parent);
extern bool ProcessEndFSteM(FSteM * node, Plan *parent);
extern void ProcessNextFSteM(FSteM * node, Plan *parent);
extern bool ProcessRemoveQueryFSteM(FSteM * node, int32 queryid, Plan *parent);
extern void ProcessPrintStatsFSteM(FSteM * node, Plan *parent);

extern void SetTimestampAccessorFSteM(FSteM * node, Var *ts_accessor);
extern void AddWindowFSteM(FSteM * node, Const *window_size);
extern void ClearWindowFSteM(FSteM * node);
extern SteM *GetRepresentativeSteMFSteM(FSteM * node);
extern void FSteMPrintData_debug(Eddy * eddy, FSteM * node, int stem_no);
extern void FSteMPrintData_debug_with_level(Eddy * eddy, FSteM * node, int stem_no, int level);
extern void FSteMSetMaxWorkPerIter(FSteM * node, int numtuples);
extern bool FSteMContainsTuple(FSteM * node, IntermediateHeapTuple iht, Eddy * parent);
#endif   /* NODEFSTEM_H */

/*-------------------------------------------------------------------------
 *
 * nodeFSched.h
 *
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFSCHED_H
#define NODEFSCHED_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecFSched(FSched * node);
extern bool ExecInitFSched(FSched * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsFSched(FSched * node);
extern void ExecEndFSched(FSched * node);
extern void init_fsched_acceptfd(void);
extern void set_failed_machine(FSched * node, int machine);
extern void set_recovered_fluxen(FSched * node, int f_idx);
#endif   /* NODEFSCHED_H */

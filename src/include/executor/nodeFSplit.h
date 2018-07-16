/*-------------------------------------------------------------------------
 *
 * nodeFSplit.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFSplit.h,v 1.2 2004/02/13 01:44:03 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFSPLIT_H
#define NODEFSPLIT_H

#include "nodes/plannodes.h"

extern bool ProcessInitFSplit(FSplit * node, EState *estate, Plan *parent);
extern bool ProcessEndFSplit(FSplit * node, Plan *parent);
extern void ProcessNextFSplit(FSplit * node, Plan *parent);
extern bool ProcessRemoveQueryFSplit(FSplit * foutput,int queryid,Plan *parent);
extern void ProcessPrintStatsFSplit(FSplit * node, Plan *parent);

#endif /* NODEFSPLIT_H */

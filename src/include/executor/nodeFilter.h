/*-------------------------------------------------------------------------
 *
 * nodeFilter.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFilter.h,v 1.7 2003/07/17 22:52:43 amol Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFILTER_H
#define NODEFILTER_H

#include "nodes/plannodes.h"

extern bool ProcessInitFilter(Filter * node, EState *estate, Plan *parent);
extern bool ProcessEndFilter(Filter * node, Plan *parent);
extern TupleTableSlot *ProcessNextFilter(Filter * node, TupleTableSlot *t);
extern bool ExecEvaluateFilter(Filter *node, ExprContext *econtext);


#endif   /* NODEFILTER_H */

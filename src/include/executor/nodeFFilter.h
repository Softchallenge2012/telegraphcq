/*-------------------------------------------------------------------------
 *
 * nodeFFilter.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFFilter.h,v 1.3 2004/07/15 20:39:58 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFFILTER_H
#define NODEFFILTER_H

#include "nodes/plannodes.h"

extern bool verifyConstTree(ExprContext *econtext,
				ConstTree * ct,
				FunctionCachePtr fcn,
				Const *c,
				bool expectedRet);

extern bool ProcessInitFFilter(FFilter * node, EState *estate, Plan *parent);
extern bool ProcessEndFFilter(FFilter * node, Plan *parent);
extern void ProcessNextFFilter(FFilter * node, Plan *parent);
extern bool ProcessRemoveQueryFFilter(FFilter * ffilter, int queryid, Plan *parent);
extern void ProcessPrintStatsFFilter(FFilter * node, Plan *parent);

#endif   /* NODEFFILTER_H */

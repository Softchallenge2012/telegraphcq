/*-------------------------------------------------------------------------
 *
 * nodeGSFilter.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeGSFilter.h,v 1.9 2004/02/13 01:44:03 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEGSFILTER_H
#define NODEGSFILTER_H

#include "nodes/plannodes.h"

extern bool verifyConstTree(ExprContext *econtext,
				ConstTree * ct,
				FunctionCachePtr fcn,
				Const *c,
				bool expectedRet);

extern bool ProcessInitGSFilter(GSFilter * node, EState *estate, Plan *parent);
extern bool ProcessEndGSFilter(GSFilter * node, Plan *parent);
extern void ProcessNextGSFilter(GSFilter * node, Plan *parent);
extern bool ProcessRemoveQueryGSF(GSFilter * gsfilter, int queryid, Plan *parent);
extern void ProcessPrintStatsGSFilter(GSFilter * node, Plan *parent);

#endif   /* NODEGSFILTER_H */

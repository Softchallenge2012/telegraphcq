/*-------------------------------------------------------------------------
 *
 * nodeFOutput.h
 *
 * This operator uses the new eddy "ProcessNode" interface as opposed to the
 * standard Iterator interface
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFOutput.h,v 1.2 2004/02/13 01:44:03 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFOUTPUT_H
#define NODEFOUTPUT_H

#include "nodes/plannodes.h"

extern bool ProcessInitFOutput(FOutput * node, EState *estate, Plan *parent);
extern bool ProcessEndFOutput(FOutput * node, Plan *parent);
extern void ProcessNextFOutput(FOutput * node, Plan *parent);
extern bool ProcessRemoveQueryFOutput(FOutput * foutput,int queryid,Plan *parent);
extern void ProcessPrintStatsFOutput(FOutput * node, Plan *parent);

#endif /* NODEFOUTPUT_H */

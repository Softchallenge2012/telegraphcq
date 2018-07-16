/*-------------------------------------------------------------------------
 *
 * nodeHashGroup.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 *
 * $Id: nodeHashGroup.h,v 1.7 2003/12/16 22:56:24 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEHASHGROUP_H
#define NODEHASHGROUP_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecHashGroup(HashGroup * node);
extern bool ExecInitHashGroup(HashGroup * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsHashGroup(HashGroup * node);
extern void ExecEndHashGroup(HashGroup * node);

extern bool ExecGetStateInitHashGroup(HashGroup * node);
extern bool ExecGetStateNextHashGroup(HashGroup * node,
						  char *buf, int bsize);
extern bool ExecGetStateCloseHashGroup(HashGroup * node);

extern bool ExecInstallStateInitHashGroup(HashGroup * node);
extern bool ExecInstallStateNextHashGroup(HashGroup * node,
							  char *page, int bsize);
extern bool ExecInstallStateCloseHashGroup(HashGroup * node);

extern bool ExecResetStateHashGroup(HashGroup * node);

extern void ExecDumpHashGroup(HashGroup * node, FILE *fp);

#endif   /* NODEHASHGROUP_H */

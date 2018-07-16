/*-------------------------------------------------------------------------
 *
 * nodeDataGen
 *	 Author: mashah
 *	 Generates random data.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeDataGen.h,v 1.4 2003/12/16 22:56:24 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEDATAGEN_H
#define NODEDATAGEN_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecDataGen(DataGen * node);
extern bool ExecInitDataGen(DataGen * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsDataGen(DataGen * node);
extern void ExecEndDataGen(DataGen * node);

extern bool ExecGetStateInitDataGen(DataGen * node);
extern bool ExecGetStateNextDataGen(DataGen * node, char * buf, int bsize);
extern bool ExecGetStateCloseDataGen(DataGen * node);

extern bool ExecInstallStateInitDataGen(DataGen * node);
extern bool ExecInstallStateNextDataGen(DataGen * node, char * buf, int bsize);
extern bool ExecInstallStateCloseDataGen(DataGen * node);

extern bool ExecResetStateDataGen(DataGen * node);

#endif   /* NODEDATAGEN_H */

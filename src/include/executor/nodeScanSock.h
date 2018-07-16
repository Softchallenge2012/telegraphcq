/*-------------------------------------------------------------------------
 *
 * nodeScanSock.h
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeScanSock.h,v 1.1 2005/12/14 20:49:24 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODESCANSOCK_H
#define NODESCANSOCK_H

#include "nodes/plannodes.h"

extern TupleTableSlot *ExecScanSock(ScanSock * node);
extern bool ExecInitScanSock(ScanSock * node, EState *estate, Plan *parent);
extern int	ExecCountSlotsScanSock(ScanSock * node);
extern void ExecEndScanSock(ScanSock * node);
extern SockState* ScanSockGetSocket(ScanSock *node);
extern void ScanSockSetSocket(ScanSock *node, SockState *sock, Eddy *e);

#endif   /* NODESCANSOCK_H */

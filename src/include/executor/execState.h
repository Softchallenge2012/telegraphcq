/*-------------------------------------------------------------------------
 *
 * execState.h
 *	  support for state extraction and installation.
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/include/executor/execState.h,v 1.6 2004/03/24 08:11:28 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef EXECSTATE_H
#define EXECSTATE_H

#include "nodes/plannodes.h"

extern bool ExecGetStateInit(Plan *node);
extern bool ExecGetStateNext(Plan *node, char *buf, int bsize);
extern bool ExecGetStateClose(Plan *node);
extern bool ExecInstallStateInit(Plan *node);
extern bool ExecInstallStateNext(Plan *node, char *buf, int bsize);
extern bool ExecInstallStateClose(Plan *node);
extern bool ExecResetState(Plan *node);

/* For testing purposes */
extern void ExecDump(Plan *node, FILE *fp);

#endif   /* EXECSTATE_H */

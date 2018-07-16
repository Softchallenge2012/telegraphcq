/*-------------------------------------------------------------------------
 *
 * lockcmds.h
 *	  prototypes for lockcmds.c.
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: lockcmds.h,v 1.1.1.1 2003/04/17 23:02:14 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef LOCKCMDS_H
#define LOCKCMDS_H

#include "nodes/parsenodes.h"

/*
 * LOCK
 */
extern void LockTableCommand(LockStmt *lockstmt);

#endif   /* LOCKCMDS_H */

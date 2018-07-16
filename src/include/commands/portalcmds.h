/*-------------------------------------------------------------------------
 *
 * portalcmds.h
 *	  prototypes for portalcmds.c.
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: portalcmds.h,v 1.3 2006/02/02 01:46:48 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PORTALCMDS_H
#define PORTALCMDS_H

#include "utils/portal.h"

/*
 * PerformPortalFetch
 *		Performs the POSTQUEL function FETCH.  Fetches count (or all if 0)
 * tuples in portal with name in the forward direction iff goForward.
 *
 * Exceptions:
 *		BadArg if forward invalid.
 *		"ERROR" if portal not found.
 */
extern void PerformPortalFetch(char *name, bool forward, bool window, int count,
				   CommandDest dest, char *completionTag);

/*
 * PerformPortalClose
 *		Performs the POSTQUEL function CLOSE.
 */
extern void PerformPortalClose(char *name, CommandDest dest);

extern void PortalCleanup(Portal portal);

/* PerformPortalTimeMove
 * Upon receiving a "move forward/backward cursor in time by ['interval']",
 * update the EState's OutputBufferState so that next time we ask for tuples,
 * we'll know where to start.  We don't actually scroll the heap scanner to
 * the correct times or run the query for more tuples until the client
 * asks for them (through a "fetch" command).
 */
void
PerformPortalTimeMove(char *name,
		      bool forward,
		      Const* interval,
		      char* completionTag);

#endif   /* PORTALCMDS_H */

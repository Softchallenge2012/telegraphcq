/*-------------------------------------------------------------------------
 *
 * backendid.h
 *	  POSTGRES backend id communication definitions
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: backendid.h,v 1.1.1.2 2003/04/17 23:02:43 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef BACKENDID_H
#define BACKENDID_H

/* ----------------
 *		-cim 8/17/90
 * ----------------
 */
typedef int BackendId;			/* unique currently active backend
								 * identifier */

#define InvalidBackendId		(-1)

extern BackendId MyBackendId;	/* backend id of this backend */

#endif   /* BACKENDID_H */

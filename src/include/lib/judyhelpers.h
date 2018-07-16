/*-------------------------------------------------------------------------
 *
 * judyhelpers.h
 *
 *    Helper functions that use Judy arrays.
 *
 * Notes:
 *
 *   (1) See http://judy.sourceforge.net for more information on Judy
 *   (2) Ideally we should try and roll these into Judy
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: judyhelpers.h,v 1.2 2004/02/13 01:44:04 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef JUDYHELPERS_H
#define JUDYHELPERS_H

#include <Judy.h>

/*
 * Function prototypes
 */

extern Pvoid_t J1Copy(Pvoid_t src);

#endif   /* JUDYHELPERS_H */

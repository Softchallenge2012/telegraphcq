/*-------------------------------------------------------------------------
 *
 * isnan.c
 *	  isnan() implementation
 *
 * Copyright (c) 1999, repas AEG Automation GmbH
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/port/qnx4/isnan.c,v 1.2 2004/03/24 08:11:05 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "c.h"

unsigned char __nan[8] = __nan_bytes;

int
isnan(double dsrc)
{
	return !memcmp(&dsrc, &NAN, sizeof(double));
}

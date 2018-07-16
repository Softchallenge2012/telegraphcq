/*-------------------------------------------------------------------------
 *
 * misceddy.h
 *	  POSTGRES Miscellaneaous Eddy definitions
 *
 * This file  contains miscellaneous definitions for the new eddy operator
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: misceddy.h,v 1.3 2003/03/20 02:28:21 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef MISCEDDY_H
#define MISCEDDY_H

#include "pg_config.h"

/*
 * MaxModulesEddyQuery limits the number of modules in an Eddy query.
 */
#define MAX_MODULES_EDDY_QUERY	   24

/*
 * EddyReadyBytes defines the number of bytes used for the ready bitmap
 */
#define EDDY_READY_BYTES		  (MAX_MODULES_EDDY_QUERY/BITS_PER_BYTE)

#endif   /* MISCEDDY_H */

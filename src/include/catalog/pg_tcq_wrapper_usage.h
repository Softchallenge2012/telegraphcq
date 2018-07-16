/*-------------------------------------------------------------------------
 *
 * pg_tcq_wrapper_usage.h
 *	  definition of the system "procedure" relation (pg_tcq_wrapper_usage)
 *	  along with the relation's initial contents.
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 * $Id: pg_tcq_wrapper_usage.h,v 1.3 2003/07/17 05:18:50 sailesh Exp $
 *
 * NOTES
 *	  The script catalog/genbki.sh reads this file and generates .bki
 *	  information from the DATA() statements.  utils/Gen_fmgrtab.sh
 *	  generates fmgroids.h and fmgrtab.c the same way.
 *
 *	  XXX do NOT break up DATA() statements into multiple lines!
 *		  the scripts are not as smart as you might think...
 *	  XXX (eg. #if 0 #endif won't do what you think)
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_TCQ_WRAPPER_USAGE_H
#define PG_TCQ_WRAPPER_USAGE_H

#include "nodes/pg_list.h"

/* ----------------
 *		postgres.h contains the system type definintions and the
 *		CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *		can be read by both genbki.sh and the C compiler.
 * ----------------
 */

/* ----------------
 *		pg_tcq_wrapper_usage definition.  cpp turns this into
 *		typedef struct FormData_pg_tcq_wrapper_usage
 * ----------------
 */
CATALOG(pg_tcq_wrapper_usage)
{

	Oid			wrapperoid;
	Oid			streamoid;
} FormData_pg_tcq_wrapper_usage;

/* ----------------
 *		Form_pg_tcq_wrapper_usage corresponds to a pointer to a tuple with
 *		the format of pg_tcq_wrapper_usage relation.
 * ----------------
 */
typedef FormData_pg_tcq_wrapper_usage *Form_pg_tcq_wrapper_usage;

/* ----------------
 *		compiler constants for pg_tcq_wrapper_usage
 * ----------------
 */
#define Natts_pg_tcq_wrapper_usage					2
#define Anum_pg_tcq_wrapper_usage_wrapperoid			1
#define Anum_pg_tcq_wrapper_usage_streamoid			2

#endif

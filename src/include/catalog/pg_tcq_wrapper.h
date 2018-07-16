/*-------------------------------------------------------------------------
 *
 * pg_tcq_wrapper.h
 *	  definition of the system "wrapper" relation (pg_tcq_wrapper)
 *	  along with the relation's initial contents.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: pg_tcq_wrapper.h,v 1.7 2004/06/29 23:17:12 phred Exp $
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
#ifndef PG_TCQ_WRAPPER_H
#define PG_TCQ_WRAPPER_H

#include "nodes/pg_list.h"

/* ----------------
 *		postgres.h contains the system type definintions and the
 *		CATALOG(), BOOTSTRAP and DATA() sugar words so this file
 *		can be read by both genbki.sh and the C compiler.
 * ----------------
 */

/* ----------------
 *		pg_tcq_wrapper definition.	cpp turns this into
 *		typedef struct FormData_pg_tcq_wrapper
 * ----------------
 */
CATALOG(pg_tcq_wrapper)
{
	NameData	wrappername;
	Oid			initproc;
	Oid			nextproc;
	Oid			doneproc;
	Oid			rescanproc;
} FormData_pg_tcq_wrapper;

/* ----------------
 *		Form_pg_tcq_wrapper corresponds to a pointer to a tuple with
 *		the format of pg_tcq_wrapper relation.
 * ----------------
 */
typedef FormData_pg_tcq_wrapper *Form_pg_tcq_wrapper;

/* ----------------
 *		compiler constants for pg_tcq_wrapper
 * ----------------
 */
#define Natts_pg_tcq_wrapper					5
#define Anum_pg_tcq_wrapper_wrappername 1
#define Anum_pg_tcq_wrapper_initproc 2
#define Anum_pg_tcq_wrapper_nextproc 3
#define Anum_pg_tcq_wrapper_doneproc 4
#define Anum_pg_tcq_wrapper_rescanproc 5

DATA(insert OID = 9800 (  csvwrapper		9700 9701 9702 0));
DESCR("(internal)");
DATA(insert OID = 9801 (  quikwrapper		9711 9712 9713 0));
DESCR("(internal)");
DATA(insert OID = 9802 (  pdumpwrapper		9720 9721 9722 0));
DESCR("(internal)");

#endif   /* PG_TCQ_WRAPPER_H */

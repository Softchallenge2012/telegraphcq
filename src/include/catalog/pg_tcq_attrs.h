/*-------------------------------------------------------------------------
 *
 * pg_tcq_attrs.h
 *	  definition of the system "wrapper attributes" relation
 * (pg_tcq_attrs)
 *	  along with the relation's initial contents.
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 * $Id: pg_tcq_attrs.h,v 1.2 2003/07/17 05:18:49 sailesh Exp $
 *
 * NOTES
 *	  The script catalog/genbki.sh reads this file and generates .bki
 *	  information from the DATA() statements.  utils/Gen_fmgrtab.sh
 *	  generates fmgroids.h and fmgrtab.c the same way.
 *
 *	  XXX do NOT break up DATA() statements into multiple lines!
 *	  the scripts are not as smart as you might think...
 *	  XXX (eg. #if 0 #endif won't do what you think)
 *
 *-------------------------------------------------------------------------
 */
#ifndef PG_TCQ_ATTRS_H
#define PG_TCQ_ATTRS_H

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
CATALOG(pg_tcq_attrs)
{
	Oid			objoid;			/* oid for which this key value pair
								 * applies */
	text		key;			/* the name of the key */
	text		value;
} FormData_pg_tcq_attrs;

/* ----------------
 *		Form_pg_tcq_attrs corresponds to a pointer to a tuple with
 *		the format of pg_tcq_attrs relation.
 * ----------------
 */
typedef FormData_pg_tcq_attrs *Form_pg_tcq_attrs;

/* ----------------
 *		compiler constants for pg_tcq_attrs
 * ----------------
 */
#define Natts_pg_tcq_attrs					3
#define Anum_pg_tcq_attrs_objoid 1
#define Anum_pg_tcq_attrs_key 2
#define Anum_pg_tcq_attrs_value 3


#endif   /* PG_TCQ_WRAPPER_ATTRS_H */

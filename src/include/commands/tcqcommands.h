/*-------------------------------------------------------------------------
 *
 * tcqcommands.h
 *	  prototypes for the TelegraphCQ specific utility commands
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: tcqcommands.h,v 1.6 2004/08/07 01:01:38 owenc Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef TCQCOMMANDS_H
#define TCQCOMMANDS_H

#include "nodes/parsenodes.h"

#define CREATEWRAPPER_INIT_NAME "init"
#define CREATEWRAPPER_NEXT_NAME "next"
#define CREATEWRAPPER_DONE_NAME "done"
#define CREATEWRAPPER_RESCAN_NAME "rescan"


int			delete_wrapper_usages(Oid wrapperoid, Oid streamoid);
void		CreateStream(CreateStrmStmt * stmt);
void		DropStream(const RangeVar *relation, DropBehavior behavior);
void		CreateWrapper(CreateWrapperStmt * stmt);
void		DropWrapper(List *wrappername);
void AlterStreamWrapper(List *tablename,
				   List *wrappername,
				   List *bindings,
			char *info,
	   bool isCreate);

#endif

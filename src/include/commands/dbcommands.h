/*-------------------------------------------------------------------------
 *
 * dbcommands.h
 *
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: dbcommands.h,v 1.1.1.2 2003/04/17 23:02:14 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef DBCOMMANDS_H
#define DBCOMMANDS_H

#include "nodes/parsenodes.h"

extern void createdb(const CreatedbStmt *stmt);
extern void dropdb(const char *dbname);
extern void AlterDatabaseSet(AlterDatabaseSetStmt *stmt);

extern Oid	get_database_oid(const char *dbname);
extern Oid	get_database_owner(Oid dbid);

#endif   /* DBCOMMANDS_H */

/*-------------------------------------------------------------------------
 *
 * superuser.c
 *	  The superuser() function.  Determines if user has superuser privilege.
 *	  Also, a function to check for the owner (datdba) of a database.
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/misc/superuser.c,v 1.2 2004/03/24 08:11:19 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include "access/heapam.h"
#include "catalog/pg_shadow.h"
#include "commands/dbcommands.h"
#include "utils/syscache.h"
#include "miscadmin.h"


/*
 * The Postgres user running this command has Postgres superuser privileges
 *
 * All code should use either of these two functions to find out
 * whether a given user is a superuser, rather than evaluating
 * pg_shadow.usesuper directly, so that the escape hatch built in for
 * the single-user case works.
 */
bool
superuser(void)
{
	return superuser_arg(GetUserId());
}


bool
superuser_arg(Oid userid)
{
	bool		result = false;
	HeapTuple	utup;

	/* Special escape path in case you deleted all your users. */
	if (!IsUnderPostmaster && userid == BOOTSTRAP_USESYSID)
		return true;

	utup = SearchSysCache(SHADOWSYSID,
						  ObjectIdGetDatum(userid),
						  0, 0, 0);
	if (HeapTupleIsValid(utup))
	{
		result = ((Form_pg_shadow) GETSTRUCT(utup))->usesuper;
		ReleaseSysCache(utup);
	}
	return result;
}


/*
 * The Postgres user running this command is the owner of the specified
 * database.
 */
bool
is_dbadmin(Oid dbid)
{
	Oid			dba;

	dba = get_database_owner(dbid);

	return (GetUserId() == dba);
}

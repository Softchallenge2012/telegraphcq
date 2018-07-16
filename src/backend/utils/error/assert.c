/*-------------------------------------------------------------------------
 *
 * assert.c
 *	  Assert code.
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/error/assert.c,v 1.9 2005/06/13 02:59:28 phred Exp $
 *
 * NOTE
 *	  This should eventually work with elog()
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <unistd.h>

#ifdef __linux__
#include "execinfo.h"
/* Prints the stack trace to standard out when needed, perhaps we should elog? */

void
TraceDump(void)
{

	void	   *array[100];
	size_t		size;
	char	  **strings;
	size_t		i;

	size = backtrace(array, 100);
	strings = backtrace_symbols(array, size);

	elog(LOG,"Printing stack trace, with %zd frames:", size);

	for (i = 0; i < size; i++) 
	{
		elog(LOG,"%s", strings[i]);
	}

	elog(LOG,"Done with stack trace.");

	free(strings);
}

#else
void
TraceDump(void)
{
}
#endif
/*
 * ExceptionalCondition - Handles the failure of an Assert()
 */
int
ExceptionalCondition(char *conditionName,
					 char *errorType,
					 char *fileName,
					 int lineNumber)
{
	if (!PointerIsValid(conditionName)
		|| !PointerIsValid(fileName)
		|| !PointerIsValid(errorType))
		fprintf(stderr, "TRAP: ExceptionalCondition: bad arguments\n");
	else
	{
		fprintf(stderr, "TRAP: %s(\"%s\", File: \"%s\", Line: %d)\n",
				errorType, conditionName,
				fileName, lineNumber);
	}

	TraceDump();

#ifdef SLEEP_ON_ASSERT
	sleep(1000000);
#endif

	abort();

	return 0;
}

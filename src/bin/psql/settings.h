/*
 * psql - the PostgreSQL interactive terminal
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/bin/psql/settings.h,v 1.8 2004/03/24 08:11:26 chungwu Exp $
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include "libpq-fe.h"

#include "variables.h"
#include "print.h"

#define DEFAULT_FIELD_SEP "|"
#define DEFAULT_RECORD_SEP "\n"
#define DEFAULT_EDITOR	"vi"

#define DEFAULT_PROMPT1 "%/%R%# "
#define DEFAULT_PROMPT2 "%/%R%# "
#define DEFAULT_PROMPT3 ">> "


typedef struct _psqlSettings
{
	PGconn	   *db;				/* connection to backend */
	int			encoding;
	FILE	   *queryFout;		/* where to send the query results */
	bool		queryFoutPipe;	/* queryFout is from a popen() */

	printQueryOpt popt;
	VariableSpace vars;			/* "shell variable" repository */

	char	   *gfname;			/* one-shot file output argument for \g */

	bool		notty;			/* stdin or stdout is not a tty (as
								 * determined on startup) */
	bool		useReadline;	/* use libreadline routines */
	bool		useHistory;
	bool		getPassword;	/* prompt the user for a username and
								 * password */
	FILE	   *cur_cmd_source; /* describe the status of the current main
								 * loop */
	bool		cur_cmd_interactive;

	char	   *progname;		/* in case you renamed psql */
	char	   *inputfile;		/* for error reporting */
	unsigned	lineno;			/* also for error reporting */

	bool		issuper;		/* is the current user a superuser? (used
								 * to form the prompt) */
	bool		timing;			/* timing of all queries */
	unsigned	cq_rows_iter;	/* how many rows to fetch each time the
								 * client gets rows from the server.  0 =>
								 * 1 */
	unsigned	cq_rows_quer;	/* how many rows to fetch before stopping
								 * a streaming query.  0 => infinity */
} PsqlSettings;

extern PsqlSettings pset;


#define QUIET() (GetVariableBool(pset.vars, "QUIET"))


#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define EXIT_BADCONN 2

#define EXIT_USER 3

#endif

/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/bin/psql/help.h,v 1.2 2004/03/24 08:11:26 chungwu Exp $
 */
#ifndef HELP_H
#define HELP_H

void		usage(void);

void		slashUsage(bool pager);

void		helpSQL(const char *topic, bool pager);

void		print_copyright(void);

#endif

/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /project/eecs/db/cvsroot/postgres/src/bin/psql/print.h,v 1.5 2004/11/23 22:14:53 phred Exp $
 */
#ifndef PRINT_H
#define PRINT_H

#include "libpq-fe.h"

enum printFormat
{
	PRINT_NOTHING = 0,			/* to make sure someone initializes this */
	PRINT_UNALIGNED,
	PRINT_ALIGNED,
	PRINT_HTML,
	PRINT_LATEX
	/* add your favourite output format here ... */
};


typedef struct _printTableOpt
{
	enum printFormat format;	/* one of the above */
	bool		expanded;		/* expanded/vertical output (if supported
								 * by output format) */
	bool		pager;			/* use pager for output (if to stdout and
								 * stdout is a tty) */
	bool		tuples_only;	/* don't output headers, row counts, etc. */
	unsigned short int border;	/* Print a border around the table.
								 * 0=none, 1=dividing lines, 2=full */
	char	   *fieldSep;		/* field separator for unaligned text mode */
	char	   *recordSep;		/* record separator for unaligned text
								 * mode */
	char	   *tableAttr;		/* attributes for HTML <table ...> */
    bool        cq_mode;        /* Printing one line at a time? */
} printTableOpt;


/*
 * Use this to print just any table in the supported formats.
 * - title is just any string (NULL is fine)
 * - headers is the column headings (NULL ptr terminated). It must be given and
 *	 complete since the column count is generated from this.
 * - cells are the data cells to be printed. Now you know why the correct
 *	 column count is important
 * - footers are lines to be printed below the table
 * - align is an 'l' or an 'r' for every column, if the output format needs it.
 *	 (You must specify this long enough. Otherwise anything could happen.)
 *
 * - width is an optional parameter (NULL if unused) that lets you keep a
 *   persistent set of column widths across multiple calls.
*/
void printTable(const char *title, const char *const * headers,
		   const char *const * cells, const char *const * footers,
		   const char *align,
		   const printTableOpt *opt, FILE *fout,
           unsigned int *width);



typedef struct _printQueryOpt
{
	printTableOpt topt;			/* the options above */
	char	   *nullPrint;		/* how to print null entities */
	bool		quote;			/* quote all values as much as possible */
	char	   *title;			/* override title */
	char	  **footers;		/* override footer (default is "(xx
								 * rows)") */
	bool		default_footer; /* print default footer if footers==NULL */
    bool        cq_mode;        /* Printing one line at a time? */
} printQueryOpt;

/*
 * Use this to print query results
 *
 * It calls the printTable above with all the things set straight.
 *
 * widths is a persistent set of row widths (for CQ mode), or NULL to
 * compute the widths based on the current result.
 */
void		printQuery(const PGresult *result, const printQueryOpt *opt, FILE *fout, unsigned int *widths);

#ifndef __CYGWIN__
#define DEFAULT_PAGER "more"
#else
#define DEFAULT_PAGER "less"
#endif

#endif   /* PRINT_H */

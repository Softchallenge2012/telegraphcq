/*-------------------------------------------------------------------------
 *
 * parser.h
 *
 *
 *
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: parser.h,v 1.1.1.2 2003/04/17 23:02:41 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSER_H
#define PARSER_H

#include "lib/stringinfo.h"
#include "parser/parse_node.h"

extern List *parser(StringInfo str, Oid *typev, int nargs);

#endif   /* PARSER_H */

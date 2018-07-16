/*-------------------------------------------------------------------------
 *
 * parse_clause.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: parse_clause.h,v 1.10 2005/06/13 02:59:29 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef PARSE_CLAUSE_H
#define PARSE_CLAUSE_H

#include "parser/parse_node.h"

extern void transformFromClause(ParseState *pstate, List *frmList);
extern WindowClause *
transformWindowClause(ParseState *pstate,		/* @dwndwSK */
					  List *windowList);		/* @dwndwSK */
extern int setTargetTable(ParseState *pstate, RangeVar *relation,
			   bool inh, bool alsoSource);
extern bool interpretInhOption(InhOption inhOpt);
extern Node *transformWhereClause(ParseState *pstate, Node *where);
extern List *transformGroupClause(ParseState *pstate, List *grouplist,
					 List *targetlist);
extern List *transformSortClause(ParseState *pstate, List *orderlist,
					List *targetlist);
extern List *transformDistinctClause(ParseState *pstate, List *distinctlist,
						List *targetlist, List **sortClause);

extern List *addAllTargetsToSortList(List *sortlist, List *targetlist);
extern Index assignSortGroupRef(TargetEntry *tle, List *tlist);

extern Oid transformRecurseRelation(ParseState *pstate, Node * recurse, List * targetList);
extern void transformWindowExpr(ParseState *pstate, Node * fromEntry, Node * transformedEntry);


extern Node *transformEventClause(ParseState *pstate, Node *eventClause);  /* - TCQ SR */

#endif   /* PARSE_CLAUSE_H */






/*-------------------------------------------------------------------------
 *
 * nodeFuncs.h
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 * Portions Copyright (c) 1996-2002, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: nodeFuncs.h,v 1.9 2003/04/23 01:35:10 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFUNCS_H
#define NODEFUNCS_H

#include "nodes/primnodes.h"

extern bool single_node(Node *node);
extern bool var_is_outer(Var *var);
extern bool var_is_rel(Var *var);
extern Oper *replace_opid(Oper *oper);
extern ConstTree *
add_to_ConstTree(ConstTree * ct,	/* @BdcacqSK */
				 Const *c,
				 int32 queryid,
				 FunctionCachePtr fcache,
				 bool isRangeEQ);		/* @EdcacqSK */


extern List *
add_to_list(List *ct,			/* @BdcacqSK */
			Const *c,
			int32 queryid,
			FunctionCachePtr fcache);	/* @EdcacqSK */

#endif   /* NODEFUNCS_H */

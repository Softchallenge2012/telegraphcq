/*-------------------------------------------------------------------------
 *
 * eddy.h
 *
 * This file declares the functions that make up the implementation of the new
 * TelegraphCQ CQ Eddy.
 *
 * This is related to the new feature:
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: eddy.h,v 1.15 2004/02/13 01:44:04 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef EDDY_H
#define EDDY_H

extern bool 	 		 isEddyModule(Plan *p);
extern void 	 		 add_plan_to_cq_eddy(Eddy * cur, Eddy * fresh, Query *qtree, int32 queryid);
extern void 	 		 add_planitem_to_eddy(Eddy * cur, Plan *newplan, EddyItem item);
extern int				 compact_array(Plan **array, int size);
extern void 	 		 remove_projection_info(Eddy * e, int32 queryid);
extern void 	 		 add_projection_info(Eddy * e, List *tlist, int32 queryid);
extern void 	 		 remove_query(Eddy * e, int queryid);
extern int		 		 varNoGetSourceArrayIndex(Eddy * e, int varno);
extern Index             sourceGetVarNo(Plan *node);
extern void			 	 freeBindinginfo(Bindinginfo * b);
extern IndexSteM  		*strmscan_to_indexstem(StrmScan * scan);
extern Oper	   			*lookup_oper(char *opname, Oid type1, Oid type2);
extern void			 	 load_oper(Oper *o);
extern void 			 PrintMetaOperator(Eddy *cur, MetaOperator *mo);

#endif   /* EDDY_H */

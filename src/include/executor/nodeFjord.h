/*-------------------------------------------------------------------------
 *
 * nodeFjord.h
 *
 * This has the declarations common to Fjord operators
 *
 * Note that this also contains declarations of some functions that are
 * implemented in processNode.c - these, including the dispatch functions, will
 * eventually be moved to a new file called nodeFjord.c
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: nodeFjord.h,v 1.8 2005/06/13 02:59:28 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEFJORD_H
#define NODEFJORD_H

#include "nodes/execnodes.h"
#include "nodes/plannodes.h"
#include "executor/intermediateTuple.h"
#include "executor/staticplan.h"
#include "telegraphcq/dyncatalog.h"
#include "telegraphcq/telegraphcqinit.h"

/*
 *	Useful macros
 */

#define FJORD_AUTOVAR_DECLS \
	Fjord				    *fjord; \
	ExprContext			    *econtext; \
	CommonScanState		    *scanstate; \
	EState				    *estate; \
	TupleTableSlot		    *input; \
	TupleTableSlot		    *output; \
	Eddy				    *eddy; \
    IntermediateHeapTuple    iiht; \
    IntermediateHeapTuple    oiht; \
    IntermediateHeapTuple   *piiht; \
	int					operator

#define FJORD_AUTOVAR_DEFNS \
	estate	  = node->fjord.plan.state;			\
	scanstate = node->fjord.scanstate;			\
	input	  = scanstate->css_ScanTupleSlot;	\
	econtext  = scanstate->cstate.cs_ExprContext;		\
	output	  = scanstate->cstate.cs_ResultTupleSlot;	\
	fjord	  = &(node->fjord);					\
	eddy	  = (Eddy *) parent; \
	operator  = node->fjord.operator; \
    piiht      = &iiht; \
    oiht      = &(eddy->eddystate->ModuleIHT_Table[operator])

/*
 * intermediate heap tuple queue functions
 */
#define ENQUEUE_IHT_DTA(q, elt)												   \
	enqueue(q, (char*)elt, true,&intermediateHeapTuple_serializer)
#define DEQUEUE_IHT_DTA(q, ret)												   \
	dequeue(q, (char**)ret, false,	&intermediateHeapTuple_serializer)

#define ENQUEUE_IHT_PTR(q, elt)												   \
	enqueue(q, (char*) elt, true,  &IHT_pointer_serializer)
#define DEQUEUE_IHT_PTR(q, ret)												   \
	dequeue(q, (char**)ret, false, &IHT_pointer_serializer)

#define F_QUEUE(res, m, elt,_dir_,_DIR_,_typ_,_TYP_,_KND_)					   \
	  (res) = (_TYP_##_IHT_##_KND_(((Fjord *)(m))->_dir_, elt));			   \
	  if (res)																   \
	  {																		   \
	  	if (! ((Fjord *)(m))->dynCat)										   \
	  	{																	   \
	  		tcq_queues_##_typ_(((Fjord *)(m))->operator,					   \
	  						   TCQ_QUEUES_##_DIR_, TCQ_QUEUES_SUCC_##_TYP_);   \
	  	}																	   \
	  }                                                                        


/* 
 * Macros for queue operations
 *
 * r = result, m = module, e = element
 *
*/

#define FJO_ENQ_INP(r,m,e,d)    F_QUEUE((r),m,e,input,INPUT,enqueue,ENQUEUE,d)
#define FJO_DEQ_INP(r,m,e,d)    F_QUEUE((r),m,e,input,INPUT,dequeue,DEQUEUE,d)
#define FJO_ENQ_NS_OUT(r,m,e,d) F_QUEUE((r),m,e,output,OUTPUT,enqueue,ENQUEUE,d)
#define FJO_DEQ_OUT(r,m,e,d)    F_QUEUE((r),m,e,output,OUTPUT,dequeue,DEQUEUE,d)

#define FJO_ENQ_OUT(r,m,e,d)								 \
	{														 \
		if (UseStaticPlan)									 \
		{													 \
			int driver = ((Fjord *)m)->driver;				 \
		    Fjord *out;										 \
															 \
		    if (driver == -1)								 \
			{												 \
				driver = StaticDriver;						 \
			}												 \
															 \
			out = ((Fjord *)m)->outputs[driver];			 \
															 \
			F_QUEUE(r,out,e,input,INPUT,enqueue,ENQUEUE,d);  \
	    }													 \
		else												 \
		{													 \
			FJO_ENQ_NS_OUT(r,m,e,d);						 \
		}													 \
	}


#define FJORD_ENQ_INP_DTA(r,m,e)       FJO_ENQ_INP(r,m,e,DTA)
#define FJORD_DEQ_INP_DTA(r,m,e)       FJO_DEQ_INP(r,m,e,DTA)
#define FJORD_ENQ_OUT_DTA(r,m,e)       FJO_ENQ_OUT(r,m,e,DTA)
#define FJORD_DEQ_OUT_DTA(r,m,e)       FJO_DEQ_OUT(r,m,e,DTA)
								       
#define FJORD_ENQ_INP_PTR(r,m,e)       FJO_ENQ_INP(r,m,e,PTR)
#define FJORD_DEQ_INP_PTR(r,m,e)       FJO_DEQ_INP(r,m,e,PTR)
#define FJORD_ENQ_OUT_PTR(r,m,e)       FJO_ENQ_OUT(r,m,e,PTR)
#define FJORD_DEQ_OUT_PTR(r,m,e)       FJO_DEQ_OUT(r,m,e,PTR)

/*
 * prototypes from functions in processNode.c
 */

extern bool ProcessInitNode(Plan *node, EState *estate, Plan *parent);
extern bool ProcessRemoveQuery(Plan *node, int32 queryid, Plan *parent);
extern bool ProcessEndNode(Plan *node, Plan *parent);
extern void ProcessNextNode(Plan *node, Plan *parent);
extern void ProcessPrintStats(Plan *node, Plan *parent);

extern bool FjordAddQuery(Plan *node, int32 queryid, Plan *parent);
extern bool FjordRemoveQuery(Fjord * f, int queryid);
extern void FjordInitialize(Fjord * f, EState *estate);
extern void FjordEnd(Fjord * f, Eddy * eddy);

#endif   /* NODEFJORD_H */

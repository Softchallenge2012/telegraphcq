/*-------------------------------------------------------------------------
 *
 * telegraphcq.h
 *
 * compile time parameters that effect telegraphCQ
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: telegraphcqinit.h,v 1.24 2005/06/13 02:59:29 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef TELEGRAPHCQINIT_H
#define TELEGRAPHCQINIT_H

/* #define NUMRESULTQUEUES  32  */
/*#define NUMRESULTQUEUES 128 */
/*#define RESULTQSIZE		 (1024*500) */
#define RESULTQSIZE		 (1024*16) 
#define RECURSIVERESULTQSIZE   (1024*2048)
#define WCHPROBEQSIZE          (1024*1024)
#define CONTROLQSIZE	 (1024*30)
#define WRAPCONTROLQSIZE (1024*1024)  /* Pull wrappers send probe tuples 
					 through this q so it should be 
					 large */
#define PLANQSIZE		 (1024*100)
#define TCQMAXQUERIES	 128
/* #define TCQMAXOPERATORS  32 */
#define TCQMAXOPERATORS  96
#define TCQMAXSOURCES	 64
#define FJORDQUEUESIZE	 4096
#define TCQTSATTRNAME	 "tcqtime"
#define TCQTSATTRNOP	 -1
#define EDDYINPUTCHECKINTERVAL 200		/* in eddy loop iterations */
#define RANGEBYDEFAULT  "100 years"
#define SLIDEBYDEFAULT  "0 seconds"

#define MAX_PARTITIONS 100		/* Maximum number of partitions allowed
								 * for the disk based symmetric hash join
								 * - Amol */

#endif   /* TELEGRAPHCQINIT_H */

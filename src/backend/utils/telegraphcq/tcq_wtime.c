/*-------------------------------------------------------------------------
 *
 * csv.c
 *
 *	  Built-in CSV wrapper
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/telegraphcq/tcq_wtime.c,v 1.4 2005/06/13 02:59:28 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "postgres.h"
#include "fmgr.h"
#include "miscadmin.h"
PG_FUNCTION_INFO_V1(tcq_wtime);
PG_FUNCTION_INFO_V1(tcq_realtime);


Datum
			tcq_wtime(PG_FUNCTION_ARGS);
Datum
			tcq_realtime(PG_FUNCTION_ARGS);


Datum
tcq_wtime(PG_FUNCTION_ARGS)
{
  if(tcqwtime == NULL)
    {
      elog(ERROR, "tcq_wtime return value variable tcqwtime not set");
    }
    PG_RETURN_POINTER(tcqwtime);
}

/* Function that returns the wallclock time, for comparison with what wtime(*)
 * returns. */
Datum
tcq_realtime(PG_FUNCTION_ARGS)
{
    Datum result = DirectFunctionCall1(text_timestamp,
            DirectFunctionCall1(timeofday,PointerGetDatum(NULL)));

    PG_RETURN_DATUM(result);
}


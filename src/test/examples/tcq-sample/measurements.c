/*-------------------------------------------------------------------------
 *
 * measurements.c
 *
 *    Obsolete antiquated measurements wrapper - DO NOT USE
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
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
#include "telegraphcq/userwrapper.h"

#include <time.h>
#include "utils/timestamp.h"
#include "utils/datetime.h"
#define BUFSIZE 1000




/* user defined wrapper functions must be new style postgres functions.
   Such functions must call the following macros.  Failure to include
   these will result in crashes when the functions are invoked by the
   wrapper clearinghouse.
*/

PG_FUNCTION_INFO_V1(measurements_init);
PG_FUNCTION_INFO_V1(measurements_next);
PG_FUNCTION_INFO_V1(measurements_done);
Datum
			measurements_init(PG_FUNCTION_ARGS);
Datum
			measurements_next(PG_FUNCTION_ARGS);
Datum
			measurements_done(PG_FUNCTION_ARGS);


/*
 * this is the private state for the user defined function.
 * An instance will be allocated by measurements_init and freed
 * by measurements_done.
 */
struct measurementsstate
{
	char		tuple[sizeof(Timestamp) + sizeof(int) + sizeof(float)];

	char		buf[BUFSIZE + 1];
	int			readidx;
	int			writeidx;
	int			tuplecount;

	/* data items we collected for this reading */
	Datum		tcqtime;
	int			stationNo;
	float		speed;
};

Datum
measurements_init(PG_FUNCTION_ARGS)
{
	/* allocate private state */
	struct measurementsstate *udfstate = NULL;
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);


	ws->udfstate = palloc(sizeof(struct measurementsstate));
	ws->hasTuple = false;

	udfstate = (struct measurementsstate *) ws->udfstate;
	memset(udfstate->buf, 'T', BUFSIZE);		/* debugging code */
	udfstate->buf[BUFSIZE] = '\0';		/* make sure the buffer is NULL
										 * terminated, so string ops are
										 * safe */
	udfstate->writeidx = 0;
	udfstate->readidx = 0;
	udfstate->tuplecount = 0;
	ws->mode = MODE_NEXT;

	PG_RETURN_BOOL(TRUE);
}

Datum
measurements_next(PG_FUNCTION_ARGS)
{
	int			bread = 0;
	char		datestr[45];
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);
	struct measurementsstate *udfstate = (struct measurementsstate *) ws->udfstate;

	if (ws->hasTuple == TRUE)
		ws->hasTuple = FALSE;

	if (ws->connectionReady == TRUE)
	{

		if ((udfstate->writeidx > (BUFSIZE - 50)) &&
			((udfstate->writeidx - udfstate->readidx) < 75))
		{
			memcpy(udfstate->buf,
				   (udfstate->buf + udfstate->readidx),
				   (udfstate->writeidx - udfstate->readidx));


			udfstate->writeidx = (udfstate->writeidx - udfstate->readidx);
			udfstate->readidx = 0;
			memset(udfstate->buf + udfstate->writeidx, 'T',
				   BUFSIZE - udfstate->writeidx);
		}

		if (udfstate->writeidx != BUFSIZE)
		{

			bread = read(ws->fd,
						 (udfstate->buf + udfstate->writeidx),
						 BUFSIZE - udfstate->writeidx);
			if (bread == 0
				&& ws->hasBufferedData == FALSE)
			{
				elog(DEBUG1,
					 "setting mode to done because 0 bytes read");

				ws->mode = MODE_DONE;

				PG_RETURN_BOOL(TRUE);
			}
			else if (bread < 0)
			{
				if (errno == EAGAIN)
					PG_RETURN_BOOL(TRUE);
				else
					PG_RETURN_BOOL(FALSE);
			}


			udfstate->writeidx += bread;
			if (bread > 0)
				ws->hasBufferedData = TRUE;
		}
	}



	if (ws->hasBufferedData == TRUE)
	{
		int			itemsread = 0;

		char	   *tupleend = strchr(udfstate->buf + udfstate->readidx, '\n');
		int			tuplesize = (tupleend - (udfstate->buf + udfstate->readidx));

		char	   *dateend = strchr(udfstate->buf + udfstate->readidx, ',');


		if (dateend != NULL && tupleend != NULL &&
			(udfstate->readidx + tuplesize <= udfstate->writeidx))
		{
			int			datesize = (dateend - (udfstate->buf + udfstate->readidx));

			strncpy(datestr,
					udfstate->buf + udfstate->readidx,
					datesize);
			datestr[datesize] = '\0';
			itemsread++;
			itemsread += sscanf(dateend,
					 ",%d,%f\n", &udfstate->stationNo, &udfstate->speed);

		}

		if (itemsread == 3)
		{
			Datum		ret;

			udfstate->tuplecount++;
			ret = DirectFunctionCall3(timestamp_in,
									  CStringGetDatum(datestr),
									  ObjectIdGetDatum(InvalidOid),
									  Int32GetDatum(-1));
			memcpy(udfstate->tuple,
				   DatumGetPointer(ret),
				   sizeof(Timestamp));
			memcpy((udfstate->tuple + sizeof(Timestamp)),
				   &udfstate->speed,
				   sizeof(float));
			pfree(DatumGetPointer(ret));

			ws->returnTuple[0] = PointerGetDatum(udfstate->tuple);
			ws->returnTuple[1] = Int32GetDatum(udfstate->stationNo);
			ws->returnTuple[2] = PointerGetDatum((udfstate->tuple) + sizeof(Timestamp));
			ws->returnTupleIsNull[0] = ' ';
			ws->returnTupleIsNull[1] = ' ';
			ws->returnTupleIsNull[2] = ' ';
			ws->hasTuple = true;
			udfstate->readidx += tuplesize;
			while (udfstate->buf[udfstate->readidx] == '\n')
				udfstate->readidx++;
		}
		else
		{
/*		  elog(DEBUG1, "looked for a tuple, but sscanf returned %d instead of 3", itemsread); */
		}
	}


	if (udfstate->readidx == udfstate->writeidx)
		ws->hasBufferedData = FALSE;
	PG_RETURN_BOOL(TRUE);
}



Datum
measurements_done(PG_FUNCTION_ARGS)
{
	/* free the user allocated state */
	/* declare we are finished */
	/* return success */
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	struct measurementsstate *udfstate = (struct measurementsstate *) s->udfstate;

	elog(DEBUG1, "wrapper done..... processed %d tuples", udfstate->tuplecount);
	pfree(s->udfstate);
	s->udfstate = NULL;
	s->mode = MODE_END;
	PG_RETURN_BOOL(TRUE);
}

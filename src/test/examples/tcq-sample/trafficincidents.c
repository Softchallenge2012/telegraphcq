/*-------------------------------------------------------------------------
 *
 * tcqsanity.c
 *
 *    Obsolete antiquated traffic incidents wrapper
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

#include <string.h>				/* for strlen. */
#include <time.h>
#include "utils/builtins.h"		/* added for textin */
#include "utils/timestamp.h"
#include "utils/datetime.h"
#define BUFSIZE 4000			/* be generous. :) */




/* user defined wrapper functions must be new style postgres functions.
   Such functions must call the following macros.  Failure to include
   these will result in crashes when the functions are invoked by the
   wrapper clearinghouse.
*/

PG_FUNCTION_INFO_V1(trafficincidents_init);
PG_FUNCTION_INFO_V1(trafficincidents_next);
PG_FUNCTION_INFO_V1(trafficincidents_done);
Datum
			trafficincidents_init(PG_FUNCTION_ARGS);
Datum
			trafficincidents_next(PG_FUNCTION_ARGS);
Datum
			trafficincidents_done(PG_FUNCTION_ARGS);
void
			getvarfld(char *target, char *source);


/**
 *	How tuple is delivered to TelegraphCQ from perl script:
 *
 *	|-------|-|----------|-|-------|-|--------|-|--------|-|----|-|--------|-|-----------|--|
 *	|tcqtime|,|incidentID|,|locsize|,|location|,|citysize|,|city|,|descsize|,|description|\n|
 *	|-------|-|----------|-|-------|-|--------|-|--------|-|----|-|--------|-|-----------|--|
 *	tcqtime		=  28 bytes (i think)
 *	,			=	7 bytes total (4,6 used to '\0'; 1,2,3,5,7 are needed.)
 *	incidentID	=	4 bytes (int)
 *	locsize		=	4 bytes (ascii number)
 *	location	=  atoi(locsize) + 1 <= 1000 bytes	(+1 == '\0')
 *	citysize	=	4 bytes (ascii number)
 *	city		=  atoi(citysize) +1 <= 1000 bytes
 *	descsize	=	4 bytes (ascii number)
 *	description =  atoi(descsize) + 1 <= 1000 bytes
 *	\n			=	1 byte
 *
 * MAX TUPLE SIZE: 3052.
 *
 */


/*
 * this is the private state for the user defined function.
 * An instance will be allocated by trafficincidents_init and freed
 * by trafficincidents_done.
 *
 *
 */
struct trafficincidentsstate
{
	char		tuple[sizeof(Timestamp) + sizeof(int) + 3020];	/* Should be 3052 bytes. */

	char		buf[BUFSIZE + 1];
	int			readidx;
	int			writeidx;
	int			tuplecount;

	/* data items we collected for this reading */
	Datum		tcqtime;
	int			incidentID;
	int			locsize;
	int			citysize;
	int			descsize;
};


Datum
trafficincidents_init(PG_FUNCTION_ARGS)
{
	/* allocate private state */
	struct trafficincidentsstate *udfstate = NULL;
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);


	ws->udfstate = palloc(sizeof(struct trafficincidentsstate));
	ws->hasTuple = false;

	udfstate = (struct trafficincidentsstate *) ws->udfstate;
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
trafficincidents_next(PG_FUNCTION_ARGS)
{
	int			bread = 0;
	char		datestr[45];
	char		locbuf[1000];	/* location buffer */
	char		citybuf[1000];	/* city buffer */
	char		descbuf[1000];	/* description buffer */


	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);
	struct trafficincidentsstate *udfstate = (struct trafficincidentsstate *) ws->udfstate;

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
		int			fieldsread = 0;

		char	   *tupleend = strchr(udfstate->buf + udfstate->readidx, '\n');
		int			tuplesize = (tupleend - (udfstate->buf + udfstate->readidx));

		char	   *dateend = strchr(udfstate->buf + udfstate->readidx, ',');

		/* dateend contains string after tcqtime.  */


		if (dateend != NULL && tupleend != NULL &&
			(udfstate->readidx + tuplesize <= udfstate->writeidx))
		{
			int			datesize = (dateend - (udfstate->buf + udfstate->readidx));
			int			i;

			strncpy(datestr,
					udfstate->buf + udfstate->readidx,
					datesize);
			datestr[datesize] = '\0';
			itemsread++;
			/* datestr is done. Now get 4 remaining tuples. */

			/* Get incidentID */
			itemsread += sscanf(dateend, ",%d,", &udfstate->incidentID);

			i = 5;				/* ,#### is 5 chars. -- i now points to
								 * locsize. */

			/**
				Get location
			*/
			fieldsread += sscanf(dateend + i, ",%d", &udfstate->locsize);
			/* null char the end of locsize */
			i += 6;				/* ,#### and nullterm -- i now points to
								 * end of location. */
			dateend[udfstate->locsize + i] = '\0';
			memset(locbuf, 0, 1000);
			/* Copy location string to LOCBUF. */
			strncpy(locbuf, dateend + i, udfstate->locsize + 1);
			locbuf[udfstate->locsize] = '\0';	/* comma became '\0' */
			itemsread += 1;

			/**
			   Get city.
			**/
			i += 1;				/* i -- now points to beginning of
								 * citysize.  */
			fieldsread += sscanf(dateend + udfstate->locsize + i, "%d", &udfstate->citysize);
			i += 5;				/* */
			dateend[udfstate->locsize + udfstate->citysize + i] = '\0';
			strncpy(citybuf, dateend + (11 + udfstate->locsize + 6), udfstate->citysize + 1);
			citybuf[udfstate->citysize] = '\0';
			itemsread += 1;

			/**
			   now do description
			**/
			fieldsread += sscanf(dateend + (18 + udfstate->locsize + udfstate->citysize), "%d", &udfstate->descsize);
			dateend[18 + 5 + udfstate->locsize + udfstate->citysize + udfstate->descsize] = '\0';

			strncpy(descbuf, dateend + (17 + udfstate->locsize + udfstate->citysize + 6), udfstate->descsize - 1);
			descbuf[udfstate->descsize - 1] = '\0';		/* -1 because the data
														 * being delivered here
														 * contains a carriage
														 * return \012 probably.
														 * I null terminate
														 * descbuf at this
														 * strategic place. */
			itemsread += 1;

			elog(DEBUG1, "YAY!! datestr:%s, dateend=%s", datestr, dateend);
			elog(DEBUG1, "YAY!! locbuf:%s locsize:%d", locbuf, udfstate->locsize);
			elog(DEBUG1, "OUTPUT!! readix: %d writeidX: %d fieldsread: %d", udfstate->readidx, udfstate->writeidx, fieldsread);
			elog(DEBUG1, "SECOND HALF OF DATEEND: %s", dateend + 12 + udfstate->locsize);
			elog(DEBUG1, "Third HALF %s", dateend + 18 + udfstate->locsize + udfstate->citysize);
			elog(DEBUG1, "citysize: %d  citybuf: %s", udfstate->citysize, citybuf);
			elog(DEBUG1, "descsize: %d desc:%s", udfstate->descsize, descbuf);

		}

		if (itemsread == 5 && fieldsread == 3)
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
			pfree(DatumGetPointer(ret));

			/* copy location */
			getvarfld(udfstate->tuple + sizeof(Timestamp), locbuf);

			/* copy city */
			getvarfld(udfstate->tuple + sizeof(Timestamp) + udfstate->locsize + 1 + 4, citybuf);

			/* copy description */
			elog(DEBUG1, "After copying city ... descbuflen: %d, tupleptr:%d", strlen(descbuf), sizeof(Timestamp) + udfstate->locsize + 1 + udfstate->citysize + 1);

			getvarfld(udfstate->tuple + (sizeof(Timestamp) + udfstate->locsize + 1 + udfstate->citysize + 1 + 8), descbuf);
			elog(DEBUG1, "After description...!");


			ws->returnTuple[0] = PointerGetDatum(udfstate->tuple);		/* Timestamp */
			ws->returnTuple[1] = Int32GetDatum(udfstate->incidentID);	/* incidentID */

			/*
			 * ws->returnTuple[2] = Int32GetDatum(udfstate->locsize); //
			 * locationsize
			 */
			ws->returnTuple[2] = PointerGetDatum(udfstate->tuple + sizeof(Timestamp));	/* location */

			/*
			 * ws->returnTuple[4] = Int32GetDatum(udfstate->citysize); //
			 * citysize
			 */
			ws->returnTuple[3] = PointerGetDatum(udfstate->tuple + sizeof(Timestamp) + udfstate->locsize + 1 + 4);		/* city */

			/*
			 * ws->returnTuple[6] = Int32GetDatum(udfstate->descsize); //
			 * description size
			 */
			ws->returnTuple[4] = PointerGetDatum(udfstate->tuple + sizeof(Timestamp) + udfstate->locsize + 1 + udfstate->citysize + 1 + 8);		/* description */

			ws->returnTupleIsNull[0] = ' ';
			ws->returnTupleIsNull[1] = ' ';
			ws->returnTupleIsNull[2] = ' ';
			ws->returnTupleIsNull[3] = ' ';
			ws->returnTupleIsNull[4] = ' ';
			/* ws->returnTupleIsNull[5]=' '; */
			/* ws->returnTupleIsNull[6]=' '; */
			/* ws->returnTupleIsNull[7]=' '; */


			ws->hasTuple = true;
			udfstate->readidx += 2;		/* Remember the carriage return
										 * that was passed in? \012? Add 2
										 * here to skip over them inorder
										 * to read the next tuple. */
			/* elog(DEBUG1, "READIDX: %d", udfstate->readidx); */
			udfstate->readidx += tuplesize;

			while (udfstate->buf[udfstate->readidx] == '\n')
				udfstate->readidx++;


		}
		else
		{
			elog(DEBUG1, "looked for a tuple, but sscanf returned %d instead of 3, dateend: %s, tupleend: %s, read: %d, write: %d, tuplesize: %d", itemsread, dateend, tupleend, udfstate->readidx, udfstate->writeidx, tuplesize);

		}
	}							/* end hasBufferedData = TRUE */


	if (udfstate->readidx == udfstate->writeidx)
		ws->hasBufferedData = FALSE;
	PG_RETURN_BOOL(TRUE);
}



Datum
trafficincidents_done(PG_FUNCTION_ARGS)
{
	/* free the user allocated state */
	/* declare we are finished */
	/* return success */
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	struct trafficincidentsstate *udfstate = (struct trafficincidentsstate *) s->udfstate;

	elog(DEBUG1,
		 "wrapper done..... processed %d tuples", udfstate->tuplecount);

	pfree(s->udfstate);
	s->udfstate = NULL;
	s->mode = MODE_END;
	PG_RETURN_BOOL(TRUE);
}


/**
 * Function: Copies SRC to TGT.
 *
 * NOTE: Does not take into account 4 bytes, the int in varlena.
 */
void
getvarfld(char *tgt, char *src)
{
	text	   *txt;
	int			len = strlen(src) + VARHDRSZ;

	txt = (text *) tgt;

	VARATT_SIZEP(txt) = len;
	strncpy(VARDATA(txt), src, len - VARHDRSZ);
}

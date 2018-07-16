#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#include "postgres.h"
#include "fmgr.h"
#include "telegraphcq/userwrapper.h"

#include <time.h>
#include "utils/timestamp.h"
#include "utils/datetime.h"

/* This user defined wrapper writes data to a stream which has two integers
   and a timestamp.  The integers are retrieved from the network, and the
   timestamp value is set by the wrapper itself according to the time the
   tuple is received by the clearinghouse.

   DDL to create an archived stream named teststream and to define the
   wrapper functions named teststream_init, teststream_next, and
   teststream_done can be found in the file teststream.sql in this
   directory.
*/




/* user defined wrapper functions must be new style postgres functions.
   Such functions must call the following macros.  Failure to include
   these will result in crashes when the functions are invoked by the
   wrapper clearinghouse.
*/

PG_FUNCTION_INFO_V1(teststream1_init);
PG_FUNCTION_INFO_V1(teststream1_next);
PG_FUNCTION_INFO_V1(teststream1_done);
Datum
			teststream1_init(PG_FUNCTION_ARGS);
Datum
			teststream1_next(PG_FUNCTION_ARGS);
Datum
			teststream1_done(PG_FUNCTION_ARGS);


/*
 * this is the private state for the user defined function.
 * An instance will be allocated by teststream_init and freed
 * by teststream_done.
 */
struct teststreamstate
{
	int			idx;			/* index into the buffer */
	char		buf[sizeof(long) * 2 + sizeof(Timestamp)];		/* buffer for data read
																 * from the network */
	int			tz;
};

Datum
teststream1_init(PG_FUNCTION_ARGS)
{
	/* allocate private state */
	/* set the index to the beginning of the buffer */
	/* declare success. */

	time_t		timeval;
	struct tm	structtm;
	struct teststreamstate *udfstate = NULL;
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

	ws->udfstate = palloc(sizeof(struct teststreamstate));
	ws->hasTuple = false;
	udfstate = (struct teststreamstate *) ws->udfstate;
	udfstate->idx = 0;
	time(&timeval);
	localtime_r(&timeval, &structtm);
	udfstate->tz = DetermineLocalTimeZone(&structtm);
	ws->mode = MODE_NEXT;

	PG_RETURN_BOOL(TRUE);
}

Datum
teststream1_next(PG_FUNCTION_ARGS)
{
	int			nread = -2;
	int			tupsize = 2 * sizeof(long);
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);
	struct teststreamstate *udfstate = (struct teststreamstate *) ws->udfstate;

	if (ws->hasTuple == true)
	{
		ws->hasTuple = false;
		udfstate->idx = 0;
	}
	nread = read(ws->fd, (udfstate->buf + udfstate->idx), (tupsize - udfstate->idx));
	if (nread == 0)
	{
		/* the stream has been closed when poll indicates there is	*/
		/* information to be read, but there is none. */
		ws->mode = MODE_DONE;
	}
	if (nread < 0)
	{
		/* if read returns failure... declare error to caller.	The  */
		/* clearinghouse will shutdown the wrapper. In fact, the next */
		if (errno == EAGAIN)
			PG_RETURN_BOOL(TRUE);
		/* UDF call will be to the done function. */
		PG_RETURN_BOOL(FALSE);
	}
	udfstate->idx += nread;


	if (udfstate->idx == tupsize)
	{
		time_t		ticks;
		struct tm	structtm;
		Timestamp	timestamp;

		/* we have a tuple.   */
		/* it consists of two longs and a timestamp which are non-pointer  */
		/* types so they */
		/* are stored directly into the resultTuple array by the *GetDatum */
		/* functions below.  The timestamp generated by the wrapper is not	*/

		/*
		 * stored directly in the tuple.  instead a pointer to the
		 * timestamp
		 */
		/* (stored in the udfstate) is placed in the datums array. */

		time(&ticks);
		gmtime_r(&ticks, &structtm);
		structtm.tm_year += 1900;
		structtm.tm_mon++;
		tm2timestamp(&structtm, 0, &udfstate->tz, &timestamp);
		memcpy((udfstate->buf + 2 * sizeof(long)),
			   &timestamp,
			   sizeof(Timestamp));

		ws->returnTuple[0] = UInt32GetDatum(*(long *) (udfstate->buf));
		ws->returnTuple[1] = UInt32GetDatum(*(long *) (udfstate->buf + sizeof(long)));
		ws->returnTuple[2] = PointerGetDatum((udfstate->buf + 2 * sizeof(long)));
		/* null values are represented by non-space characters in the */
		/* returnTupleIsNull array.  Since we happen to know that this	*/
		/* stream never includes NULL values, these entries are always set */
		/* to non-null. */
		ws->returnTupleIsNull[0] = ' ';
		ws->returnTupleIsNull[1] = ' ';
		ws->returnTupleIsNull[2] = ' ';

		/* we have a tuple. */
		ws->hasTuple = true;
	}
	else
		ws->hasTuple = false;
	/* function returns successfully */
	PG_RETURN_BOOL(TRUE);
}

Datum
teststream1_done(PG_FUNCTION_ARGS)
{
	/* free the user allocated state */
	/* declare we are finished */
	/* return success */
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);

	pfree(s->udfstate);
	s->udfstate = NULL;
	s->mode = MODE_END;
	PG_RETURN_BOOL(TRUE);
}

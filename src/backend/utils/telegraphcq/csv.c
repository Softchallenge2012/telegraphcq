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
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/telegraphcq/csv.c,v 1.17 2005/07/27 20:04:12 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

/*#define LOG_TUPLES*/
    /* Define to log all tuples received to /tmp/csv_logfile */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "postgres.h"
#include "fmgr.h"
#include "telegraphcq/userwrapper.h"
#include "parser/parse_wrapper.h"
#include "catalog/pg_type.h"
#include <time.h>
#include "utils/timestamp.h"
#include "utils/datetime.h"
#include "utils/builtins.h"

/* How often we send punctuation tuples if we aren't receiving data and we're
 * generating timestamps. */
#define CSV_PUNCT_INTERVAL_SEC 1

/* The CSV wrapper doesn't like lines that are longer than the buffer.  This
 * causes a problem with large varchars, so we keep the buffer size very 
 * large. */
#define BUFSIZE 100000
/*#define BINDINGBUFFERSIZE 2000*/
PG_FUNCTION_INFO_V1(csv_init);
PG_FUNCTION_INFO_V1(csv_next);
PG_FUNCTION_INFO_V1(csv_done);

#ifdef LOG_TUPLES
static FILE * g_csv_logfile = NULL;
static wrapperstate *g_csv_logfile_owner = NULL;
#endif /* LOG_TUPLES */

Datum
			csv_init(PG_FUNCTION_ARGS);
Datum
			csv_next(PG_FUNCTION_ARGS);
Datum
			csv_done(PG_FUNCTION_ARGS);


/* Our knowledge of whether we're generating timestamps for the current stream.
 */
#if 0
enum csv_ts_gen_state {
    DONT_KNOW = 0,      /* We don't know, because we haven't seen any tuples.*/
    GENERATING,         /* We're generating the timestamps for this stream. */
    NOT_GENERATING      /* We're not generating timestamps, because the
                           stream comes already timestamped. */
};
#endif /* 0 */

/*
 * this is the private state for the user defined function.
 * An instance will be allocated by csv_init and freed
 * by csv_done.
 */
struct csvstate
{
	char		buf[BUFSIZE + 1];

	int			readidx;
	int			writeidx;
	int			tuplecount;
	int			numrescans;
	char	  **column_delimiters;
	char	   *default_delimiters;
	char	   *error_indicator;
	char	   *done_indicator;
	int			done_indicator_size;
	int			error_indicator_size;

};

/* Prototypes of internal functions. */
void compact_buffer(struct csvstate *udfstate);
void gen_timestamp_field(wrapperstate *ws);
void gen_punct_tuple(wrapperstate *ws);
bool tuple_has_timestamp(wrapperstate *ws);
void ask_for_punct_timeout(wrapperstate *ws);


Datum
csv_init(PG_FUNCTION_ARGS)
{
	int			i = 0;
	char		keystr[100];
	key_value_entry *entry = NULL;

	/* allocate private state */

	struct csvstate *udfstate = NULL;
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

	/* check that tcqtime is the last attribute of the stream */

	if (ws->tdesc->attrs[ws->tdesc->natts - 1]->atttypid != TIMESTAMPOID)
	{
		ws->error_message = "The last column of any stream which uses the CSV wrapper must have datatype TIMESTAMP";
		PG_RETURN_BOOL(FALSE);
	}

	ws->udfstate = palloc(sizeof(struct csvstate));
	ws->hasTuple = false;

	udfstate = (struct csvstate *) ws->udfstate;
	memset(udfstate->buf, 'T', BUFSIZE);		/* debugging code */
	udfstate->buf[BUFSIZE] = '\0';		/* make sure the buffer is NULL
										 * terminated, so string ops are
										 * safe */
	udfstate->writeidx = 0;
	udfstate->readidx = 0;
	udfstate->tuplecount = 0;
	udfstate->numrescans = 0;
	udfstate->column_delimiters = 0;
	udfstate->column_delimiters = palloc(sizeof(char *) * ws->tdesc->natts);
    ws->tsState = DONT_KNOW;

	for (i = 0; i < ws->tdesc->natts; i++)
	{
		sprintf(keystr, "DELIMITERS%d", i);
		entry = get_entry(keystr, ws->wrapper_info);
		if (entry)
			udfstate->column_delimiters[i] = entry->value;
		else
			udfstate->column_delimiters[i] = NULL;

	}

	entry = get_entry("DEFAULT_DELIMITERS", ws->wrapper_info);
	if (entry)
		udfstate->default_delimiters = entry->value;
	else
		udfstate->default_delimiters = ",";

	entry = get_entry("error_indicator", ws->wrapper_info);
	if (entry)
		udfstate->error_indicator = entry->value;
	else
		udfstate->error_indicator = "ERROR MESSAGE:";
	udfstate->error_indicator_size = strlen(udfstate->error_indicator);
	entry = get_entry("done_indicator", ws->wrapper_info);
	if (entry)
		udfstate->done_indicator = entry->value;
	else
		udfstate->done_indicator = "DONE WITH PROBE";
	udfstate->done_indicator_size = strlen(udfstate->done_indicator);

	if (ws->fd < 0)
	{
		ws->fd = establishConnection(ws->wrapper_info);
		ws->mode = MODE_RESCAN;
	}
	else
		ws->mode = MODE_NEXT;

#ifdef LOG_TUPLES
    /* Create a logfile for the tuples, if one does not already exist. */
    if (NULL != g_csv_logfile) {
        elog(LOG, "CSV wrapper at %p NOT logging tuples", ws);
    } else {
        /* Only the first stream connected gets logged. */
        elog(LOG, "CSV wrapper at %p logging tuples", ws);
        g_csv_logfile = fopen("/tmp/csvwrap.log", "w");
        g_csv_logfile_owner = ws;
    }
#endif /* LOG_TUPLES */


	if (ws->fd < 0)
	{
		ws->error_message = "unable to open a connection to the wrapper server";
		PG_RETURN_BOOL(FALSE);
	}
	else
		PG_RETURN_BOOL(TRUE);
}

Datum
csv_next(PG_FUNCTION_ARGS)
{
	int			bread = 0;
	bool nextcol=false;
	bool wasNull=false;
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);
	struct csvstate *udfstate = (struct csvstate *) ws->udfstate;

    /* SPECIAL CASE: The timeout we asked for the last time we sent a tuple has
     * expired, indicating that we should send a punctuation tuple. */
    if (ws->timeoutExpired == TRUE) {
        /*elog(LOG, "CSV: Generating a punctuation tuple.");*/
        gen_punct_tuple(ws);
        ask_for_punct_timeout(ws);

        PG_RETURN_BOOL(TRUE);
    } 
    /* END SPECIAL CASE */


	if (ws->connectionReady == TRUE)	/* wch indicates that the
										 * connection has data */
	{
		/* compact buffer */
		if ((udfstate->writeidx > (BUFSIZE - 50)))
		{
            compact_buffer(udfstate);
		}

		if (udfstate->writeidx != BUFSIZE)
		{
			bread = read(ws->fd,
						 (udfstate->buf + udfstate->writeidx),
						 BUFSIZE - udfstate->writeidx);

            /*
            elog(LOG, "csv: Read %d of %d bytes.", bread, 
                    BUFSIZE - udfstate->writeidx);
                    */

			if (bread == 0
				&& ws->hasBufferedData == FALSE)
			{
				elog(NOTICE,
					 "(csv) setting mode to done because 0 bytes read");

				ws->mode = MODE_DONE;

				PG_RETURN_BOOL(TRUE);
			}
			else if (bread < 0)
			{
				if (errno != EAGAIN)
				{
					ws->error_message = "unrecoverable error on wrapper  network socket";
					PG_RETURN_BOOL(FALSE);
				}

			}

			if (bread > 0)
			{
				udfstate->writeidx += bread;
				ws->hasBufferedData = TRUE;
			}

		}
	}

	/* we have buffered data, check for a tuple */
	if (ws->hasBufferedData == TRUE)
	{
		char	   *delimiters = NULL;

		char	   *tupleend = strchr(udfstate->buf + udfstate->readidx, '\n');
		char	   *tuplebegin = udfstate->buf + udfstate->readidx;

		int			tuplesize = (tupleend - (udfstate->buf + udfstate->readidx));

		if (tupleend != NULL &&
			(udfstate->readidx + tuplesize <= udfstate->writeidx))
		{
			/*
			 * got a tuple, now process it column by column according to
			 * the delimiters
			 */
			int			col = 0;
			int			span_size = 0;

			char	   *colbegin = udfstate->buf + udfstate->readidx;
			char	   *colend = NULL;
			char	   *filter = NULL;
			char	   *bindingval = NULL;



#ifdef LOG_TUPLES
            char *tuplecopy = palloc(tuplesize + 1);
            strncpy(tuplecopy, tuplebegin, tuplesize);
            tuplecopy[tuplesize] = '\0';
#endif /* LOG_TUPLES */

			*tupleend = '\0';	/* replace the delimiter at the end of the
								 * tuple, s.t. the tuple is a null
								 * terminated string */

			if (strncasecmp(tuplebegin,
							udfstate->error_indicator,
							udfstate->error_indicator_size) == 0)
/* is this an error message? */
			{
				ws->error_message = tuplebegin;
				udfstate->readidx += tuplesize + 1;
				PG_RETURN_BOOL(FALSE);
			}
			if (strncasecmp(tuplebegin,
							udfstate->done_indicator,
							udfstate->done_indicator_size) == 0)
/* is this an end of probe? */
			{
				udfstate->readidx += tuplesize + 1;
				ws->doneWithProbe = true;
				PG_RETURN_BOOL(TRUE);
			}

			while (colbegin < tupleend)
			{
				/*
				 * tuples must echo their bindings in the result tuple
				 */

			  /* truncate long input lines */
			  if(col >= ws->returnTupleLength)
			    {
			      elog(LOG, "csv wrapper truncating long input line");
			      break;
			    }
				delimiters = udfstate->column_delimiters[col];
				if (delimiters == NULL)
					delimiters = udfstate->default_delimiters;

				filter = NULL;
				nextcol=false;
				while ((span_size = strcspn(colbegin, delimiters)) == 0)
				{
				  ws->returnTuple[col] = PointerGetDatum(NULL);
				  ws->returnTupleIsNull[col] = 'n';
				  col++;
				  colbegin += 1;
				  nextcol=true;
				  break;
				}
				if(nextcol) continue;
				if (colbegin == tupleend)
					break;
				if (span_size <= 0)
					elog(FATAL, "spansize 0");

				colend = colbegin + span_size;

				if (colend == NULL)
				  colend = tupleend;
				*colend = '\0';
				while ((filter = strchr(colbegin, '\r')))
					*filter = ' ';

				bindingval = getBindingValueByStreamCol(ws, col + 1);
				if (bindingval &&
					strcasecmp(colbegin, bindingval) != 0)
				{
					ws->returnTuple[col] = DatumFromTypeInstance(ws->aim,
																 col + 1,
															  bindingval,
													ws->perTupleContext, &wasNull);
					if(wasNull)
					  {
					    ws->returnTupleIsNull[col] = 'n';
					  }
					else
					  {
					    ws->returnTupleIsNull[col] = ' ';
					  }
					elog(DEBUG1, "col %d: %s", col, bindingval);
					col++;
					if (col == ws->returnTupleLength)
						break;
				}

				ws->returnTuple[col] = DatumFromTypeInstance(ws->aim,
															 col + 1,
															 colbegin,
													ws->perTupleContext, &wasNull);
				if (wasNull)
					ws->returnTupleIsNull[col] = 'n';
				else
					ws->returnTupleIsNull[col] = ' ';
				if (col == ws->returnTupleLength)
					break;
				col++;
				colbegin = colend + 1;
			}
			for (; col < ws->returnTupleLength; col++)
			{
				ws->returnTuple[col] = PointerGetDatum(NULL);
				ws->returnTupleIsNull[col] = 'n';
			}

            /* SPECIAL CASE: Determining whether we should be generating the
             * timestamps for this stream. */
            if (DONT_KNOW == ws->tsState) {
                if (!tuple_has_timestamp(ws)) {
                    ws->tsState = GENERATING;
                    /* This ensures that we will always generate punctuations
					 * irrespective of the actual data rate. This and the fact
					 * that the invocation of ask_for_punct_timeout() below is
					 * also commented out */
					ask_for_punct_timeout(ws);
                }
            }

            if (GENERATING == ws->tsState) {
                if (tuple_has_timestamp(ws)) {
                    ws->error_message = 
                        "Received an explicit timestamp while generating "
                        "real-time timestamps";
                    PG_RETURN_BOOL(FALSE);
                }

                gen_timestamp_field(ws);

                /* Ask for a timeout CSV_PUNCT_INTERVAL seconds in the future.
                 */
                /* ask_for_punct_timeout(ws); */

#ifdef LOG_TUPLES
                {
                char * ts = DatumGetCString(
                        DirectFunctionCall1(timestamp_out,
                            ws->returnTuple[ws->returnTupleLength - 1]));
                if (NULL == g_csv_logfile_owner) {
                    g_csv_logfile_owner = ws;
                }
                if (g_csv_logfile_owner == ws) {
                    fprintf(g_csv_logfile, "%s,%s\n", tuplecopy, ts);
                }
                pfree(ts);
                pfree(tuplecopy);
                }
#endif /* LOG_TUPLES */

			} else {
                if (!tuple_has_timestamp(ws)) {
                    ws->error_message = 
                        "Tuple has no timestamp, and we are not generating "
                        "real-time timestamps";
                    PG_RETURN_BOOL(FALSE);
                }
            }

			udfstate->tuplecount++;
			ws->hasTuple = true;	/* not safe to return a tuple */

			udfstate->readidx += tuplesize + 1;
			while ((udfstate->readidx < udfstate->writeidx) &&
				   ((udfstate->buf[udfstate->readidx] == '\n') ||
					(udfstate->buf[udfstate->readidx] == '\0')))
				udfstate->readidx++;
		}

	}

	if (udfstate->readidx == udfstate->writeidx)
		ws->hasBufferedData = FALSE;
	PG_RETURN_BOOL(TRUE);
}




Datum
csv_done(PG_FUNCTION_ARGS)
{
        /* Send a done message to the pull wrapper */
	/* free the user allocated state */
	/* declare we are finished */
	/* return success */
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	struct csvstate *udfstate = (struct csvstate *) s->udfstate;
	char		 *command = "DONE";
	int  bwrite = 0;

	/* 
	 *   Ugh ..
	 */
	key_value_entry 	*strmname = get_entry("HIFI_STRM_NAME", s->wrapper_info);

#ifdef LOG_TUPLES
    /* Flush the logfile, but leave it open for other guys. */
    if (s == g_csv_logfile_owner) {
        fflush(g_csv_logfile);
        g_csv_logfile_owner = NULL;
    }
#endif /* LOG_TUPLES */


	if (strmname != NULL)
	{
	  elog(LOG,"Sending DONE message to HiFi Glue");
	  if ((bwrite = write(s->fd,command,strlen(command))) < 0)
	  {
	    s->error_message = "Could not sent DONE to HiFi Glue";
	  }
        }
	/*
	 *  End Ugh 
	 */
	elog(NOTICE, "wrapper done..... processed %d tuples", udfstate->tuplecount);
	pfree(s->udfstate);
	s->udfstate = NULL;
	s->mode = MODE_END;
	PG_RETURN_BOOL(TRUE);
}


/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/

/* Remove stuff that we have consumed from the beginning of the buffer and copy
 * what's left to the beginning of the buffer. */
void compact_buffer(struct csvstate *udfstate) {
    memcpy(udfstate->buf,
           (udfstate->buf + udfstate->readidx),
           (udfstate->writeidx - udfstate->readidx));

    udfstate->writeidx = (udfstate->writeidx - udfstate->readidx);
    udfstate->readidx = 0;
    memset(udfstate->buf + udfstate->writeidx, 'T',
           BUFSIZE - udfstate->writeidx);
}

/* Set the timestamp field of the returned tuple to the current time. 
 * Assumes that the timestamp field comes last. */
void gen_timestamp_field(wrapperstate *ws) {
	MemoryContext oldcxt = MemoryContextSwitchTo(ws->perTupleContext);

    /* We generate the current time as a string, then turn that back into a
     * timestamp.  Yucky, but currently the only clean way to get a timestamp
     * accurate to usec. */
    ws->returnTuple[ws->returnTupleLength - 1] =
        DirectFunctionCall1(
            text_timestamp,
            DirectFunctionCall1(timeofday,PointerGetDatum(NULL)));
    MemoryContextSwitchTo(oldcxt);
    ws->returnTupleIsNull[ws->returnTupleLength - 1] = ' ';
}

/* Create a punctuation tuple and prepare it to be sent to the wrapper
 * clearinghouse. */
void gen_punct_tuple(wrapperstate *ws) {
	struct csvstate *udfstate = (struct csvstate *) ws->udfstate;

    /* We shouldn't be making any punctuation tuples unless the wrapper is
     * generating timestamps for this stream. */
    Assert(GENERATING == ws->tsState);

    /* The wrapper clearinghouse will ignore everything except the timestamp
     * field. */
    gen_timestamp_field(ws);

    ws->returnTupleIsPunct = true;

    udfstate->tuplecount++;
    ws->hasTuple = true;
 
}

/* Returns true of the proto-tuple we're constructing has a timestamp field. */
bool tuple_has_timestamp(wrapperstate *ws) {
    if (ws->returnTuple[ws->returnTupleLength - 1] != PointerGetDatum(NULL)) {
        return true;
    } else {
        return false;
    }
}

/* Ask for a timeout CSV_PUNCT_INTERVAL_SEC seconds in the future. */
void ask_for_punct_timeout(wrapperstate *ws) {
    gettimeofday(&(ws->wakeup_time), NULL);
    ws->wakeup_time.tv_sec += CSV_PUNCT_INTERVAL_SEC;
}



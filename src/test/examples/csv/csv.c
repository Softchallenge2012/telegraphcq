#include <sys/types.h>
#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "postgres.h"
#include "fmgr.h"
#include "telegraphcq/userwrapper.h"
#include "parser/parse_wrapper.h"
#include <time.h>
#include "utils/timestamp.h"
#include "utils/datetime.h"
#include "utils/builtins.h"
#define BUFSIZE 5000
#define BINDINGBUFFERSIZE 2000
PG_FUNCTION_INFO_V1(csv_init);
PG_FUNCTION_INFO_V1(csv_next);
PG_FUNCTION_INFO_V1(csv_done);


Datum
			csv_init(PG_FUNCTION_ARGS);
Datum
			csv_next(PG_FUNCTION_ARGS);
Datum
			csv_done(PG_FUNCTION_ARGS);

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

Datum
csv_init(PG_FUNCTION_ARGS)
{
	int			i = 0;
	char		keystr[100];
	key_value_entry *entry = NULL;

	/* allocate private state */

	struct csvstate *udfstate = NULL;
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

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
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);
	struct csvstate *udfstate = (struct csvstate *) ws->udfstate;
	MemoryContext oldcxt = NULL;

	if (ws->connectionReady == TRUE)	/* wch indicates that the
										 * connection has data */
	{
		/* compact buffer */
		if ((udfstate->writeidx > (BUFSIZE - 50)))
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
				elog(NOTICE,
					 "(testcsv) setting mode to done because 0 bytes read");

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

				delimiters = udfstate->column_delimiters[col];
				if (delimiters == NULL)
					delimiters = udfstate->default_delimiters;

				filter = NULL;
				while ((span_size = strcspn(colbegin, delimiters)) == 0)
				{
					if (++colbegin == tupleend)
						break;
				}
				if (span_size <= 0)
					elog(FATAL, "spansize 0");

				colend = colbegin + span_size;

				if (colend == NULL)
					colend = tupleend;
				if (colend == NULL)
					elog(FATAL, "nextend is NULL");

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
													ws->perTupleContext);
					ws->returnTupleIsNull[col] = ' ';
					elog(DEBUG1, "col %d: %s", col, bindingval);
					col++;
					if (col == ws->returnTupleLength)
						break;
				}

				elog(DEBUG1, "col %d: %s", col, colbegin);
				ws->returnTuple[col] = DatumFromTypeInstance(ws->aim,
															 col + 1,
															 colbegin,
													ws->perTupleContext);

				if (ws->returnTuple[col] == PointerGetDatum(NULL))
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
			if (ws->returnTuple[ws->returnTupleLength - 1] == PointerGetDatum(NULL))
			{
				oldcxt = MemoryContextSwitchTo(ws->perTupleContext);
				ws->returnTuple[ws->returnTupleLength - 1] =
					TimestampGetDatum(GetCurrentTimestamp());
				MemoryContextSwitchTo(oldcxt);
				ws->returnTupleIsNull[ws->returnTupleLength - 1] = ' ';
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
	if (strmname != NULL) 
	{  
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

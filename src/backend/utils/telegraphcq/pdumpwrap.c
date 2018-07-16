/*-------------------------------------------------------------------------
 *
 * pdumpwrap.c
 *
 *	  Built-in wrapper that reads in a libpcap packet dump.
 *
 *	  Based on the quikwrap code.
 *
 *    The protocol for this wrapper is as follows:
 *          -- Connect to the wrapper clearinghouse and send the
 *          wrapper clearinghouse init string.
 *          -- Send the name of the dump file as a pascal string (e.g. 32 bits
 *          of length followed by data).
 *          -- Then hold the connection open as long as you want the packets to
 *          keep streaming.
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/telegraphcq/pdumpwrap.c,v 1.1 2004/07/01 15:55:14 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

/* TEMPORARY HACK to make this file link without libpcap.  This BREAKS the
 * file's functionality!!! */
#define DONT_HAVE_LIB_PCAP
/* END TEMPORARY HACK */

#include <errno.h>
#include <netinet/in.h>
#include <pcap.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>


#include "postgres.h"
#include "fmgr.h"
#include "telegraphcq/userwrapper.h"
#include "parser/parse_wrapper.h"
#include "catalog/pg_type.h"
#include "utils/timestamp.h"
#include "utils/datetime.h"
#include "utils/builtins.h"
#include "lib/stringinfo.h"
#include "executor/spi.h"

#define PDUMP_MAX_FILENAME_LEN 127

/* Define PDUMP_REAL_TIMESTAMP to mark each packet with the actual wall-clock
 * time it was sent up to the query engine, as opposed to the theoretical
 * wall-clock time computed from the timestamps in the packet dump file. */
/*#define PDUMP_REAL_TIMESTAMP*/

PG_FUNCTION_INFO_V1(pdump_init);
PG_FUNCTION_INFO_V1(pdump_next);
PG_FUNCTION_INFO_V1(pdump_done);




Datum
			pdump_init(PG_FUNCTION_ARGS);
Datum
			pdump_next(PG_FUNCTION_ARGS);
Datum
			pdump_done(PG_FUNCTION_ARGS);

/* Status of a connection. */
enum PdumpStatus {
    NOT_INITIALIZED = 0,
    READING_FILENAME_LEN,
    READING_FILENAME_STR,
        /* Reading the filename (as a pascal string) off the wire */
    READING_PACKET,
        /* Reading a packet from the file. */
    WAITING_TO_OUTPUT_TUPLE,
        /* Have generated a tuple, but we're waiting until the appropriate 
         * timeout has elapsed before outputting it. */
    DONE = -1
};


/*
 * This is the private state for the user defined function.
 * An instance will be allocated by pdump_init and freed
 * by pdump_done.
 */
typedef struct pdumpstate
{
    enum PdumpStatus      status;

    struct timeval earliest_timestamp;
                    /* The earliest timestamp on the data file.
                     */

    struct timeval start_time;
                    /* The physical time at which we read the first packet
                     * record from the file.  Logical time offsets from
                     * earliest_timestamp are converted into offsets from
                     * start_time.
                     */

    bool        timestamps_initialized;


    int32       filename_len;
    char        filename_buf[PDUMP_MAX_FILENAME_LEN + 1];
                        /* Leave room for trailing NULL */

	int			tuplecount;
                    /* How many tuples we've created from packets. */

    pcap_t      *dump_file;

} pdumpstate;


/*******************************************************************************
 * Prototypes of internal functions.
 ******************************************************************************/
/* Event handlers for the different states of pdump_next() */
/* These all return TRUE if they manage to make any forward progress. */
static bool handle_reading_filename_len(wrapperstate *ws, pdumpstate *udfstate);

static bool handle_reading_filename_str(wrapperstate *ws, pdumpstate *udfstate);

static bool handle_reading_packet(wrapperstate *ws, pdumpstate *udfstate);

static bool handle_waiting_to_output_tuple(wrapperstate *ws, pdumpstate *udfstate);

static Datum tv2text(struct timeval *tp);

/* Define timersub() macro, in case __POSIX_SOURCE or -ansi are specified */
/* Usage: timersub( a, b, result ) computes result = a - b. */
#ifndef timersub
#define timersub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;			      \
    if ((result)->tv_usec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_usec += 1000000;					      \
    }									      \
  } while (0)
#endif


/*******************************************************************************
 * External functions
 ******************************************************************************/

/* FUNCTION pdump_init
 * ARGUMENTS: A pointer to a wrapperstate struct.
 * PRECONDITIONS: We're connected to a socket that's going to feed us the
 *      name of the file that tcpdump wrote out to disk. 
 * POSTCONDITIONS: 
 */
Datum
pdump_init(PG_FUNCTION_ARGS)
{
    /* Function arguments: */
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

    /* Locals: */
	/*key_value_entry *entry = NULL;*/

	struct pdumpstate *udfstate = NULL;

	/* check that tcqtime is the last attribute of the stream */
	if (ws->tdesc->attrs[ws->tdesc->natts - 1]->atttypid != TIMESTAMPOID)
	{
		ws->error_message = "The last column of any stream which uses the "
                "pdumpwrap wrapper must have datatype TIMESTAMP";
		PG_RETURN_BOOL(FALSE);
	}

    /* Allocate our persistent state. */
	ws->udfstate = palloc(sizeof(struct pdumpstate));
	ws->hasTuple = false;
    ws->hasBufferedData = false;

	udfstate = (struct pdumpstate *) ws->udfstate;

    udfstate->status = NOT_INITIALIZED;

	udfstate->tuplecount = 0;

    udfstate->dump_file = NULL;

    Assert(ws->aim != NULL);

    memset(&(udfstate->earliest_timestamp), 0x0, 
                sizeof(udfstate->earliest_timestamp));
    memset(&(udfstate->start_time), 0x0, sizeof(udfstate->start_time));

    udfstate->timestamps_initialized = false;

	if (ws->fd < 0)
	{
        /* Not sure why this would ever be true. */
        elog(LOG, "pdump_init(): Establishing connection.");
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
	else {
        /* Done with initialization. */
        udfstate->status = READING_FILENAME_LEN;
		PG_RETURN_BOOL(TRUE);
    }
}

/* FUNCTION pdump_next()
 * ARGUMENTS: A pointer to a wrapperstate struct.
 * PRECONDITIONS: You've called pdump_init() on the struct.
 * POSTCONDITIONS: Depending on the state of the connection, reads data and/or
 *      spits out a tuple.
 */
Datum
pdump_next(PG_FUNCTION_ARGS)
{
    /* Arguments */
	wrapperstate *ws = (wrapperstate *) PG_GETARG_POINTER(0);

    /* Locals */
	struct pdumpstate *udfstate = (struct pdumpstate *) ws->udfstate;
    bool        keepgoing = false;

    /* Repeat as long as we keep making forward progress. */
    do { 
        switch (udfstate->status) {
            case NOT_INITIALIZED:
                udfstate->status = READING_FILENAME_LEN;
                keepgoing = true;
                break;

            case READING_FILENAME_LEN:
                keepgoing = handle_reading_filename_len(ws, udfstate);
                break;

            case READING_FILENAME_STR:
                keepgoing = handle_reading_filename_str(ws, udfstate);
                break;

            case READING_PACKET:
                keepgoing = handle_reading_packet(ws, udfstate);
                break;
    
            case WAITING_TO_OUTPUT_TUPLE:
                keepgoing = handle_waiting_to_output_tuple(ws, udfstate);
                break;

            default:
                elog(ERROR, "pdump_next(): Invalid wrapper state!");
                /* unreachabe. */
                break;
        }

    } while (true == keepgoing);

    /* Did an error occur? */
    if (NULL != ws->error_message) {
        PG_RETURN_BOOL(FALSE);
    } else {
        PG_RETURN_BOOL(TRUE);
    }
}




Datum
pdump_done(PG_FUNCTION_ARGS)
{
	/* free the user allocated state */
	/* declare we are finished */
	/* return success */
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	struct pdumpstate *udfstate = (struct pdumpstate *) s->udfstate;

    /* Calculate how long we spent in this wrapper. */
    int sec_elapsed = -1;
    struct timeval now;
    gettimeofday(&now, NULL);
    sec_elapsed = now.tv_sec - udfstate->start_time.tv_sec;
	elog(NOTICE, "quikwrapper processed %d tuples in %d seconds", 
            udfstate->tuplecount, sec_elapsed);
	pfree(udfstate);
	s->udfstate = NULL;
	s->mode = MODE_END;

	PG_RETURN_BOOL(TRUE);
}


/*******************************************************************************
 * INTERNAL FUNCTIONS
 ******************************************************************************/

static bool handle_reading_filename_len(wrapperstate *ws, pdumpstate *udfstate) 
{
    int numread = read(ws->fd, &(udfstate->filename_len), sizeof(int32));

    /* TODO: Handle partial reads. */
    if (numread != sizeof(int32)) {
        ws->error_message = "Read wrong number of filename length bytes.";
        return false;
    }

    /* Fix endian issues (we hope) */
    udfstate->filename_len = ntohl(udfstate->filename_len);

    if (udfstate->filename_len > PDUMP_MAX_FILENAME_LEN) {
        ws->error_message = "Dump filename is too long.";
        return false;
    }

    udfstate->status = READING_FILENAME_LEN;
    return true;
}

static bool handle_reading_filename_str(wrapperstate *ws, pdumpstate *udfstate) 
{
    char errbuf[PCAP_ERRBUF_SIZE];

    int numread = read(ws->fd, udfstate->filename_buf, udfstate->filename_len);

    /* TODO: Handle partial reads. */
    if (numread != udfstate->filename_len) {
        ws->error_message = "Read wrong number of filename bytes.";
        return false;
    }

    /* Add a trailing NULL.  And yes, we did leave room for the trailing NULL
     * in the struct. */
    udfstate->filename_buf[udfstate->filename_len] = '\0';

    /* Open the file! */
#ifndef DONT_HAVE_LIB_PCAP
    if ((udfstate->dump_file 
            = pcap_open_offline(udfstate->filename_buf, errbuf) ) == NULL)
    {
        ws->error_message = "Error opening dump file.";
        return false;
    }
#endif /* DONT_HAVE_LIB_PCAP */

    /* We always have buffered data available, at least until we run through
     * the entire dump file! */
    ws->hasBufferedData = true;

    udfstate->status = READING_PACKET;
    return true;
}


static bool handle_reading_packet(wrapperstate *ws, pdumpstate *udfstate) 
{
    struct pcap_pkthdr header;
    u_char *pkt_data;

    MemoryContext oldcxt;

    /* Read the next packet from the file. */
#ifndef DONT_HAVE_LIB_PCAP
    pkt_data = (u_char*)pcap_next(udfstate->dump_file, &header);
        /* This does NOT leak memory, since pcap manages the chunk of memory
         * that pkt_data points to.  We just have to be careful to copy
         * everything out of there, since the memory will be overwritten on the
         * next call to pcap_next()!
         */
#endif /* DONT_HAVE_LIB_PCAP */

    if (NULL == pkt_data) {
        /* We've read the last packet in the file. */
        udfstate->status = DONE;
        ws->mode = MODE_DONE;
        ws->hasBufferedData = false;
        return false;
    }

    if (false == udfstate->timestamps_initialized) {
        /* Need to record "baseline" timestamps. */ 
        udfstate->earliest_timestamp = header.ts;
        gettimeofday(&(udfstate->start_time), NULL);
    }

    
    /* Create a new tuple based on the one we've just read. */
    oldcxt = MemoryContextSwitchTo(ws->perTupleContext);
    { /* Begin per tuple memory context. */

        /* Calculate the real time that this tuple will enter the query engine */
        timersub(&(header.ts), &(udfstate->earliest_timestamp), &(ws->wakeup_time));


#ifndef PDUMP_REAL_TIMESTAMP
        /* We put the artifical timestamp instead of the real-time one into
         * the tuple, so that tuples will not jump from one window to the
     * next. */
        /* TODO: Convert directly to timestamp without going through text
         * part. */
        ws->returnTuple[ws->returnTupleLength - 1] =
                DirectFunctionCall1(
                    text_timestamp,
                    tv2text(&(ws->wakeup_time)));
        ws->returnTupleIsNull[ws->returnTupleLength - 1] = ' ';
#endif /* ! PDUMP_REAL_TIMESTAMP */

        /* TODO: Grab the other fields of the tuple from the packet! */

        /* The contents of ws->returnTuple are pointers into cur_tup, so
         * it's important that we don't call heap_freetuple() on it here!
         */

    } /* End per tuple memory context. */
    MemoryContextSwitchTo(oldcxt);

    /* Move to next state. */
    udfstate->status = WAITING_TO_OUTPUT_TUPLE;

    ws->hasTuple = false;
        /* We wait until the appropriate timeout has elapsed before
         * emitting the tuple. */

    ws->asleep = true;

    /* We return FALSE here because we want to go to sleep. */
    return false;
}

static bool handle_waiting_to_output_tuple(wrapperstate *ws, pdumpstate *udfstate) 
{
    /* This function should only be called when the current timeout has
     * elapsed. */

#ifdef PDUMP_REAL_TIMESTAMP
    /* Don't forget Mr. Timestamp!  We want this timestamp to represent the
     * time that this tuple was sent up to the wrapper clearinghouse, so we
     * create it here. */
    MemoryContext oldcxt = MemoryContextSwitchTo(ws->perTupleContext);
    { /* Begin per tuple memory context. */
        ws->returnTuple[ws->returnTupleLength - 1] =
                DirectFunctionCall1(
                    text_timestamp,
                    DirectFunctionCall1(timeofday,PointerGetDatum(NULL)));
        ws->returnTupleIsNull[ws->returnTupleLength - 1] = ' ';
    } /* End per tuple memory context. */
    MemoryContextSwitchTo(oldcxt);
#endif /* PDUMP_REAL_TIMESTAMP */
    
    /* Indicate to the caller that the tuple is ready for processing. */
    ws->hasTuple = true;

    /* Move to the next state. */
    udfstate->status = READING_PACKET;

    /* We need to return FALSE here to give the caller time to pass the
     * tuple back through the clearinghouse. */
    return false;
}


/*
 * Other internal functions.
 */

/* Convert a struct timeval into a string Datum.  Based on code in 
 * timeofday() in nabstime.c. */
static Datum tv2text(struct timeval *tp) {
    char		templ[100];
	char		buf[100];
    text	   *result;
	int			len;


    strftime(templ, sizeof(templ), "%a %b %d %H:%M:%S.%%06d %Y %Z",
			 localtime((time_t *) &(tp->tv_sec)));
    /*elog(LOG, "tv2text: usec is %d", tp->tv_usec);*/
    /*elog(LOG, "tv2text: templ is '%s'", templ);*/
	snprintf(buf, sizeof(buf), templ, tp->tv_usec);

    /*elog(LOG, "tv2text: buf is '%s'", buf);*/

    /* Add a trailing NULL for c-compatiblity */
	len = VARHDRSZ + strlen(buf);
	result = (text *) palloc(len + 1);
	VARATT_SIZEP(result) = len;
	memcpy(VARDATA(result), buf, strlen(buf) + 1);
	PG_RETURN_TEXT_P(result);
}



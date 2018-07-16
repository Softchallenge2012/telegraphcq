/*-------------------------------------------------------------------------
 *
 * tcq_usleep.c
 *
 *	  Replacement for usleep(3).
 *
 *	  usleep(3) can do all sorts of crazy things when Postgres processes talk
 *	  to each other, so we emulate its behavior using select().
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 *
 * IDENTIFICATION
 *	  $Header: /project/eecs/db/cvsroot/postgres/src/backend/utils/telegraphcq/tcq_usleep.c,v 1.3 2004/03/24 08:11:20 chungwu Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "telegraphcq/tcq_usleep.h"

#include <sched.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* See comment at top of file. */
int tcq_usleep(int usec) {
    struct timeval delay;
    /*
    struct timeval before;
    struct timeval after;
    */

    /* Currently we get a min. delay of whatever the kernel timeslice is. */
    /* TODO: Spin-wait for delays below 10 msec or so. */

    /* SPECIAL CASE: Delay of zero -- don't sleep at all. */
    if (usec == 0) {
        /* TODO: Should we be calling sched_yield() here? */
        return 0;
    }
    /* END SPECIAL CASE. */

    delay.tv_sec = 0;
    delay.tv_usec = usec;

    /*gettimeofday(&before, NULL);*/

    (void) select(0, NULL, NULL, NULL, &delay);

    /*gettimeofday(&after, NULL);*/

    /*
    elog(LOG, "tcq_usleep(%d): Before: %lu sec and %lu usec.",
            usec, before.tv_sec, before.tv_usec);
    elog(LOG, "tcq_usleep(%d):     After: %lu sec and %lu usec.",
            usec, after.tv_sec, after.tv_usec);
            */

    return 0;
}


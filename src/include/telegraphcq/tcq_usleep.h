
/* -------------------------------------------------------------------------
 *
 * tcq_usleep.h
 *
 *    Replacement for usleep(3).
 *
 *    usleep(3) can do all sorts of crazy things when Postgres processes talk
 *    to each other, so we emulate its behavior using select().
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 *
 * IDENTIFICATION
 *    $Header: /project/eecs/db/cvsroot/postgres/src/include/telegraphcq/tcq_usleep.h,v 1.2 2004/03/24 08:11:29 chungwu Exp $
 *
 * -------------------------------------------------------------------------
 */

int tcq_usleep(int usec);


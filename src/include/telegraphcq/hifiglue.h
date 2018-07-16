/*-------------------------------------------------------------------------
 *
 * hifiglue.h
 *
 * This file declares the functions that make up the implementation of
 * the HiFi Glue server.  This essentially is a separate process that
 * serves as the "glue" between telegraphCQ's and performs system-wide
 * query rewriting
 *
 * Portions Copyright (c) 2004, Regents of the University of California
 *
 * IDENTIFICATION
 *           $Header: /project/eecs/db/cvsroot/postgres/src/include/telegraphcq/hifiglue.h,v 1.1 2004/09/10 07:08:11 sirrice Exp $
 *
 *-------------------------------------------------------------------------
 */

#ifndef HIFIGLUE_H
#define HIFIGLUE_H


extern int	acceptPort;
extern char *telegraphcqwchport;

int		HiFiGlueMain(int argc, char *argv[]);
int     hifi_main_loop();
void	AbortHiFiGlue(void);

#endif   /* WRAPCH_H */

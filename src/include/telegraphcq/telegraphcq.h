/*-------------------------------------------------------------------------
 *
 * telegraphcq.h
 *
 * This file declares the functions that make up the implementation of the
 * TelegraphCQ backend
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * $Id: telegraphcq.h,v 1.15 2004/10/04 22:49:43 phred Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef TELEGRAPHCQ_H
#define TELEGRAPHCQ_H

#include "telegraphcq/tcq_usleep.h"
#include "telegraphcq/telegraphcqinit.h"
#include "utils/timestamp.h"

extern char *telegraphcqdb;
extern char *telegraphcquser;
extern int32 tcqlocaltz;

extern void InitializeTCQPlumbing(void);
extern void CleanupTCQPlumbing(void);
extern int	DoTelegraphCQ(void);
extern int	TelegraphCQMain(int argc, char *argv[]);
extern void AbortCurrentQuery(void);
extern void setTCQJoinOptions(void);
extern void resetJoinOptions(void);
extern Timestamp GetCurrentTimestamp(void);

#endif   /* TELEGRAPHCQ_H */

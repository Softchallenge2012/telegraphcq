
/*-------------------------------------------------------------------------
 *
 * hifiglue.c
 *
 * HiFi Glue.  A separate process that serves as a proxy server for
 * connecting multiple telegraphCQ's as well as a query rewrite engine
 * for dissemination into the HiFi system
 *
 * Portions Copyright (c) 2004, Regents of the University of California
 *
 * IDENTIFICATION
 *           $Header: /project/eecs/db/cvsroot/postgres/src/backend/telegraphcq/hifiglue.c,v 1.3 2005/07/27 20:04:12 phred Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "libpq/pqsignal.h"
#include "miscadmin.h"
#include "telegraphcq/telegraphcq.h"
#include "telegraphcq/hifiglue.h"
#include "utils/fcache.h"
#include "utils/guc.h"
#include "utils/inval.h"
#include "utils/memutils.h"
#include "utils/ps_status.h"
#include "utils/relcache.h"



void		restore_working_directory(int code, char *pathname);

char	   *telegraphcqdb;
char	   *telegraphcquser;
int32		tcqlocaltz;
static int  running = 1;

#define ARGV_SIZE	64

/* from tcop/postgres.c */
extern CommandDest whereToSendOutput;
extern int	       random_seed;

int 
HiFiGlueMain(int argc, char** argv) {
	char	usrbuf[ARGV_SIZE];
	int	status;
    /*pid_t   pid = getpid();*/

	//setupGlobalConfig();

	whereToSendOutput = Debug;
	StrNCpy(usrbuf, GetConfigOption("telegraphcquser"), ARGV_SIZE);

	//Why does it call postgresmain?
	
	//status = PostgresMain(argc, argv, usrbuf);

	//Set the process name so we can debug it later
	set_ps_display("hifiglue");

	PG_SETMASK(&UnBlockSig);	// Unblock signals
	MemoryContextSwitchTo(TopMemoryContext);	//Top context
	

	hifi_main_loop();

	
	return status;
}


int
hifi_main_loop() {
	do {
		/* 
		   get query plan
		   fold into eddy
		   generate new query
		   get destination from MDR
		   send to destination
		*/
		AbortHiFiGlue();
	} while (running);
	return 0;
}


void		
AbortHiFiGlue(void){
	running = false;
}

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

#define GLUEBUFFERSIZE 100

PG_FUNCTION_INFO_V1(glue_rescan);

/*
  This file defines a rescan function for the GlueServer. it can be used with
  the csv wrapper.  See gluewrapper.sql ..
*/

Datum glue_rescan(PG_FUNCTION_ARGS);

Datum
glue_rescan(PG_FUNCTION_ARGS)
{
	char		 command[GLUEBUFFERSIZE];
	int         	 commandlen;	
	int		 bwrite = 0;
	wrapperstate 	*s 	= (wrapperstate *) PG_GETARG_POINTER(0);

	key_value_entry 	*strmname = get_entry("HIFI_STRM_NAME", s->wrapper_info);
	key_value_entry 	*wchport  = get_entry("HIFI_PORT", s->wrapper_info);

	if (hasNewProbeTuple(s))
	{
  
		*command = '\0';
		memset(command, 0, GLUEBUFFERSIZE);

		sprintf(command,"%s;%s\n",strmname->value,wchport->value);
		/* elog(LOG,"Command %s ", command); */
		commandlen = strlen(command);
		if ((bwrite = write(s->fd,
							(void *) command,
							commandlen)) < 0)
		{
			char *em = (char *) palloc(300);
			char *fem = (char *) palloc(320);
			strerror_r(errno,em,300);
			sprintf(fem, "Couldn't send query to HiFi Glue: %s",em);
			s->error_message = fem;
			PG_RETURN_BOOL(FALSE);
		}
		else 
		{
			s->mode = MODE_NEXT;
			PG_RETURN_BOOL(TRUE);
		}
	}
  
	PG_RETURN_BOOL(TRUE);
}

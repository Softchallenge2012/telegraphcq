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

#define BINDINGBUFFERSIZE 25
PG_FUNCTION_INFO_V1(geocode_rescan);

/* this file defines a rescan function for the WebQueryServer. it can be used
   with the csv wrapper.  see web.sql.
*/

Datum
			geocode_rescan(PG_FUNCTION_ARGS);

Datum
geocode_rescan(PG_FUNCTION_ARGS)
{
	char		command[BINDINGBUFFERSIZE];

	int			bwrite = 0;
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);

	*command = '\0';
	memset(command, 0, BINDINGBUFFERSIZE);

	if (hasNewProbeTuple(s))
	{
		int			bytesleft = BINDINGBUFFERSIZE;
		int			commandlen = 0;
		char	   *value = NULL;
		int			valuelen = 0;

		value = getBindingValueByNumber(s, 1);
		valuelen = strlen(value) + 1;
		bytesleft -= valuelen + 1;
		if (bytesleft > 0)
		{
			char	   *cp;

			cp = strchr(value, '/');
			if (cp)
				*cp = '\0';
			strcat(command, value);
		}
		else
		{
			s->error_message = "binding string too long";
			PG_RETURN_BOOL(FALSE);
		}
		commandlen = 16;
		if ((bwrite = write(s->fd,
							(void *) command,
							commandlen)) < 0)
		{
			s->error_message = "couldn't send query";
			PG_RETURN_BOOL(FALSE);
		}
		if (bwrite != commandlen)
		{
			s->error_message = "couldn't write the entire query";
			PG_RETURN_BOOL(FALSE);
		}
		s->mode = MODE_NEXT;
		PG_RETURN_BOOL(TRUE);
	}
	else
		PG_RETURN_BOOL(TRUE);	/* no probe yet, continue in rescan */

}

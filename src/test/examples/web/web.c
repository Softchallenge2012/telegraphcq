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

#define BINDINGBUFFERSIZE 4000
PG_FUNCTION_INFO_V1(web_rescan);

/* this file defines a rescan function for the WebQueryServer. it can be used
   with the csv wrapper.  see web.sql.
*/

Datum
			web_rescan(PG_FUNCTION_ARGS);

Datum
web_rescan(PG_FUNCTION_ARGS)
{
	char		command[BINDINGBUFFERSIZE];

	int			bwrite = 0;
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	key_value_entry *query = get_entry("QUERY", s->wrapper_info);

	*command = '\0';

	if (hasNewProbeTuple(s))
	{
		int			numbindings = getNumBindings(s);
		int			i = 0;
		int			bytesleft = BINDINGBUFFERSIZE;
		char	   *value = NULL;
		int			valuelen = 0;

		valuelen = strlen(query->value) + 1;
		bytesleft -= valuelen;
		if (bytesleft > 0)
			strcat(command, query->value);
		else
		{
			s->error_message = "binding string too long";
			PG_RETURN_BOOL(FALSE);
		}
		for (i = 1; i <= numbindings; i++)
		{

			value = getBindingValueByNumber(s, i);
			valuelen = strlen(value) + 1;
			bytesleft -= valuelen + 1;
			if (bytesleft > 0)
			{
				strcat(command, ",");
				strcat(command, value);
			}
			else
			{
				s->error_message = "binding string too long";
				PG_RETURN_BOOL(FALSE);
			}

		}

		if (query)
		{
			if ((bwrite = write(s->fd,
								(void *) command,
								strlen(command))) < 0)
			{
				s->error_message = "couldn't send query";
				PG_RETURN_BOOL(FALSE);
			}
			if (bwrite != strlen(command))
			{
				s->error_message = "couldn't write the entire query";
				PG_RETURN_BOOL(FALSE);
			}
			write(s->fd, "\n", 1);
			s->mode = MODE_NEXT;
			PG_RETURN_BOOL(TRUE);
		}
		else
			PG_RETURN_BOOL(FALSE);

	}
	PG_RETURN_BOOL(TRUE);

}

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

#define BINDINGBUFFERSIZE 10240
PG_FUNCTION_INFO_V1(webservices_rescan);

/* this file defines a rescan function for the WebQueryServer. it can be used
   with the csv wrapper.  see web.sql.
*/

Datum
			webservices_rescan(PG_FUNCTION_ARGS);

Datum
webservices_rescan(PG_FUNCTION_ARGS)
{
	char		command[BINDINGBUFFERSIZE];

	int			bwrite = 0;
	wrapperstate *s = (wrapperstate *) PG_GETARG_POINTER(0);
	key_value_entry *package = get_entry("package", s->wrapper_info);
	key_value_entry *service = get_entry("service", s->wrapper_info);
	key_value_entry *schema =  get_entry("schema", s->wrapper_info);
	key_value_entry *port =  get_entry("wsdlport", s->wrapper_info);
	key_value_entry *operation = get_entry("operation", s->wrapper_info);
	
	if(package == NULL || service == NULL || schema == NULL || port == NULL ||  operation == NULL)
	{
		s->error_message = "webservices_rescan requires package,service, and operation key-value pairs";
			PG_RETURN_BOOL(FALSE);
	}
	*command = '\0';

	if (hasNewProbeTuple(s))
	{
		int			numbindings = getNumBindings(s);
		int			i = 0;
		int			bytesleft = BINDINGBUFFERSIZE;
		char	   *value = NULL;
		int			valuelen = 0;

		valuelen = strlen(package->value) + 2;
		bytesleft -= valuelen;
		if (bytesleft > 0)
		{
			strcat(command, package->value);
			strcat(command,",");
		}
		else
		{
			s->error_message = "binding string too long";
			PG_RETURN_BOOL(FALSE);
		}
		valuelen = strlen(service->value) + 2;
		bytesleft -= valuelen;
		if (bytesleft > 0)
		{
			strcat(command, service->value);
			strcat(command,",");
		}
		else
		{
			s->error_message = "binding string too long";
			PG_RETURN_BOOL(FALSE);
		}
		valuelen = strlen(schema->value) + 2;
		bytesleft -= valuelen;
		if (bytesleft > 0)
		{
			strcat(command, schema->value);
			strcat(command,",");
		}
		else
		{
			s->error_message = "binding string too long";
			PG_RETURN_BOOL(FALSE);
		}
		valuelen = strlen(port->value) + 2;
		bytesleft -= valuelen;
		if (bytesleft > 0)
		{
			strcat(command, port->value);
			strcat(command,",");
		}
		else
		{
			s->error_message = "binding string too long";
			PG_RETURN_BOOL(FALSE);
		}
		valuelen = strlen(operation->value) + 2;
		bytesleft -= valuelen;
		if (bytesleft > 0)
		{
			strcat(command, operation->value);
		}
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
	PG_RETURN_BOOL(TRUE);

}




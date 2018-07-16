/*-------------------------------------------------------------------------
 *
 * debug.c
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: debug.c,v 1.10 2003/12/16 22:56:23 mashah Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include "utils/debug.h"
#include "utils/elog.h"

struct timeval saved_times[100];
char	   *saved_messages[100];
int			num_saved = 0;

FILE	   *output_times_file = NULL;
struct timeval beginning_time;

void		debug_log_message(char *str);
void		debug_print_times(void);
void		finish_recording_output_times(void);
void		record_output_time(void);
void		init_recording_output_times(void);

void
debug_log_message(char *str)
{
	if (num_saved < 100)
	{
		gettimeofday(saved_times + num_saved, 0);
		saved_messages[num_saved] = str;
		num_saved++;
	}
}

void
debug_print_times()
{
	int			i;

	for (i = 0; i < num_saved; i++)
        elog(LOG, "%d.%.6d : %s", 
                (int) saved_times[i].tv_sec, 
                (int) saved_times[i].tv_usec, saved_messages[i]);
}

void
init_recording_output_times()
{
	char		file_name[80];

	gettimeofday(&beginning_time, 0);
	sprintf(file_name, "outputtimes_%ld", (long) beginning_time.tv_sec);
	output_times_file = fopen(file_name, "w");
}

void
record_output_time()
{
	struct timeval now;
	long		l;

	gettimeofday(&now, 0);
	l = (long) (now.tv_usec - beginning_time.tv_usec);
	if (l < 0)
		fprintf(output_times_file, "%ld.%.6ld\n", (long) (now.tv_sec - beginning_time.tv_sec - 1), l + 1000000);
	else
		fprintf(output_times_file, "%ld.%.6ld\n", (long) (now.tv_sec - beginning_time.tv_sec), l);
}

void
finish_recording_output_times()
{
	fclose(output_times_file);
}

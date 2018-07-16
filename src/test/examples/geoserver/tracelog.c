/*
 *
 * This code is written by Lakshminarayanan Subramanian from the University of
 * California at Berkeley. This software was created during his internship
 * in Microsoft Research, Redmond under the mentorship of Venkata
 * Padmanabhan. This software is copyrighted by the authors, Microsoft
 * Research and the Regents of University of California at Berkeley.
 *
 * The following terms apply to all files associated with the software unless
 * explicitly disclaimed in individual files.
 *
 * The authors hereby grant permission to use this software without
 * fee or royalty for any non-commercial purpose.  The authors also
 * grant permission to redistribute this software, provided this
 * copyright and a copy of this license (for reference) are retained
 * in all distributed copies.
 *
 * IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 * FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 * DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.	THIS SOFTWARE
 * IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
 * NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 * MODIFICATIONS.
 */


/*
 * This code is similar to "gettrace.c" except for the fact that this operates
 * over a log of traceroute data rather than performing the traceroute in
 * the code itself.
 */

/* NOTE: Presently there is an artificial limit of 2000 records in the log.
 * To change this, modify the NUMRECORDS field in the "tracelog.h" file.
 */

#include "loadcodes.h"
#include "parselabel.h"
#include "gettrace.h"
#include "tracelog.h"
#include "caldist.h"

void
initialize_trrecord()
{

	int			i;

	trrecord.dest[0] = 0;
	for (i = 0; i < NUMHOPS; i++)
	{
		trrecord.trace_label[i][0] = 0;
		trrecord.location[i][0] = 0;
		trrecord.networkname[i][0] = 0;
		trrecord.lat[i] = -1.0;
		trrecord.lon[i] = -181.0;
		trrecord.delay[i] = -1;
		trrecord.sourdist[i] = 0;
		trrecord.sourlindist[i] = 0;
	}
	numhops_record = 0;

	return;
}




void
get_networkname(char label[STRINGSIZE], char net[STRINGSIZE])
{

	int			i,
				j,
				k;
	char		pieces[NUMPIECES][STRINGSIZE];
	char		net1[STRINGSIZE];

	i = 0;
	j = 0;
	k = 0;
	while (label[i] != 0 && label[i] != '\n')
	{
		if (label[i] == '.')
		{
			pieces[j][k] = 0;
			j++;
			k = 0;
		}
		else
		{
			pieces[j][k] = label[i];
			k++;
		}
		i++;
	}

	pieces[j][k] = 0;

	if (j == 0)
	{
		net[0] = 0;
		return;
	}
	if (isdigit(pieces[j - 1][0]))
	{
		net[0] = 0;
		return;
	}

	strcpy(net1, pieces[j - 1]);
	convert_small(net1, net);

	return;
}




int
is_blank(char str[STRINGSIZE])
{

	int			i;

	i = 0;
	while (str[i] != '\0' && str[i] != '\n')
	{
		if (str[i] != ' ' || str[i] != '\t')
			return 0;
		i++;
	}

	return 1;
}

int
get_next_record(FILE *fp)
{

	char		str[STRINGSIZE],
				str1[STRINGSIZE];
	char		del[STRINGSIZE],
				loc1[STRINGSIZE];
	int			i,
				delay;

	if (fgets(str, STRINGSIZE, fp) == NULL)
		return 0;


	errcount_record++;

	if (!is_blank(str))
	{
		sscanf(str, "%s %s\n", str1, loc1);
		strcpy(trrecord.dest, str1);
	}
	else
	{
		printf("Checkpoint 1: Mismatch in format of Log file:record=%d\n", errcount_record);
		exit(1);
	}
	if (fgets(str, STRINGSIZE, fp) == NULL)
	{
		printf("Checkpoint 2: Error in file:record=%d \n", errcount_record);
		exit(1);
	}

	if (!is_blank(str))
	{
		printf("Checkpoint 3: Mismatch in format of Log file:record=%d\n", errcount_record);
		exit(1);
	}

	i = 0;

	if (fgets(str, STRINGSIZE, fp) == NULL)
	{
		printf("Checkpoint 4: Error in file \n");
		exit(1);
	}

	while (!(is_blank(str)) && i < NUMHOPS)
	{
		sscanf(str, "%s %s\n", str1, del);
		delay = extract_delay(del);
		if (delay == -INFINITY)
		{
			sscanf(str, "%s %s\n", str1, loc1);
			strcpy(trrecord.dest, str1);
			if (fgets(str, STRINGSIZE, fp) == NULL)
			{
				printf("Checkpoint 2a: Error in file:record=%d \n", errcount_record);
				exit(1);
			}
			if (!is_blank(str))
			{
				printf("Checkpoint 3a: Mismatch in format of Log file:record=%d\n", errcount_record);
				exit(1);
			}
			i = 0;
			if (fgets(str, STRINGSIZE, fp) == NULL)
			{
				printf("Checkpoint 4a: Error in file \n");
				exit(1);
			}
		}
		trrecord.delay[i] = delay;
		strcpy(trrecord.trace_label[i], str1);
		i++;
		if (fgets(str, STRINGSIZE, fp) == NULL)
			str[0] = 0;
	}

	numhops_record = i;

	if (i == NUMHOPS)
	{
		printf("Too many hops in the log of %s\n", trrecord.dest);
		exit(1);
	}

	return 1;
}


void
delay_verify_record()
{

	char		prev[STRINGSIZE];
	int			prevdelay = NEGINFINITE;
	int			delaydiff;
	int			i;

	prev[0] = 0;

	for (i = 0; i < numhops_record; i++)
	{
		if (trrecord.location[i][0] != 0)
		{
			if (prev[0] == 0)
			{
				strcpy(prev, trrecord.location[i]);
				prevdelay = trrecord.delay[i];
			}
			else
			{
				if (is_in_usa(prev) && !is_in_usa(trrecord.location[i]))
				{
					delaydiff = trrecord.delay[i] - prevdelay;
					if (delaydiff < USEUROPE_DELAYDIFF)
						trrecord.location[i][0] = 0;
					else
					{
						strcpy(prev, trrecord.location[i]);
						prevdelay = trrecord.delay[i];
					}
				}
				else if (!is_in_usa(prev) && is_in_usa(trrecord.location[i]))
				{
					delaydiff = trrecord.delay[i] - prevdelay;
					if (delaydiff < USEUROPE_DELAYDIFF)
						trrecord.location[i][0] = 0;
					else
					{
						strcpy(prev, trrecord.location[i]);
						prevdelay = trrecord.delay[i];
					}
				}
				else
				{
					strcpy(prev, trrecord.location[i]);
					prevdelay = trrecord.delay[i];
				}
			}
		}
	}

	return;
}



void
process_trrecord_lindist(char source[STRINGSIZE], int *finalsourdist, int *finalsourlindist)
{

	int			i;
	float		latprev,
				lonprev,
				latnew,
				lonnew;
	float		latsour,
				lonsour;
	int			lindist,
				dist,
				sourdist;

	for (i = 0; i < numhops_record; i++)
		get_location(trrecord.trace_label[i], trrecord.location[i]);

	delay_verify_record();

	lindist = 0;
	sourdist = 0;

	if (source[0] != 0)
	{
		strcpy(trrecord.location[0], source);
		get_latlong(trrecord.location[0], &latprev, &lonprev);
		latsour = latprev;
		lonsour = lonprev;
		if (lonsour == -181.0)
		{
			printf("Improper Source Entry\n");
			exit(1);
		}
		trrecord.lat[0] = latsour;
		trrecord.lon[0] = lonsour;

		for (i = 1; i < numhops_record; i++)
		{
			if (trrecord.location[i][0] != 0)
			{
				get_latlong(trrecord.location[i], &latnew, &lonnew);
				if (lonnew == -181.0)
				{
					printf("Check Location database for Location %s\n",
						   trrecord.location[i]);
					exit(1);
				}
				trrecord.lat[i] = latnew;
				trrecord.lon[i] = lonnew;
				trrecord.sourdist[i] = get_distance(latsour, latnew, lonsour, lonnew);
				sourdist = trrecord.sourdist[i];
				dist = get_distance(latprev, latnew, lonprev, lonnew);
				lindist += dist;
				trrecord.sourlindist[i] = lindist;
				latprev = latnew;
				lonprev = lonnew;
			}
			else
			{
				trrecord.sourdist[i] = -1;
				trrecord.sourlindist[i] = -1;
			}
		}
	}
	else
	{
		printf("Please enter source info\n");
		exit(1);
	}

	*finalsourdist = sourdist;
	*finalsourlindist = lindist;

	return;
}


void
process_trrecord(char loc[STRINGSIZE])
{

	int			i;

	for (i = 0; i < numhops_record; i++)
		get_location(trrecord.trace_label[i], trrecord.location[i]);

	delay_verify_record();

	loc[0] = 0;
	for (i = 0; i < numhops_record; i++)
	{
		if (trrecord.location[i][0] != 0)
			strcpy(loc, trrecord.location[i]);
	}

	return;
}


void
process_log(char logfile[STRINGSIZE], char outputfile[STRINGSIZE])
{

	FILE	   *fp,
			   *fp1;
	char		loc[STRINGSIZE];

	fp = fopen(logfile, "r");
	if (outputfile[0] != 0)
		fp1 = fopen(outputfile, "w");
	else
		fp1 = NULL;

	initialize_trrecord();

	while (get_next_record(fp) == 1)
	{
		process_trrecord(loc);
		if (fp1 != NULL)
			fprintf(fp1, "%s %s\n", trrecord.dest, loc);
		else
			printf("%s %s\n", trrecord.dest, loc);

		initialize_trrecord();
	}
	fclose(fp);
	if (fp1 != NULL)
		fclose(fp1);

	return;
}


void
copy_record(int num, char source[STRINGSIZE])
{

	int			i;
	char		loc[STRINGSIZE],
				net[STRINGSIZE],
				prevnet[STRINGSIZE];
	int			prevlindist;

	strcpy(loc, source);
	strcpy(rec[num].dest, trrecord.dest);
	rec[num].numhops = numhops_record;
	for (i = 0; i < NUMHOPS; i++)
	{
		strcpy(rec[num].trace_label[i], trrecord.trace_label[i]);
		if (trrecord.location[i][0] != 0)
			strcpy(loc, trrecord.location[i]);
		strcpy(rec[num].location[i], loc);
		rec[num].lat[i] = trrecord.lat[i];
		rec[num].lon[i] = trrecord.lon[i];
		rec[num].delay[i] = trrecord.delay[i];
		rec[num].sourdist[i] = trrecord.sourdist[i];
	}

	prevnet[0] = 0;
	net[0] = 0;
	prevlindist = 0;
	for (i = 0; i < numhops_record; i++)
	{
		get_networkname(trrecord.trace_label[i], net);
		if (net[0] != 0)
		{
			strcpy(rec[num].networkname[i], net);
			strcpy(prevnet, net);
		}
		else
			strcpy(rec[num].networkname[i], prevnet);
		if (trrecord.sourlindist[i] != -1)
			prevlindist = trrecord.sourlindist[i];
		rec[num].sourlindist[i] = prevlindist;
	}

	return;
}



int
get_recordnum(char dest[STRINGSIZE])
{

	int			i;

	for (i = 0; i < numrecords; i++)
	{
		if (strcmp(rec[i].dest, dest) == 0)
			return i;
	}

	return -1;
}



void
load_logfile(char logfile[STRINGSIZE], char source[STRINGSIZE])
{

	FILE	   *fp;
	int			sourdist,
				lindist;

	fp = fopen(logfile, "r");
	initialize_trrecord();
	numrecords = 0;


	while (get_next_record(fp) == 1)
	{
		process_trrecord_lindist(source, &sourdist, &lindist);
		copy_record(numrecords, source);
		numrecords++;
		initialize_trrecord();
	}

	fclose(fp);

	return;
}




void
process_log_lindist(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE])
{

	int			i;
	FILE	   *fp,
			   *fp1;
	int			sourdist,
				lindist;
	int			num_located_routers;

	fp = fopen(logfile, "r");
	if (outputfile[0] != 0)
		fp1 = fopen(outputfile, "w");
	else
		fp1 = NULL;

	initialize_trrecord();

	while (get_next_record(fp) == 1)
	{
		process_trrecord_lindist(source, &sourdist, &lindist);
		num_located_routers = 0;
		for (i = 0; i < numhops_record; i++)
		{
			if (trrecord.location[i][0] != 0)
				num_located_routers++;
		}

		if (fp1 != NULL)
		{
			fprintf(fp1, "\n%s %d %d %d %d\n\n", trrecord.dest, sourdist, lindist, numhops_record, num_located_routers);
			for (i = 0; i < numhops_record; i++)
				fprintf(fp1, "%d %s %s %d %d\n", i + 1, trrecord.trace_label[i], trrecord.location[i], trrecord.sourdist[i], trrecord.sourlindist[i]);
		}
		else
		{
			printf("\n%s %d %d %d %d\n\n", trrecord.dest, sourdist, lindist, numhops_record, num_located_routers);
			for (i = 0; i < numhops_record; i++)
				printf("%d %s %s %d %d\n", i + 1, trrecord.trace_label[i], trrecord.location[i], trrecord.sourdist[i], trrecord.sourlindist[i]);
		}

		initialize_trrecord();
	}

	fclose(fp);
	if (fp1 != NULL)
		fclose(fp1);

	return;
}




void
process_log_hotpotato(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE])
{

	int			i,
				j;
	FILE	   *fp,
			   *fp1;
	int			sourdist,
				lindist;
	int			ispdist;
	char		net[STRINGSIZE];
	char		presnet[STRINGSIZE];
	char		setloc[STRINGSIZE];
	int			prevlindist,
				netdist;
	int			start,
				end;
	int			num_located_routers;
	float		l1,
				l2;

	fp = fopen(logfile, "r");
	if (outputfile[0] != 0)
		fp1 = fopen(outputfile, "w");
	else
		fp1 = NULL;

	initialize_trrecord();

	while (get_next_record(fp) == 1)
	{
		process_trrecord_lindist(source, &sourdist, &lindist);
		num_located_routers = 0;

		for (i = 0; i < numhops_record; i++)
		{
			if (trrecord.location[i][0] != 0)
				num_located_routers++;
			get_networkname(trrecord.trace_label[i], net);
			strcpy(trrecord.networkname[i], net);
		}

		prevlindist = 0;
		for (i = 0; i < numhops_record; i++)
		{
			if (trrecord.sourlindist[i] != -1)
				prevlindist = trrecord.sourlindist[i];
			else
				trrecord.sourlindist[i] = prevlindist;
		}

		strcpy(setloc, source);
		for (i = 0; i < numhops_record; i++)
		{
			if (trrecord.location[i][0] != 0)
				strcpy(setloc, trrecord.location[i]);
			else
				strcpy(trrecord.location[i], setloc);
			get_latlong(trrecord.location[i], &l1, &l2);
			trrecord.lat[i] = l1;
			trrecord.lon[i] = l2;
		}


		if (fp1 != NULL)
		{
			j = 0;
			presnet[0] = 0;
			while (j < numhops_record && presnet[0] == 0)
			{
				if (trrecord.networkname[j][0] != 0)
					strcpy(presnet, trrecord.networkname[j]);
				j++;
			}
			if (j == numhops_record)
			{
				initialize_trrecord();
				continue;
			}
			fprintf(fp1, "\n%s %d %d %d %d\n\n", trrecord.dest, sourdist, lindist, numhops_record, num_located_routers);
			start = 0;
			end = 1;
			while (end < numhops_record)
			{
				if (trrecord.networkname[end][0] == 0 || strcmp(presnet, trrecord.networkname[end]) == 0)
					end++;
				else if (strcmp(trrecord.networkname[end], presnet) != 0)
				{
					netdist = trrecord.sourlindist[end - 1] - trrecord.sourlindist[start];
					ispdist = get_distance(trrecord.lat[start], trrecord.lat[end - 1], trrecord.lon[start], trrecord.lon[end - 1]);
					fprintf(fp1, "%s %d %d\n", presnet, netdist, ispdist);
					start = end - 1;
					strcpy(presnet, trrecord.networkname[end]);
					end++;
				}
			}
			netdist = trrecord.sourlindist[end - 1] - trrecord.sourlindist[start];
			ispdist = get_distance(trrecord.lat[start], trrecord.lat[end - 1], trrecord.lon[start], trrecord.lon[end - 1]);
			fprintf(fp1, "%s %d %d\n", presnet, netdist, ispdist);
		}
		else
		{
			j = 0;
			presnet[0] = 0;
			while (j < numhops_record && presnet[0] == 0)
			{
				if (trrecord.networkname[j][0] != 0)
					strcpy(presnet, trrecord.networkname[j]);
				j++;
			}
			if (j == numhops_record)
			{
				initialize_trrecord();
				continue;
			}

			printf("\n%s %d %d %d %d\n\n", trrecord.dest, sourdist, lindist, numhops_record, num_located_routers);
			start = 0;
			end = 1;
			while (end < numhops_record)
			{
				if (trrecord.networkname[end][0] == 0)
					end++;
				else if (strcmp(trrecord.networkname[end], presnet) == 0)
					end++;
				else if (strcmp(trrecord.networkname[end], presnet) != 0)
				{
					netdist = trrecord.sourlindist[end - 1] - trrecord.sourlindist[start];
					ispdist = get_distance(trrecord.lat[start], trrecord.lat[end - 1], trrecord.lon[start], trrecord.lon[end - 1]);
					printf("%s %d %d\n", presnet, netdist, ispdist);
					start = end - 1;
					strcpy(presnet, trrecord.networkname[end]);
					end++;
				}
			}
			netdist = trrecord.sourlindist[end - 1] - trrecord.sourlindist[start];
			ispdist = get_distance(trrecord.lat[start], trrecord.lat[end - 1], trrecord.lon[start], trrecord.lon[end - 1]);
			printf("%s %d %d\n", presnet, netdist, ispdist);
		}

		initialize_trrecord();
	}

	fclose(fp);
	if (fp1 != NULL)
		fclose(fp1);

	return;
}




void
process_log_cityadjgraph(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE])
{

	int			i,
				j;
	FILE	   *fp,
			   *fp1;
	int			sourdist,
				lindist,
				numlocs;
	char		loc[NUMHOPS][STRINGSIZE];
	char		labels[NUMHOPS][STRINGSIZE];

	for (i = 0; i < NUMHOPS; i++)
		loc[i][0] = 0;

	fp = fopen(logfile, "r");
	if (outputfile[0] != 0)
		fp1 = fopen(outputfile, "w");
	else
		fp1 = NULL;

	initialize_trrecord();

	while (get_next_record(fp) == 1)
	{
		process_trrecord_lindist(source, &sourdist, &lindist);

		j = 0;
		for (i = 0; i < numhops_record; i++)
		{
			if (trrecord.location[i][0] != 0)
			{
				strcpy(loc[j], trrecord.location[i]);
				strcpy(labels[j], trrecord.trace_label[i]);
				j++;
			}
		}
		numlocs = j;

		if (fp1 != NULL)
		{
			if (numlocs > 1)
				for (i = 0; i < numlocs - 1; i++)
					fprintf(fp1, "%s %s %s %s\n", labels[i], labels[i + 1], loc[i], loc[i + 1]);
		}
		else
		{
			if (numlocs > 1)
				for (i = 0; i < numlocs - 1; i++)
					printf("%s %s %s %s\n", labels[i], labels[i + 1], loc[i], loc[i + 1]);
		}

		initialize_trrecord();
	}

	fclose(fp);
	if (fp1 != NULL)
		fclose(fp1);

	return;
}


void
process_log_cityclosuregraph(char logfile[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE])
{

	int			i,
				j;
	FILE	   *fp,
			   *fp1;
	int			sourdist,
				lindist,
				numlocs;
	char		loc[NUMHOPS][STRINGSIZE];
	char		labels[NUMHOPS][STRINGSIZE];
	char		prevloc[STRINGSIZE];

	for (i = 0; i < NUMHOPS; i++)
		loc[i][0] = 0;

	fp = fopen(logfile, "r");
	if (outputfile[0] != 0)
		fp1 = fopen(outputfile, "w");
	else
		fp1 = NULL;

	initialize_trrecord();

	while (get_next_record(fp) == 1)
	{
		process_trrecord_lindist(source, &sourdist, &lindist);

		prevloc[0] = 0;
		j = 0;
		for (i = 0; i < numhops_record; i++)
		{
			if (trrecord.location[i][0] != 0 && strcmp(trrecord.location[i], prevloc) != 0)
			{
				strcpy(loc[j], trrecord.location[i]);
				strcpy(labels[j], trrecord.trace_label[i]);
				strcpy(prevloc, trrecord.location[i]);
				j++;
			}
		}
		numlocs = j;

		if (fp1 != NULL)
		{
			if (numlocs > 1)
				for (i = 0; i < numlocs - 1; i++)
					for (j = i + 1; j < numlocs; j++)
						fprintf(fp1, "%s %s %s %s\n", labels[i], labels[j], loc[i], loc[j]);
		}
		else
		{
			if (numlocs > 1)
				for (i = 0; i < numlocs - 1; i++)
					for (j = i + 1; j < numlocs; j++)
						printf("%s %s %s %s\n", labels[i], labels[j], loc[i], loc[j]);
		}

		initialize_trrecord();
	}

	fclose(fp);
	if (fp1 != NULL)
		fclose(fp1);

	return;
}


void
process_log_routerlabels(char logfile[STRINGSIZE], char outputfile[STRINGSIZE])
{

	FILE	   *fp,
			   *fp1;
	char		loc[STRINGSIZE],
				label[STRINGSIZE],
				label1[STRINGSIZE];

	fp = fopen(logfile, "r");
	if (outputfile[0] != 0)
		fp1 = fopen(outputfile, "w");
	else
		fp1 = NULL;


	while (fgets(label, STRINGSIZE, fp) != NULL)
	{
		sscanf(label, "%s\n", label1);
		get_location(label1, loc);
		if (fp1 != NULL)
			fprintf(fp1, "%s %s\n", label1, loc);
		else
			printf("%s %s\n", label1, loc);
	}
	fclose(fp);
	if (fp1 != NULL)
		fclose(fp1);

	return;
}


void
process_commonpath_rec(int num)
{

	int			i,
				j;
	int			n1,
				n2;
	int			commonhops;
	int			commonhopgeodist;
	int			commongeodist;
	int			commonispgeodist;
	int			ld1,
				ld2;
	int			lastcommonhop;
	char		loc[STRINGSIZE],
				isp[STRINGSIZE];
	int bool;

	n1 = numhops_record;
	n2 = rec[num].numhops;

	i = n1 - 1;
	j = n2 - 1;
	commonhops = 0;

	lastcommonhop = -1;

	while (i >= 0 && j >= 0)
	{
		if (strcmp(rec[num].trace_label[j], trrecord.trace_label[i]) == 0 && (strcmp(rec[num].location[j], trrecord.location[i]) == 0) && (strcmp(rec[num].networkname[j], trrecord.networkname[i]) == 0))
			lastcommonhop = i;
		i--;
		j--;
	}

	if (lastcommonhop == -1)
	{
		commonhops = 0;
		commonhopgeodist = 0;
	}
	else
	{
		commonhops = n1 - lastcommonhop;
		commonhopgeodist = trrecord.sourlindist[n1 - 1] - trrecord.sourlindist[lastcommonhop];
	}


	i = n1 - 1;
	j = n2 - 1;
	lastcommonhop = -1;
	bool = 1;

	while (i >= 0 && j >= 0 && bool)
	{
		if (strcmp(rec[num].location[j], trrecord.location[i]) == 0)
		{
			lastcommonhop = i;
			strcpy(loc, trrecord.location[i]);
			while (j >= 0 && strcmp(rec[num].location[j], loc) == 0)
				j--;
			while (i >= 0 && strcmp(trrecord.location[i], loc) == 0)
				i--;
		}
		else
			bool = 0;
	}

	if (lastcommonhop == -1)
		commongeodist = 0;
	else
		commongeodist = trrecord.sourlindist[n1 - 1] - trrecord.sourlindist[lastcommonhop];

	i = n1 - 1;
	j = n2 - 1;
	bool = 1;

	lastcommonhop = -1;

	while (i >= 0 && j >= 0 && bool)
	{
		if ((strcmp(rec[num].location[j], trrecord.location[i]) == 0) && (strcmp(rec[num].networkname[j], trrecord.networkname[i]) == 0))
		{
			lastcommonhop = i;
			strcpy(loc, trrecord.location[i]);
			strcpy(isp, trrecord.networkname[i]);

			while (j >= 0 && strcmp(rec[num].location[j], loc) == 0 && strcmp(rec[num].networkname[j], isp) == 0)
				j--;
			while (i >= 0 && strcmp(trrecord.location[i], loc) == 0 && strcmp(trrecord.networkname[i], isp) == 0)
				i--;
		}
		else
			bool = 0;
	}

	if (lastcommonhop == -1)
		commonispgeodist = 0;
	else
		commonispgeodist = trrecord.sourlindist[n1 - 1] - trrecord.sourlindist[lastcommonhop];

	ld2 = trrecord.sourlindist[numhops_record - 1];
	ld1 = rec[num].sourlindist[rec[num].numhops - 1];

	printf("%s %d %d %d %d %d %d %d %d\n", rec[num].dest, rec[num].numhops, numhops_record, commonhops, ld1, ld2, commonhopgeodist, commonispgeodist, commongeodist);

	return;

}



void
process_commonpath(char logfile1[STRINGSIZE], char logfile2[STRINGSIZE], char source1[STRINGSIZE], char source2[STRINGSIZE])
{

	FILE	   *fp;
	int			sourdist,
				lindist;
	int			num;
	char		loc[STRINGSIZE];
	int			i;
	int			prevlindist;
	char		prevnet[STRINGSIZE];
	char		net[STRINGSIZE];

	load_logfile(logfile1, source1);
	fp = fopen(logfile2, "r");

	initialize_trrecord();

	while (get_next_record(fp) == 1)
	{
		process_trrecord_lindist(source2, &sourdist, &lindist);
		num = get_recordnum(trrecord.dest);

		if (num != -1)
		{

			strcpy(loc, source2);

			for (i = 0; i < numhops_record; i++)
			{
				if (trrecord.location[i][0] != 0)
					strcpy(loc, trrecord.location[i]);
				else
					strcpy(trrecord.location[i], loc);
			}

			prevlindist = 0;
			for (i = 0; i < numhops_record; i++)
			{
				if (trrecord.sourlindist[i] != -1)
					prevlindist = trrecord.sourlindist[i];
				else
					trrecord.sourlindist[i] = prevlindist;
			}

			prevnet[0] = 0;
			for (i = 0; i < numhops_record; i++)
			{
				get_networkname(trrecord.trace_label[i], net);
				if (net[0] != 0)
				{
					strcpy(trrecord.networkname[i], net);
					strcpy(prevnet, net);
				}
				else
					strcpy(trrecord.networkname[i], prevnet);
			}

			/* for(i=0;i<numhops_record;i++) */
			/* printf("%s\n",trrecord.networkname[i]); */

			/* for(i=0;i<rec[num].numhops;i++) */
			/* printf("%s\n",rec[num].networkname[i]); */

			process_commonpath_rec(num);
		}
		initialize_trrecord();
	}

	fclose(fp);

	return;
}

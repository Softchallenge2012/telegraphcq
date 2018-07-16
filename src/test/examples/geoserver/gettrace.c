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
 *
 * This code obtains the traceroute data for a given destination. Using the
 * routerlabels in traceroute, it obtains the location of the intermediary
 * routers and their corresponding locations.
 *
 */

#include "loadcodes.h"
#include "parselabel.h"
#include "gettrace.h"
#include "caldist.h"

void
initialize_trinfo()
{

	int			i;

	for (i = 0; i < NUMHOPS; i++)
	{
		trinfo[i].label[0] = 0;
		trinfo[i].location[0] = 0;
		trinfo[i].networkname[0] = 0;
		trinfo[i].lat = -1.0;
		trinfo[i].lon = -181.0;
		trinfo[i].distsour = 0;
		trinfo[i].lindistsour = 0;
		trinfo[i].delay = -1;
	}

	return;
}


int
extract_delay(char del[STRINGSIZE])
{

	int			i;
	char		str[STRINGSIZE];

	i = 0;

	while (del[i] != '\0' && del[i] != '.')
	{
		if (!isdigit(del[i]))
		{
			printf("Improper Entry for delay=%s\n", del);
			return -INFINITY;
		}
		str[i] = del[i];
		i++;
	}

	str[i] = 0;

	return atoi(str);
}


int
is_in_usa(char location[STRINGSIZE])
{

	char	   *s;

	if (location[0] == 0 || strlen(location) < 2)
		return -1;

	s = location + strlen(location);
	s = s - 3;

	if (*s == '+')
		return 1;

	return 0;
}

/*
 * Performs very simple delay-based verification. There is scope for
 * improving this function.
 */

void
delay_verify()
{

	char		prev[STRINGSIZE];
	int			prevdelay = NEGINFINITE;
	int			delaydiff;
	int			i;

	prev[0] = 0;

	for (i = 0; i < numhops; i++)
	{
		if (trinfo[i].location[0] != 0)
		{
			if (prev[0] == 0)
			{
				strcpy(prev, trinfo[i].location);
				prevdelay = trinfo[i].delay;
			}
			else
			{
				if (is_in_usa(prev) && !is_in_usa(trinfo[i].location))
				{
					delaydiff = trinfo[i].delay - prevdelay;
					if (delaydiff < USEUROPE_DELAYDIFF)
						trinfo[i].location[0] = 0;
					else
					{
						strcpy(prev, trinfo[i].location);
						prevdelay = trinfo[i].delay;
					}
				}
				else if (!is_in_usa(prev) && is_in_usa(trinfo[i].location))
				{
					delaydiff = trinfo[i].delay - prevdelay;
					if (delaydiff < USEUROPE_DELAYDIFF)
						trinfo[i].location[0] = 0;
					else
					{
						strcpy(prev, trinfo[i].location);
						prevdelay = trinfo[i].delay;
					}
				}
				else
				{
					strcpy(prev, trinfo[i].location);
					prevdelay = trinfo[i].delay;
				}
			}
		}
	}

	return;
}



/* Performs traceroute and obtains the locations of the routers in the path */

void
trace(char ipaddr[STRINGSIZE], char outputfile[STRINGSIZE])
{

	FILE	   *fp;
	char		str[STRINGSIZE],
				str1[STRINGSIZE];
	int			i,
				delay;
	char		del[STRINGSIZE];

	initialize_trinfo();

	/* strcpy(str,"traceroute "); */
	strcpy(str, "traceroute -m 20 -w 2 ");
	strcat(str, ipaddr);
	strcat(str, " > t1\n");
    fprintf(stderr, "Executing '%s'\n", str);
	system(str);
	system("cat t1 | awk '{ if ($2!=\"to\" && $2!=\"*\") { printf(\"%s \",$2); bool=0; for(i=2;i<=NF;i++) { if ($i==\"ms\" && !bool) { printf(\"%s\\n\",$(i-1)); bool=1; } }} }' >t2\n");

	fp = fopen("t2", "r");
	i = 0;
	while (fgets(str, STRINGSIZE, fp) != NULL && i < NUMHOPS)
	{
		sscanf(str, "%s %s\n", str1, del);
		delay = extract_delay(del);
		trinfo[i].delay = delay;
		strcpy(trinfo[i].label, str1);
		i++;
	}

	if (i == NUMHOPS)
	{
		printf("Too many hops in traceroute\n");
		exit(1);
	}

	fclose(fp);
	numhops = i;
	system("rm t1\n");
	system("rm t2\n");


	for (i = 0; i < numhops; i++)
	{
		if (trinfo[i].label[0] != '*')
			get_location(trinfo[i].label, trinfo[i].location);
		else
			trinfo[i].location[0] = 0;
	}

	delay_verify();

	if (outputfile[0] == 0)
	{
		for (i = 0; i < numhops; i++)
			printf("%d %s %s\n", i + 1, trinfo[i].label, trinfo[i].location);
	}
	else
	{
		fp = fopen(outputfile, "w");
		for (i = 0; i < numhops; i++)
			fprintf(fp, "%d %s %s\n", i + 1, trinfo[i].label, trinfo[i].location);
		fclose(fp);
	}

	return;
}


void
trace_finalloc(char ipaddr[STRINGSIZE], char loc[STRINGSIZE])
{

	FILE	   *fp;
	char		str[STRINGSIZE],
				str1[STRINGSIZE];
	int			i,
				delay;
	char		del[STRINGSIZE];

	initialize_trinfo();

	/* strcpy(str,"traceroute "); */
	strcpy(str, "traceroute -m 20 -w 2 ");

	strcat(str, ipaddr);
	strcat(str, " > t1\n");
    fprintf(stderr, "Executing '%s'\n", str);
	system(str);
	system("cat t1 | awk '{ if ($2!=\"to\" && $2!=\"*\") { printf(\"%s \",$2); bool=0; for(i=2;i<=NF;i++) { if ($i==\"ms\" && !bool) { printf(\"%s\\n\",$(i-1)); bool=1; } }} }' >t2\n");

	fp = fopen("t2", "r");
	i = 0;
	while (fgets(str, STRINGSIZE, fp) != NULL && i < NUMHOPS)
	{
		sscanf(str, "%s %s\n", str1, del);
		delay = extract_delay(del);
		trinfo[i].delay = delay;
		strcpy(trinfo[i].label, str1);
		i++;
	}

	if (i == NUMHOPS)
	{
		printf("Too many hops in traceroute\n");
		exit(1);
	}

	fclose(fp);
	numhops = i;
	system("rm t1\n");
	system("rm t2\n");


	for (i = 0; i < numhops; i++)
	{
		if (trinfo[i].label[0] != '*')
			get_location(trinfo[i].label, trinfo[i].location);
		else
			trinfo[i].location[0] = 0;
	}

	delay_verify();

	loc[0] = 0;
	for (i = 0; i < numhops; i++)
		if (trinfo[i].location[0] != 0)
			strcpy(loc, trinfo[i].location);

	return;
}


/*
 * Trace the route and also calculate hop-hop geographic distance
 * information. Also calculates the "linearized distance" of the route.
 */

void
trace_lindist(char ipaddr[STRINGSIZE], char outputfile[STRINGSIZE], char source[STRINGSIZE])
{

	FILE	   *fp;
	char		str[STRINGSIZE],
				str1[STRINGSIZE];
	float		latprev,
				lonprev;
	float		latnew,
				lonnew;
	float		latsour,
				lonsour;
	int			lindist,
				dist;
	int			i,
				delay;
	char		del[STRINGSIZE];

	initialize_trinfo();

	strcpy(str, "traceroute ");
	strcat(str, ipaddr);
	strcat(str, " > t1\n");
	system(str);
	system("cat t1 | awk '{ if ($2!=\"to\" && $2!=\"*\") { printf(\"%s \",$2); bool=0; for(i=2;i<=NF;i++) { if ($i==\"ms\" && !bool) { printf(\"%s\\n\",$(i-1)); bool=1; } }} }' >t2\n");

	fp = fopen("t2", "r");
	i = 0;
	while (fgets(str, STRINGSIZE, fp) != NULL && i < NUMHOPS)
	{
		sscanf(str, "%s %s\n", str1, del);
		delay = extract_delay(del);
		trinfo[i].delay = delay;
		strcpy(trinfo[i].label, str1);
		i++;
	}

	if (i == NUMHOPS)
	{
		printf("Too many hops in traceroute\n");
		exit(1);
	}

	fclose(fp);
	numhops = i;
	system("rm t1\n");
	system("rm t2\n");


	for (i = 0; i < numhops; i++)
	{
		if (trinfo[i].label[0] != '*')
			get_location(trinfo[i].label, trinfo[i].location);
		else
			trinfo[i].location[0] = 0;
	}

	delay_verify();

	lindist = 0;

	if (source[0] != 0)
	{
		strcpy(trinfo[0].location, source);
		get_latlong(trinfo[0].location, &latprev, &lonprev);
		latsour = latprev;
		lonsour = lonprev;
		if (lonsour == -181.0)
		{
			printf("Improper Source Entry\n");
			exit(1);
		}
		trinfo[0].lat = latsour;
		trinfo[0].lon = lonsour;

		for (i = 1; i < numhops; i++)
		{
			if (trinfo[i].location[0] != 0)
			{
				get_latlong(trinfo[i].location, &latnew, &lonnew);
				if (lonnew == -181.0)
				{
					printf("Check Location database for Location %s\n",
						   trinfo[i].location);
					exit(1);
				}
				trinfo[i].lat = latnew;
				trinfo[i].lon = lonnew;
				trinfo[i].distsour = get_distance(latsour, latnew, lonsour, lonnew);
				dist = get_distance(latprev, latnew, lonprev, lonnew);
				lindist += dist;
				trinfo[i].lindistsour = lindist;
				latprev = latnew;
				lonprev = lonnew;
			}
			else
			{
				trinfo[i].distsour = -1;
				trinfo[i].lindistsour = -1;
			}
		}
	}
	else
	{
		printf("Please enter source info\n");
		exit(1);
	}


	if (outputfile[0] == 0)
	{
		for (i = 0; i < numhops; i++)
			printf("%d %s %s %d %d\n", i + 1, trinfo[i].label, trinfo[i].location, trinfo[i].distsour, trinfo[i].lindistsour);
	}
	else
	{
		fp = fopen(outputfile, "w");
		for (i = 0; i < numhops; i++)
			fprintf(fp, "%d %s %s %d %d\n", i + 1, trinfo[i].label, trinfo[i].location, trinfo[i].distsour, trinfo[i].lindistsour);
		fclose(fp);
	}

	return;
}

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
 * This C code contains just the main function that determines which geotrack
 * option to use.
 *
 */

#include "loadcodes.h"
#include "parselabel.h"
#include "gettrace.h"
#include "tracelog.h"
#include "caldist.h"
#define LATLONG_PATH ""

int
main(int argc, char **argv)
{

	char		loc[STRINGSIZE];
	char		label[STRINGSIZE];
	char		ipaddr[STRINGSIZE];
	char		interfacefile[STRINGSIZE];
	char		outputfile[STRINGSIZE];
	char		logfile[STRINGSIZE];
	char		source[STRINGSIZE];
	char		compsource[STRINGSIZE];
	char		complogfile[STRINGSIZE];

	char		citifile[STRINGSIZE];
	int			i;

	i = 1;

	if (strcmp(argv[i], "-i") == 0)
	{
		strcpy(interfacefile, argv[i + 1]);
		i = i + 2;
	}
	else
		strcpy(interfacefile, "interface");

	source[0] = 0;
	if (strcmp(argv[i], "-s") == 0)
	{
		strcpy(source, argv[i + 1]);
		i = i + 2;
	}

	load_interface(interfacefile);
	load_allcodes(interfacefile);
	sprintf(citifile, "%slatlong.new", LATLONG_PATH);

	load_cities(citifile);


	if (strcmp(argv[i], "-h") == 0)
	{
		printf("GeoTrack Syntax: ./geotrack [-i <interfacefile>] [-s <source>] [-h | -r <routerlabel> | -t [-o outputfile] ipaddr | -l <ipaddr> | -f [-o outputfile] logfile | -lindist [-o outputfile] ipaddr | -lindistlog [-o outputfile] logfile] | -hotpotato [-o outputfile] logfile] | -cityadj [-o outputfile] logfile] | -cityclosure [-o outputfile] logfile]\n ");
		return 1;
	}

	if (strcmp(argv[i], "-r") == 0)
	{
		strcpy(label, argv[i + 1]);
		get_location(label, loc);
		printf("location=%s\n", loc);
	}
	else if (strcmp(argv[i], "-t") == 0)
	{
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;

		strcpy(ipaddr, argv[i + 1]);
		trace(ipaddr, outputfile);
	}
	else if (strcmp(argv[i], "-l") == 0)
	{
		strcpy(ipaddr, argv[i + 1]);
		trace_finalloc(ipaddr, loc);
		printf("location=%s\n", loc);
	}
	else if (strcmp(argv[i], "-f") == 0)
	{
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(logfile, argv[i + 1]);
		process_log(logfile, outputfile);
	}
	else if (strcmp(argv[i], "-lindist") == 0)
	{
		if (source[0] == 0)
		{
			printf("Source location is not entered\n");
			exit(1);
		}
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(ipaddr, argv[i + 1]);
		trace_lindist(ipaddr, outputfile, source);
	}
	else if (strcmp(argv[i], "-lindistlog") == 0)
	{
		if (source[0] == 0)
		{
			printf("Source location is not entered\n");
			exit(1);
		}
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(logfile, argv[i + 1]);
		process_log_lindist(logfile, outputfile, source);
	}
	else if (strcmp(argv[i], "-hotpotato") == 0)
	{
		if (source[0] == 0)
		{
			printf("Source location is not entered\n");
			exit(1);
		}
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(logfile, argv[i + 1]);
		process_log_hotpotato(logfile, outputfile, source);
	}
	else if (strcmp(argv[i], "-cityadj") == 0)
	{
		if (source[0] == 0)
		{
			printf("Source location is not entered\n");
			exit(1);
		}
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(logfile, argv[i + 1]);
		process_log_cityadjgraph(logfile, outputfile, source);
	}
	else if (strcmp(argv[i], "-cityclosure") == 0)
	{
		if (source[0] == 0)
		{
			printf("Source location is not entered\n");
			exit(1);
		}
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(logfile, argv[i + 1]);
		process_log_cityclosuregraph(logfile, outputfile, source);
	}
	else if (strcmp(argv[i], "-routerlog") == 0)
	{
		if (strcmp(argv[i + 1], "-o") == 0)
		{
			strcpy(outputfile, argv[i + 2]);
			i = i + 2;
		}
		else
			outputfile[0] = 0;
		strcpy(logfile, argv[i + 1]);
		process_log_routerlabels(logfile, outputfile);
	}
	else if (strcmp(argv[i], "-commonpath") == 0)
	{
		if (source[0] == 0)
		{
			printf("Source location is not entered\n");
			exit(1);
		}
		strcpy(logfile, argv[i + 1]);
		strcpy(compsource, argv[i + 2]);
		strcpy(complogfile, argv[i + 3]);
		process_commonpath(logfile, complogfile, source, compsource);
	}
	else
	{
		printf("Use one of these options\n");
		printf("GeoTrack Syntax: ./geotrack [-i <interfacefile>] [-s <source>] [-h | -r <routerlabel> | -t [-o outputfile] ipaddr | -l <ipaddr> | -f [-o outputfile] logfile | -lindist [-o outputfile] ipaddr | -lindistlog [-o outputfile] logfile] | -hotpotato [-o outputfile] logfile] | -cityadj [-o outputfile] logfile] | -cityclosure [-o outputfile] logfile] | -routerlog [-o outputfile] logfile]\n ");
		exit(1);
	}

	return 1;
}

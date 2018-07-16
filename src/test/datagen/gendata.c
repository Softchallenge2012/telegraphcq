/*-------------------------------------------------------------------------
 *
 * gendata.c
 *
 * Author: Sailesh Krishnamurthy
 *
 *
 * Portions Copyright (c) 2003, Regents of the University of California
 *
 * $Id: gendata.c,v 1.3 2003/03/20 20:01:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SEED 1

void
usage(char *pgm)
{
	printf("Usage: %s -o outputfile -n numcols -r numrows -b beginrange -e endrange \n", pgm);
}
int
main(int argc, char *const argv[])
{
	char	   *optstring = "o:n:b:e:r:";
	int			opt;
	int			numCols = 0;
	int			numRows = 0;
	int			beginRange = 0;
	int			endRange = 0;
	FILE	   *ofile = NULL;
	int			i,
				j;
	int			range;

	while ((opt = getopt(argc, argv, optstring)) != EOF)
	{
		switch (opt)
		{
			case 'o':
				if ((ofile = fopen(optarg, "w")) == NULL)
				{
					printf("Couldn't open file %s for reading \n", optarg);
					return 1;
				}
				break;

			case 'n':
				numCols = atoi(optarg);
				break;

			case 'r':
				numRows = atoi(optarg);
				break;

			case 'b':
				beginRange = atoi(optarg);
				break;

			case 'e':
				endRange = atoi(optarg);
				break;

			default:
				usage(argv[0]);
				return 1;
		}
	}
	range = endRange - beginRange;
	srand(SEED);

	for (i = 0; i < numRows; i++)
	{
		struct timeval tv;
		struct timezone tz;
		char	   *ct;
		char		str[20];
		int			l;

		gettimeofday(&tv, &tz);
		ct = asctime(localtime(&(tv.tv_sec)));
		strcpy(str, ct);
		l = strlen(str);
		str[l - 1] = '\0';
		fprintf(ofile, "%s ", str);
		sleep(1);
		for (j = 0; j < numCols; j++)
		{
			fprintf(ofile, ",%d", (rand() % range) + beginRange);
/*			if (j != (numCols - 1))  */
/*			{ */
/*				fprintf(ofile,"\t"); */
/*			} */
		}
		fprintf(ofile, "\n");
	}

	fflush(ofile);
	fclose(ofile);
}

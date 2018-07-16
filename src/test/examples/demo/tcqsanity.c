/*-------------------------------------------------------------------------
 *
 * tcqsanity.c
 *
 *    A sanity test case
 *
 * Portions Copyright (c) 2003, Regents of the University of California 
 *
 *-------------------------------------------------------------------------
 */
#include "stdio.h"
#include "stdlib.h"

/* constant flags */
#define SCHEDULED 1
#define WAITING 0
#define NO_ORDER -1
#define NOT_STARTED -1
#define PARENT 999
#define CHILD 888

/* constant commands */
#define RUN_STREAMS "./streamers.sh"
#define NUM_MAJOR_SLEEPS 10
#define SLEEP_COMMAND "sleep 3"
#define SHUTDOWN_SLEEP "sleep 10"
#define TCQ_START_COMMAND "pg_ctl start -l results/tcq-logfile -o \"-t sample -u garrettj -G\""
#define BEGIN_PSQL_COMMAND "psql -C -i 1 -d sample -o "
#define OUTPUT_BASE "results/sanity."
#define KILL_BASE_COMMAND "kill -9 "

/* Constant Query Information */
#define NUM_QUERIES 6
#define MAX_CONCURRENT_QUERIES 32

/* constant array with all the queries */
char	   *queries[] = {"select dst, count(*) as c from network.tcpdump as st group by dst window st ['5 seconds'];",
	"select dport, count(*) as c from network.tcpdump as st group by dport window st ['5 seconds'];",
	"SELECT ts.stationid, tm.speed, ts.latitude,ts.longitude FROM traffic.measurements AS tm, traffic.stations AS ts WHERE tm.stationid = ts.stationid;",
	"SELECT tcqtime, incidentID, location, city, description, tinc.highway, tinc.street, latitude, longitude FROM traffic.incidents AS tinc, traffic.intersections AS tint WHERE (tinc.highway||tinc.street) = (tint.highway||tint.street);",
	"SELECT gs.latitude, gs.longitude, count(*) as connections FROM network.geoserver gs, network.tcpdump td WHERE gs.addr = td.dst GROUP BY gs.latitude, gs.longitude WINDOW td ['5 seconds'];",
"select top.opdesc as stem, count(*) as c from tcq_queues as tq, tcq_operators as top where tq.opid = top.opid and top.opkind = 'F' and tq.qkind = 'I' and tq.kind = 'E' group by stem window tq ['5 seconds'];"};

/* main program */
int
main(int argc, char *argv[])
{

	/* Variable Declarations */
	FILE	   *log_file,
			   *seed_file;
	int			num_iterations,
				id,
				cur_iteration,
				cur_query;
	int			num_queries_this_iteration,
				i,
				cur_rand;
	int		   *seeds,
			   *query_pids,
			   *query_order;
	char	   *command_string,
			   *iteration_number,
			   *query_number,
			   *pid_number;

	/* make sure there is one argument */
	if (argc != 2)
	{
		fprintf(stderr, "Error: incorrect arguments!\nUsage: tcqsanity <config_file>\n");
		return -1;
	}

	/* remove any old results */
	system("rm results/*");

	/* open config file (file of seeds) and log file */
	log_file = fopen("results/tcqsanity_logfile", "w");
	seed_file = fopen(argv[1], "r");

	/* make sure both files opened correctly */
	if (log_file <= 0)
		fprintf(stderr, "Error opening logfile for writing. Aborting...\n");
	if (seed_file <= 0)
		fprintf(stderr, "Error opening config file for reading. Aborting...\n");

	/* read the number of iterations out of the config file */
	fscanf(seed_file, "%d\n", &num_iterations);

	/* create an array of the seed for each iteration */
	seeds = (int *) malloc(sizeof(int) * num_iterations);

	id = PARENT;				/* is this a parent or a child? */

	/* read in all the seed values */
	for (cur_iteration = 0; cur_iteration < num_iterations; cur_iteration++)
		fscanf(seed_file, "%d\n", &seeds[cur_iteration]);

	/* Start logfile */
	fprintf(log_file, "TCQSanity Run Started\n");
	fprintf(log_file, "Number of iterations: %d\n", num_iterations);

	/* for (each iteration) */
	for (cur_iteration = 0; cur_iteration < num_iterations; cur_iteration++)
	{

		/* Begin an Iteration */
		fprintf(log_file, "Iteration %d Beginning...\n", cur_iteration);
		fprintf(log_file, "Seed: %d\n", seeds[cur_iteration]);

		/* seed with this iteration's seed value */
		srand(seeds[cur_iteration]);
		num_queries_this_iteration = (rand() % MAX_CONCURRENT_QUERIES) + 1;

		fprintf(log_file, "Executing %d queries this iteration.", num_queries_this_iteration);

		query_pids = (int *) malloc(sizeof(int) * num_queries_this_iteration);
		query_order = (int *) malloc(sizeof(int) * num_queries_this_iteration);

		/* 'zero' out data structures */
		for (cur_query = 0; cur_query < num_queries_this_iteration; cur_query++)
		{
			query_pids[cur_query] = NOT_STARTED;
			query_order[cur_query] = NO_ORDER;
		}

		fprintf(log_file, "Query Order: ");
		/* get the order of the queries for this run ... */
		for (cur_query = 0; cur_query < num_queries_this_iteration; cur_query++)
		{
			cur_rand = rand() % (NUM_QUERIES);
			query_order[cur_query] = cur_rand;
			fprintf(log_file, "%d ", cur_rand);
		}
		fprintf(log_file, "\n");

		/* start up TCQ */
		fprintf(log_file, "Starting TCQ\n");
		system("pg_ctl stop");
		system("sleep 5");
		system(TCQ_START_COMMAND);

		/* wait to attach streams */
		system("sleep 15");
		fprintf(log_file, "Streaming streams\n");
		system(RUN_STREAMS);

		/* wait to run queries */
		system("sleep 15");

		fprintf(log_file, "Beginning to fork queries...\n");
		/* cur_query now refers to query ORDER... */
		for (cur_query = 0; cur_query < num_queries_this_iteration; cur_query++)
		{

			if (id == PARENT)
			{
				/* this is a parent, fork away */
				command_string = (char *) malloc(500);
				iteration_number = (char *) malloc(10);
				query_number = (char *) malloc(10);
				sprintf(iteration_number, "%d", cur_iteration);
				sprintf(query_number, "%d", cur_query);

				/* form the command-line string */
				strcpy(command_string, BEGIN_PSQL_COMMAND);
				strcat(command_string, OUTPUT_BASE);
				strcat(command_string, iteration_number);
				strcat(command_string, ".");
				strcat(command_string, query_number);
				strcat(command_string, ".out ");
				strcat(command_string, "-c \"");
				strcat(command_string, queries[query_order[cur_query]]);
				strcat(command_string, "\"");

				free(iteration_number);
				free(query_number);

				/* fork off a child to run the command */
				query_pids[cur_query] = fork();
				if (query_pids[cur_query] == 0)
				{
					/* child, run a query */
					system(command_string);
					id = CHILD;
					printf("Executed command string: %s\n", command_string);
					free(command_string);
					/* children have nothing else to do... */
					return 0;
				}
				else if (query_pids[cur_query] > 0)
				{
					/* parent */
					fprintf(log_file, "Starting Query %d (Query Text %d) with PID %d.\n", cur_query, query_order[cur_query], query_pids[cur_query]);
					free(command_string);
					continue;
				}
				else
				{
					/* error */
					fprintf(stderr, "Error with Fork...Aborting...\n");
					exit(-1);
				}

			}
			else
			{
				/* don't want children forking repeats... */
				break;
			}
		}

		/* sleep and let queries run.... */
		printf("Sleeping for a while to let the queries run....relax....\n");
		fprintf(log_file, "Sleeping with command: %s\n", SLEEP_COMMAND);
		system(SLEEP_COMMAND);

		if (id == CHILD)
		{
			/* just make sure no children get here */
			fprintf(stderr, "ERROR, child should not get this far...\n");
		}
		else
		{						/* id == PARENT */
			fprintf(log_file, "Begin Termination of Queries\n");
			for (cur_query = 0; cur_query < num_queries_this_iteration; cur_query++)
			{
				/*
				 * for(each started query) kill -9 <its pid>
				 */
				command_string = (char *) malloc(100);
				pid_number = (char *) malloc(10);
				sprintf(pid_number, "%d", query_pids[cur_query]);
				fprintf(log_file, "Killing Query Started: %d (Query Text %d) at PID %d\n", cur_query, query_order[cur_query], query_pids[cur_query]);
				strcpy(command_string, KILL_BASE_COMMAND);
				strcat(command_string, pid_number);
				printf("Killing with command: %s\n", command_string);
				system(command_string);
				free(command_string);
				free(pid_number);
			}
		}

		/* shutting down TCQ */
		fprintf(log_file, "Shutting down all TCQ stuff with stop.sh script\n");
		system("./stop.sh > /dev/null");

		printf("Sleeping waiting for things to shutdown correctly....\n");
		fprintf(log_file, "Sleeping waiting for everything to get shutdown correctly\n");
		system(SHUTDOWN_SLEEP);

		fprintf(log_file, "Done with iteration %d.\n", cur_iteration);
	}

	/* done with all tests */
	fprintf(log_file, "Done with all tests! Good luck!\n");
	return 0;
}

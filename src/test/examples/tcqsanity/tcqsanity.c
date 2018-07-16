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
#include <stdio.h>
#include <stdlib.h>

int got_expected_results(int query_num);
void sort_output(int query_num);
void remove_blank_lines_from_output(int query_num);
int check_output_against_expected(int query_num, const char *expected_file);

/* #define Definitions */
#define SHORT_SLEEP_COMMAND "sleep 2"
#define LONG_SLEEP_COMMAND "sleep 20" 
#define TCQ_STOP "./tcq_stop"
/* this needs to be a script that pushes fake data into ALL the streams that the demo setup uses */
#define START_STREAMS "./push_streams.sh"
#define BOUNCE_SERVER "./auto-bounce"

#define PSQL_BASE_COMMAND "psql -t -C -d sample -o "
#define KILL_BASE_COMMAND "kill -9 "
#define DIFF_BASE_COMMAND "diff -b "
#define SORT_BASE_COMMAND "sort -n " 
#define DIFF_OUTPUT "diff.out"
#define OUTPUT_BASE_DIR "output/"
#define OUTPUT_BASE "output."
#define EXPECTED_RESULTS_DIR "expected/"
#define EXPECTED_BASE "expected."

#define TRUE 1
#define FALSE 0

/* To visually inspect results, don't diff since, window semantics will be lost.*/
#define DIFF (1 << 0)
#define SORT (1 << 1)


/* Constant Declarations */
/* what are the queries in the system ? */
const char *queries[] = {
  "select dst, count(*) as c from network.tcpdump as st [range by '5 seconds' slide by '1 seconds' start at '2003-06-06 18:50:20'] group by dst;",
  "select ts.stationid, tm.speed, ts.latitude, ts.longitude from traffic.measurements tm, traffic.stations ts where tm.stationid = ts.stationid;",
  "select tcqtime, incidentID, location, city, description, tinc.highway, tinc.street, latitude, longitude from traffic.incidents tinc, traffic.intersections tint where (tinc.highway||tinc.street) = (tint.highway||tint.street);",
  "select tm.stationid, ts.name, ts.highway, ts.mile, AVG(tm.speed) from traffic.measurements tm [range by '1 second' slide by '1 seconds' start at '2003-06-06 20:50:22'], traffic.stations ts where ts.stationid = tm.stationid group by tm.stationid, ts.name, ts.highway, ts.mile HAVING tm.stationid > 8000 ;",
  "WITH numbers.nat as (select * from numbers.nat_src) (select n.i from numbers.nat n);",
  "select sum(n.i) from numbers.nat n [range by '3 seconds' slide by '1 seconds' start at '2004-12-03 16:22:00'];",
  "WITH store.counter as (select tag_id, tcqtime from store.rfid where loc_id='Counter') store.door as (select tag_id, tcqtime from store.rfid where loc_id='Door') (select D.* from store.counter C, store.door D partition by tag_id event notprecedes(D, C, '5 min'));",
  "WITH library.books1 as (Select * from library.books) (select B1.* from library.books B1, library.books1 B2 partition by book_name, shelf_id event notfollows(B1, B2, '2 min'));",
  "select count(*), wtime(*) as c from network.tcpdump as st [range by '2 second' slide by '2 second' start at '2003-06-06 18:50:20'];",
  "WITH  numbers.prime as (select * from numbers.prime_src) (select n.i from numbers.nat n, numbers.prime p where n.i = p.i);",
};

/* what are the queries that should be run? */
const int run_queries[] = {
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
};

/* give the query/test a name so it can be identified */							       
const char *query_names[] = {
  "windowed selection of network traffic grouped by destination ip",
  "join between measurement stream and sensor location table",
  "join between incident stream and highway/street location table",
  "join between measurement stream and sensor location table with sliding window aggregate",
  "simple WITH clause for a natural number stream",
  "Aggregate operators on sliding windows ",
  "event-driven shoplifting detection query. has two WITH clauses, the PARTITION BY clause, and the NOTPRECEDES event operator",
  "event-driven query for spotting misplaced books, has one WITH clause, the PARTITION BY clause, and the NOTFOLLOWS event operator",
  "Sliding-window selection with wtime()",
  "Stream-stream join with WITH clauses",
};

/* should the results of the query be diff'ed? */
const int query_result_flags[] = {
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
  (DIFF | SORT),
};

const int num_queries = 10; /* must be length of array of queries */

int num_run_queries = 0;

/* Main tcqsanity program */
int main(int argc, char *argv[]) 
{

  /* Variable Declarations */
  int i, pid[num_queries];
  char *query_command, *query_number;
  char *kill_command, *pid_number;
  int passed_tests = 0;
  char *pgdir;
  char *bounce_server;
  char *tcq_stop;

  if (argc > 1) {
	pgdir = argv[1];
  }
  else {
	pgdir = "data";
  }

  /* bounce the server to remove the old stream connections */
  bounce_server = (char *) malloc(strlen(BOUNCE_SERVER) + 5 + strlen(pgdir));
  sprintf(bounce_server, "%s %s ", BOUNCE_SERVER, pgdir);
  system(bounce_server);

  /* Fork a child to issue each continuous query */
  for(i=0; i<num_queries; i++) {
    /* malloc strings */
	if (! run_queries[i]) {
	  continue;
	}
	num_run_queries++;

    query_command = (char*) malloc(sizeof(char)*100 + strlen(queries[i]));
    query_number = (char*) malloc(sizeof(char)*10);
    
    /* turn the query's number into a string */
    sprintf(query_number, "%d", i);

    /* create the command string to issue the query */
    strcpy(query_command, PSQL_BASE_COMMAND);
	strcat(query_command, OUTPUT_BASE_DIR);
	strcat(query_command, OUTPUT_BASE);
    strcat(query_command, query_number);
    strcat(query_command, " -t -c \"");
    strcat(query_command, queries[i]);
    strcat(query_command, "\"");

	sleep(5);
	
	printf("\n\t%d. Beginning to fork off a child psql process for query....\n\n", i + 1);
	
    /* fork a child process */
    if ((pid[i] = fork()) == 0) {
      /* child process */

      /* issue the query */
	  printf("Executing: %s \n" ,query_command);
      system(query_command);

      /* after it gets killed, release memory and return */
      free(query_command);
      free(query_number);
      return 0;
    }
    else if (pid[i] > 0) {
      /* parent process */
      free(query_command);
      free(query_number);
    }
    else {
      /* error with fork */
      fprintf(stderr, "Error forking children process...quitting test...\n");
      return -1;
    }
  }

  /* sleep to allow queries to register correctly */
  printf("Sleeping: %s\n", LONG_SLEEP_COMMAND);
  system(LONG_SLEEP_COMMAND);
  printf("Done sleeping \n");
      
  /* Now start the streaming data... */
  system(START_STREAMS);
    
  /* only parent should get here... */
  /* sleep and let the queries run on all the sample data */
  printf("Sleeping: %s\n", LONG_SLEEP_COMMAND);
  system(LONG_SLEEP_COMMAND);
  printf("Done sleeping \n");

  printf("Starting to kill all the psql processes\n");

  /* kill off all the psql processes */
#if 0
  for(i=0; i<num_queries; i++) {
	if (! run_queries[i]) {
	  continue;
	}
    pid_number = (char*) malloc(sizeof(char)*10);
    kill_command = (char*) malloc(sizeof(char)*20);
    sprintf(pid_number, "%d", pid[i]);
    strcpy(kill_command, KILL_BASE_COMMAND);
    strcat(kill_command, pid_number);
		/* printf("Killing something ! (%s) \n", kill_command); */
    /* system(kill_command); */
    free(kill_command);
    free(pid_number);
  }
#endif
  
  printf("Printing TelegraphCQ status and then attempting a shutdown...\n");
  tcq_stop = (char *) malloc(strlen(TCQ_STOP) + 5 + strlen(pgdir));
  sprintf(tcq_stop,"%s %s ", TCQ_STOP, pgdir);
  printf("Running: %s \n", tcq_stop);
  system(tcq_stop);
  printf("Starting to diff output with expected output\n********************\n");

  /* now do a comparison of the outputs and indicate any diff-erences */
  for(i=0; i<num_queries; i++) {
	if (! run_queries[i]) {
		//printf("Skipping query %d\n", i);
	  continue;
	}
	
    if (query_result_flags[i] & DIFF) {
			//		printf("Diffing query %d\n", i);
      if (got_expected_results(i)) {
		/* test passed */
		passed_tests++;
		//		printf("Passed test %d\n", i);
		printf("TelegraphCQ PASSED integrity test: %s\n", query_names[i]);
      }
      else {
		/* test failed, warn the user */
		printf("TelegraphCQ FAILED integrity test: %s\n", query_names[i]);
      }
    }
    else {
      /* make it look like this passed */
      passed_tests++;
      printf("Skipping the diff of this query since there isn't a way to get expected results...\n");

    }
  }

  printf("**********************************************************************\nTCQSanity has finished running %d tests.\n", num_run_queries);
  printf("\n\tTests Passed: %d \t Tests Failed: %d \n", passed_tests, num_run_queries-passed_tests);
  printf("**********************************************************************\n");

  if (passed_tests == num_run_queries) {
    printf("This version of TCQ passed all base tcqsanity tests.  Congratulations!\n");
  }
  else {
    printf("Get back to work! TCQ failed %d tcqsanity tests! Fix it...\n", num_run_queries-passed_tests);
  }

  /* remove old files */
      
  return 0;
}

int got_expected_results(int query_num)
{
  char *expected_file = (char*) malloc(strlen(EXPECTED_BASE) +
								  ((int)log10(query_num)) + 1);
  int check;
  
  sprintf(expected_file, "%s%d",EXPECTED_BASE,query_num);

	//	printf("Expected output in file %s\n", expected_file);
  if (query_result_flags[query_num] & SORT) 
  {
		//		printf("Sorting expected output %d\n", query_num);
	  sort_output(query_num);
  }

	//	printf("Removing blanks from expected output %d\n", query_num);
  remove_blank_lines_from_output(query_num);

	//	printf("Comparing against expected output for query %d\n", query_num);
  check = check_output_against_expected(query_num, expected_file);

  free(expected_file);

  return check;
}

void sort_output(int query_num)
{
	char *query_number, *sort_command, *move_command;

	query_number = (char*)malloc(sizeof(char)*5);
	sprintf(query_number, "%d", query_num);

	sort_command = (char*)malloc(sizeof(char)*100);
	strcpy(sort_command, SORT_BASE_COMMAND);
	strcat(sort_command, OUTPUT_BASE_DIR);
	strcat(sort_command, OUTPUT_BASE);
	strcat(sort_command, query_number);
	strcat(sort_command, " > ");
	strcat(sort_command, OUTPUT_BASE_DIR);
	strcat(sort_command, "tmp");

	system(sort_command);

	move_command = (char*)malloc(sizeof(char)*100);
	strcpy(move_command, "mv ");
	strcat(move_command, OUTPUT_BASE_DIR);
	strcat(move_command, "tmp ");
	strcat(move_command, OUTPUT_BASE_DIR);
	strcat(move_command, OUTPUT_BASE);
	strcat(move_command, query_number);

	system(move_command);

	free(sort_command);
	free(move_command);
	free(query_number);
}

void remove_blank_lines_from_output(int query_num)
{
	char *query_number, *sed_command, *move_command;
		
	query_number = (char*)malloc(sizeof(char)*5);
	sprintf(query_number, "%d", query_num);

	sed_command = (char*)malloc(sizeof(char)*100);
	strcpy(sed_command, "cat ");
	strcat(sed_command, OUTPUT_BASE_DIR);
	strcat(sed_command, OUTPUT_BASE);
	strcat(sed_command, query_number);
	strcat(sed_command, " | sed \"/^$/d\" > ");
	strcat(sed_command, OUTPUT_BASE_DIR);
	strcat(sed_command, OUTPUT_BASE);
	strcat(sed_command, query_number);
	strcat(sed_command, ".sed");

	system(sed_command);

	move_command = (char*)malloc(sizeof(char)*100);
	strcpy(move_command, "mv ");
	strcat(move_command, OUTPUT_BASE_DIR);
	strcat(move_command, OUTPUT_BASE);
	strcat(move_command, query_number);
	strcat(move_command, ".sed ");
	strcat(move_command, OUTPUT_BASE_DIR);
	strcat(move_command, OUTPUT_BASE);
	strcat(move_command, query_number);

	system(move_command);

	free(move_command);
	free(query_number);
	free(sed_command);
}

int check_output_against_expected(int query_num, const char *expected_file) 
{
	char *diff_command, *query_number, *diff_filename;
	char input;
	FILE *diff_file;
	
	query_number = (char*)malloc(sizeof(char)*5);
	sprintf(query_number, "%d", query_num);

	//	printf("Query num %s\n", query_number);
	diff_filename = (char*)malloc(sizeof(char)*100);
	strcpy(diff_filename, OUTPUT_BASE_DIR);
	strcat(diff_filename, DIFF_OUTPUT);
	strcat(diff_filename, ".");
	strcat(diff_filename, query_number);
	
	//#	printf("Diff filename %s\n", diff_filename);
	diff_command = (char*)malloc(sizeof(char)*100);
	strcpy(diff_command, DIFF_BASE_COMMAND);
	strcat(diff_command, OUTPUT_BASE_DIR);
	strcat(diff_command, OUTPUT_BASE);
	strcat(diff_command, query_number);
	strcat(diff_command, " ");
	strcat(diff_command, EXPECTED_RESULTS_DIR);
	strcat(diff_command, expected_file);
	strcat(diff_command, " > ");
	strcat(diff_command, diff_filename);

	//	printf("Running diff command %s\n", diff_command);
	system(diff_command);

	diff_file = fopen(diff_filename, "r");
	
	if (diff_file == NULL) 
	{
		fprintf(stderr, "Could not open diff file: %s\n", diff_filename);
	}
	
	free(diff_filename);
	free(diff_command);
	free(query_number);

	if (diff_file == NULL) 
	{
		return FALSE;
	}
			
	//	printf("Checking if diff file empty\n");
	input = fgetc(diff_file);

	if (input == EOF) 
	{
		//		printf("diff file empty\n");
		return TRUE;
	}
	else
	{
		//		printf("diff file NOT! empty\n");
		return FALSE;
	}
	
}

#include <stdio.h>
#include <unistd.h>

#include "postgres.h"			/* general Postgres declarations */
#include "fmgr.h"				/* for argument/result macros */

#include "telegraphcq/tcq_usleep.h"

#define RAND_RANGE 100

bool expensive(int, int, int);
bool cheap(int, int, int);
bool setseed(int);
int delay(int , int);

static
int rand_in_range()
{
    return (int) (RAND_RANGE * 1.0 * rand() / (RAND_MAX + 1.0));
}

/* The 's' can be used to 'bind' this predicate to a particular relation. 
 * It is basically ignored. */
bool
expensive(int a, int b, int s)
{
    static int prev_s = -1;

    bool predicate_value;

    tcq_usleep(a); /* Note - microseconds */

    if(prev_s == s)
    {
        predicate_value = true; /* Lets say the predicates are very heavily positively correlated. 
                                   If the first one evaluates to true, all will. If the 
                                   first one evaluates to false, we will never call this with the
                                   same key. */
    }
    else
    {
        prev_s = s;
        predicate_value = (rand_in_range() < b);
    }

    return predicate_value;
}

/** 'a' is supposed to be the attribute to be returned, and 'b' is the 
 *  delay. */
int
delay(int a, int b)
{
    return a;
}

bool setseed(int seed)
{
	srand(seed);
	return true;
}

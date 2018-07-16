#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define SEED 1

void
usage(char *pgm)
{
	printf("Usage: %s -i incidents -s sensors \n", pgm);
}

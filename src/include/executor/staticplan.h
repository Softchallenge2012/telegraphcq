
#ifndef STATICPLAN_H
#define STATICPLAN_H

#include "postgres.h"

#include "c.h"

#include "nodes/plannodes.h"
#include "nodes/parsenodes.h"

extern int 		numStaticQueries;
extern int 		numStaticSources;
extern int      numStaticModules;

extern int 		whichquery;
extern int 		StaticDriver;
extern bool     UseStaticPlan;
extern int UseStaticPlanNumber;
extern Eddy 	*StaticEddy;

typedef struct StaticPlan
{
	void (*fsp_fp)(Eddy*, Query*);
	int num_queries;
	
} StaticPlan;

extern StaticPlan MyStaticPlans[];

#endif /* STATICPLAN_H */

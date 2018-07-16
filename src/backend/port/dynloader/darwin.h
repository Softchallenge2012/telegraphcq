/* $Header: /project/eecs/db/cvsroot/postgres/src/backend/port/dynloader/darwin.h,v 1.2 2004/03/24 08:11:04 chungwu Exp $ */

#include "fmgr.h"

void	   *pg_dlopen(char *filename);
PGFunction	pg_dlsym(void *handle, char *funcname);
void		pg_dlclose(void *handle);
char	   *pg_dlerror(void);

/* $Id: mbprint.h,v 1.1.1.2 2003/04/17 23:01:17 sailesh Exp $ */
#ifndef MBPRINT_H
#define MBPRINT_H

#include "mb/pg_wchar.h"

pg_wchar	utf2ucs(const unsigned char *c);

unsigned char *mbvalidate(unsigned char *pwcs);

int			pg_wcswidth(unsigned char *pwcs, size_t len);

#endif   /* MBPRINT_H */

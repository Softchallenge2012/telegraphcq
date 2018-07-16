/*
 * strmsource.h
 *
 * Routines for reading CSV tuples directly off a socket in the backend.
 *
 * Author: Fred Reiss <phred@cs.berkeley.edu>
 */


#ifndef STRMSOURCE_H
#define STRMSOURCE_H

#include "nodes/execnodes.h"

extern int fillReadBuf(SockState *ss);
extern char * getNextLine(SockState *ss);
extern bool lossyWrite(SockState *ss, const char *buf, size_t buflen);


#endif /* STRMSOURCE_H */



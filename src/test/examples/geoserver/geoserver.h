#ifndef GEOSERVER_H
#define GEOSERVER_H

#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbg_lib.h"
#include "hashcache.h"

#define GEOSERVER_DEFAULT_PORT 6754
#define LISTEN_QUEUE_SIZE 5
#define INC_MESSAGE_LENGTH 16
#define OUT_MESSAGE_LENGTH 99
#define TEMP_FILE "gt.temp"
#define LATLONG_FILE "latlong.hc"
#define CACHE_FILE "cache.hc"
#define GT_OUTPUT_SIZE 40
#define INVALID_LOCATION "BERMUDA+TR"
#define DONE_MESSAGE "DONE WITH PROBE\n"
#define INVALID_STATE " "
#define USE_PERSISTANT_CACHE TRUE
#define LATLONG_BASE "latlong.data"
#define MAX_CITY_SIZE 30
#define WS_DELIMETER '+'

#endif

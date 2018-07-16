/*-------------------------------------------------------------------------
 *
 * debug.h
 *
 * Copyright (c) 2003, Regents of the University of California
 *
 * $Id: debug.h,v 1.2 2003/07/17 05:18:51 sailesh Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef DEBUG_H_
#define DEBUG_H_

extern void debug_log_message(char *str);

extern void debug_print_times(void);

extern void init_recording_output_times();
extern void finish_recording_output_times();
extern void record_output_time();

#endif   /* DEBUG_H */

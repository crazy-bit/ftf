/**
 * File:	ftf_util.h
 * Purpose:	Provide utilitarian functions
 * Author: 	yo-yo
 * Date: 	2013.03.28
 */

#ifndef _FTF_UTIL_H_
#define _FTF_UTIL_H_

#include <syslog.h>

#define FNAME __FUNCTION__

// ouput deubg info, change LOG_LEVEL to manage these infos
void ftf_debug(int level, const char *fname, const char *fmt, ...);

// set signal action
void *signal_set(int signo, void (*func)(int));

// set terminal to raw mode
void set_terminal();

// restore into original mode
void restore_terminal();
#endif


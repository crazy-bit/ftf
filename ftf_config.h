/**
 * File:	ftf_config.h
 * Purpose:	Define paramters that can be configed
 * Author:	yoyo
 * Date:	2013.04.02
 */

#ifndef _FTF_CONFIG_H
#define _FTF_CONFIG_H

// log info level
#define FTF_LOG_LEVEL	LOG_DEBUG

// maximum number of commands that can matched
#define FTF_MAX_MATCH_SIZE	100

// maximum number of command arguments
#define FTF_MAX_CMD_NUM	10

// when interacting, the prompt like "FTF>"
#define FTF_PROMPT	"FTF"

// enter may be '\n' or '\r\n'
#define FTF_ENTER	"\r\n"

// how much number of command ouput in one line
#define FTF_LINE_NUM	2

#endif


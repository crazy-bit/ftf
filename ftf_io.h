/**
 * File:	ftf_io.h
 * Purpose:	FTF I/O functions
 * Author:	yoyo
 * Date:	2013.03.28
 */

#ifndef _FTF_IO_H
#define _FTF_IO_H

#include "ftf_vty.h"

/**
 * Cursor back one grid
 *
 * @return nonoe
 */
void ftf_back_one();

/**
 * Put a char into terminal
 *
 * @return none
 */
void ftf_put_one(char c);

/**
 * Read input
 *
 * @param vty ftf_vty
 * @return none
 */
void ftf_read(struct ftf_vty *vty);

/**
 * Write to terminal
 *
 * @return none
 */
void ftf_write(const char *format, ...);

/**
 * Clear one line in terminal
 *
 * @param vty ftf_vty
 * @return none
 */
void ftf_clear_line(struct ftf_vty *vty);

#endif


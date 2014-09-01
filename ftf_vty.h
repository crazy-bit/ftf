/*
 * File:	ftf_vty.h
 * Purpose: Including vty struct definition and operations
 * Author: 	yo-yo
 * Date: 	2013-03-28
 */

#ifndef _FTF_VTY_H
#define _FTF_VTY_H

#include "ftf.h"

/**
 * Initial ftf_vty
 *
 * @return initialized ftf_vty
 */
struct ftf_vty *ftf_vty_init();

/**
 * Destory ftf_vty, including allocating history
 *
 * @param vty ftf_vty to destory
 * @return none
 */
void ftf_vty_deinit(struct ftf_vty *vty);

/**
 * Add one command line into ftf_vty history
 *
 * @param vty ftf_vty to add history, command is in vty->buffer
 * @return none
 */
void ftf_vty_add_history(struct ftf_vty *vty);


#endif


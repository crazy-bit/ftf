/**
 * File:	ftf_cmd.h
 * Purpose:	Including ftf_cmd struct definition and operations
 * Author:	yoyo
 * Date:	2013.04.02
 */

#ifndef _FTF_CMD_H
#define _FTF_CMD_H

#include "ftf.h"
#include "ftf_vector.h"
#include "ftf_vty.h"


/**
 * A struct para_desc relative to One command parameter
 *
 * @param para command parameter, such as 'show' of 'show vlan'
 * @param desc command parameter reference
 */
struct para_desc {
	char *para;
	char *desc;
};

// command match type
enum CMD_MATCH_STATUS {
	CMD_NO_MATCH,
	CMD_FULL_MATCH,
	CMD_PART_MATCH,
	CMD_LIST_MATCH,
	CMD_ERR_AMBIGOUS,
	CMD_ERR_ARGU,
};

extern ftf_vector_t *cmd_vec;

/**
 * For input 'icmd_vec', find matched command,  matched string store in matched
 *
 * @param icmd_vec input command vector
 * @param vty ftf_vty
 * @param match store matched command
 * @param match_size matched command number
 * @param lcd_str matched command LCD string if have
 * @return match type, one of CMD_MATCH_STATUS
 */
int cmd_match_command(ftf_vector_t *icmd_vec, struct ftf_vty *vty, struct para_desc **match, int *match_size, char *lcd_str);

/**
 * For input 'icmd_vec', find complete matched command, matched string
 * store in matched
 *
 * @param icmd_vec input command vector
 * @param vty ftf_vty
 * @param doc store matched command document
 * @return 0 if success, otherwise return -1
 */
int cmd_complete_command(ftf_vector_t *icmd_vec, struct ftf_vty *vty, char **doc);


int cmd_execute_command(ftf_vector_t *icmd_vec, struct ftf_vty *vty);

/**
 * Initial command
 */
void cmd_init();

ftf_vector_t *cmd2vec(char *string, char *doc);

#endif


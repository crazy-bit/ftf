#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ftf_config.h"
#include "ftf_util.h"
#include "ftf_cmd.h"

// Global command vector, to store user installed commands
//ftf_vector_t *cmd_vec;

/*** ---------------- Auxiliary Function ------------------ ***/
// check if 'str' is unique in matchvec, matchvec->data point to
// a double array char array like char **
static int match_unique_string(struct para_desc **match, char *str, int size)
{
	int i;

	for (i = 0; i < size; i++) {
		if (match[i]->para != NULL && strcmp(match[i]->para, str) == 0)
			return 0;
	}
	return 1;
}

// turn a command into vector
ftf_vector_t *cmd2vec(char *string, char *doc)
{
	char *sp, *d_sp;	// parameter start point
	char *cp = string;	// parameter current point
	char *d_cp = doc;	// doc point
	char *token, *d_token;	// paramter token
	int len, d_len;
	ftf_vector_t *allvec;
	struct para_desc *desc;

	if(cp == NULL)
		return NULL;

	// split command string into paramters, and turn these paramters
	// into vector form
	allvec = ftf_vector_init(1);
	while (1)
	{
		// get next parameter from 'string'
		while(isspace((int) *cp) && *cp != '\0')
			cp++;
		if(*cp == '\0') 
			return allvec;

		sp = cp;
		while(!(isspace ((int) *cp) || *cp == '\r' || *cp == '\n') 
				&& *cp != '\0')
			cp++;
		len = cp - sp;
		token = malloc(len + 1);
		memcpy(token, sp, len);
		*(token + len) = '\0';

		// get next paramter info from 'doc'
		while(isspace((int)*d_cp) && *d_cp != '\0')
			d_cp++;
		if (*d_cp == '\0') 
			d_token = NULL;
		else {
				d_sp = d_cp;
			while(!(*d_cp == '\r' || *d_cp == '\n') && *d_cp != '\0')
				d_cp++;
			d_len = d_cp - d_sp;
			d_token = malloc(d_len + 1);
			memcpy(d_token, d_sp, d_len);
			*(d_token + d_len) = '\0';
		}

		// add new para_vector into command vector
		desc = calloc(1, sizeof(struct para_desc));
		desc->para = token;
		desc->desc = d_token;
		ftf_vector_insert(allvec, (void *)desc);
	}
}

// get LCD of matched command
int match_lcd(struct para_desc **match, int size)
{
	int i, j;
	int lcd = -1;
	char *s1, *s2;
	char c1, c2;

	if (size < 2)
		return 0;

	for (i = 1; i < size; i++) {
		s1 = match[i - 1]->para;
		s2 = match[i]->para;

		for (j = 0; (c1 = s1[j]) && (c2 = s2[j]); j++)
			if (c1 != c2)
				break;

		if (lcd < 0)
			lcd = j;
		else {
			if (lcd > j)
				lcd = j;
		}
	}

	return lcd;
}

static int cmd_filter_command(char *cmd, ftf_vector_t *v, int index)
{
	int i;
	struct cmd_elem *elem;
	struct para_desc *desc;

	// For each command, check the 'index'th parameter if it matches the 
	// 'index' paramter in command, set command vector which does not
	// match cmd NULL
	for (i = 0; i < ftf_vector_max(v); i++) { 
		if ((elem = ftf_vector_slot(v, i)) != NULL) {	
			if (index >= ftf_vector_max(elem->para_vec)) {
				ftf_vector_slot(v, i) = NULL;
				continue;
			}

			desc = (struct para_desc *)ftf_vector_slot(elem->para_vec, index);
			if(strncmp(cmd, desc->para, strlen(cmd)) != 0) 
				ftf_vector_slot(v, i) = NULL;
		}
	}
		
	return 0;
}



/*** ---------------- Interface Function ------------------ ***/
int cmd_match_command(ftf_vector_t *icmd_vec, struct ftf_vty *vty, 
		struct para_desc **match, int *match_size, char *lcd_str)
{
	int i;
	ftf_vector_t *cmd_vec_copy = ftf_vector_copy(cmd_vec);
	int isize = ftf_vector_max(icmd_vec) - 1;
	int size = 0;

	// Three steps to find matched commands in 'cmd_vec'
	// 1. for input command vector 'icmd_vec', check if it is matching cmd_vec
	// 2. store last matching command parameter in cmd_vec into match_vec
	// 3. according to match_vec, do no match, one match, part match, list match
	
	// Step 1
	for (i = 0; i < isize; i++) {
		char *ipara = ftf_vector_slot(icmd_vec, i);
		cmd_filter_command(ipara, cmd_vec_copy, i);
	}

	// Step 2
	// insert matched command word into match_vec, only insert the next word
	for(i = 0; i < ftf_vector_max(cmd_vec_copy); i++) {
		struct cmd_elem *elem;
		if((elem = ftf_vector_slot(cmd_vec_copy, i)) != NULL) {
			if (isize >= ftf_vector_max(elem->para_vec)) {
				ftf_vector_slot(cmd_vec_copy, i) = NULL;
				continue;
			}

			struct para_desc *desc = ftf_vector_slot(elem->para_vec, isize);
			char *str = ftf_vector_slot(icmd_vec, isize);

			if (str == NULL || strncmp(str, desc->para, strlen(str)) == 0) {
				if (match_unique_string(match, desc->para, size))
					match[size++] = desc;
			}

		}
	}
	ftf_vector_deinit(cmd_vec_copy, 0);	// free cmd_vec_copy, no longer use

	// Step 3
	// No command matched
	if (size == 0) {
		*match_size = size;
		return CMD_NO_MATCH;
	} 
	
	// Only one command matched
	if (size == 1) {
		*match_size = size;
		return CMD_FULL_MATCH;
	}
	
	// Make it sure last element is NULL
	if (ftf_vector_slot(icmd_vec, isize) != NULL) {
		int lcd = match_lcd(match, size);
		if(lcd) {
			int len = strlen(ftf_vector_slot(icmd_vec, isize));
			if (len < lcd) {
				memcpy(lcd_str, match[0]->para, lcd);
				lcd_str[lcd] = '\0';
				*match_size = size = 1;
				return CMD_PART_MATCH;
			}
		}
	}

	*match_size = size;
	return CMD_LIST_MATCH;
}

int cmd_complete_command(ftf_vector_t *icmd_vec, struct ftf_vty *vty, char **doc)
{
	int i;
	ftf_vector_t *cmd_vec_copy = ftf_vector_copy(cmd_vec);
	int match_num = 0;

	// Two steps to find matched commands in 'cmd_vec'
	// 1. for input command vector 'icmd_vec', check if it is matching cmd_vec
	// 2. check last matching command parameter in cmd_vec match cmd_vec
	
	*doc = NULL;
	// Step 1
	cmd_filter_command(ftf_vector_slot(icmd_vec, 0), cmd_vec_copy, 0);

	// Step 2
	// insert matched command word into match_vec, only insert the next word
	for(i = 0; i < ftf_vector_max(cmd_vec_copy); i++) {
		char *str;
		struct para_desc *desc;
		struct cmd_elem *elem;
		if((elem = ftf_vector_slot(cmd_vec_copy, i)) != NULL) {
			str = ftf_vector_slot(icmd_vec, 0);
			desc = ftf_vector_slot(elem->para_vec, 0);

			if (str != NULL && strncmp(str, desc->para, strlen(str)) == 0) {
				match_num++;
				*doc = elem->doc;
			}
		}
	}
	ftf_vector_deinit(cmd_vec_copy, 0);	// free cmd_vec_copy, no longer use

	if (match_num != 1)
		*doc = NULL;
	return 0;
}

int cmd_execute_command(ftf_vector_t *icmd_vec, struct ftf_vty *vty)
{
	int i, j;
	ftf_vector_t *cmd_vec_copy = ftf_vector_copy(cmd_vec);

	int argc;
	char *argv[FTF_MAX_CMD_NUM];
	struct cmd_elem *match_elem;
	int match_num = 0;

	// Two steps to find matched commands in 'cmd_vec'
	// 1. for input command vector 'icmd_vec', check if it is matching cmd_vec
	// 2. check last matching command parameter in cmd_vec match cmd_vec
	
	// Step 1
	cmd_filter_command(ftf_vector_slot(icmd_vec, 0), cmd_vec_copy, 0);

	// Step 2
	// insert matched command word into match_vec, only insert the next word
	for(i = 0; i < ftf_vector_max(cmd_vec_copy); i++) {
		char *str;
		int not_match = 0;
		struct para_desc *desc;
		struct cmd_elem *elem;
		if((elem = ftf_vector_slot(cmd_vec_copy, i)) != NULL) {	
			for (j = 0; j < ftf_vector_max(elem->para_vec); j++) {
				str = ftf_vector_slot(icmd_vec, j);
				desc = ftf_vector_slot(elem->para_vec, j);
				if (str == NULL || strcmp(str, desc->para) != 0) {
					not_match = 1;
					break;
				}
			}

			if (not_match == 0) {
				match_num++;
				match_elem = elem;
			}
		}
	}
	ftf_vector_deinit(cmd_vec_copy, 0);	// free cmd_vec_copy, no longer use

	// check if exactly match
	if (match_num != 1)
		return CMD_NO_MATCH;

	argc = 0;
	for (i = ftf_vector_max(match_elem->para_vec); i < ftf_vector_max(icmd_vec); i++) {
		argv[argc++] = ftf_vector_slot(icmd_vec, i);
		if (argc >= FTF_MAX_CMD_NUM)
			return CMD_ERR_ARGU;
	}

	// execute command
	(*match_elem->func)(match_elem, vty, argc, argv);
	return CMD_FULL_MATCH;
}


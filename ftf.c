/**
 * Purpose:	FTF means Function Test Frame, it provides a more effective and convenient way to Test
 * 			your APIs or functions. It provides visible terminal operations, that you can free from
 * 			writing a lot of test scripts, and FTF suggests you publish your APIs with FTF_XXX(your own
 * 			test frame), so that anyone can verify and understand your APIs easily.
 * Author:	yoyo
 * Date:	2012-03-26
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <assert.h>

#include "ftf_vty.h"
#include "ftf_cmd.h"
#include "ftf_vector.h"
#include "ftf_util.h"
#include "ftf_io.h"


struct ftf_vty *vty;
ftf_vector_t *cmd_vec;

void sigint(int sig)
{
	restore_terminal();
	ftf_vty_deinit(vty);
	printf("\n");
	exit(0);
}

int ftf_run()
{
	int ret;

	// set up signal
	signal_set(SIGINT, sigint);
	signal_set(SIGTERM, sigint);
	signal_set(SIGSEGV, sigint);
	
	// set terminal
	set_terminal();
	
	vty = ftf_vty_init();
	if (vty == NULL)
		exit(1);	

	ftf_write("%s>", vty->prompt);
	while (1) {
		ftf_read(vty);
	}

	// never reached
	restore_ternimal();
	free(vty);
	printf("\n");

	return 0;
}



/* ============================================================================ */
/*                              Cmd Element Operation							*/
/* ============================================================================ */
void install_element(struct cmd_elem *elem)
{
	if (cmd_vec == NULL) 
	{
		// if not allocated, then allocate now
		cmd_vec = ftf_vector_init(1);
	}

	ftf_vector_insert(cmd_vec, elem);
	elem->para_vec = cmd2vec(elem->string, elem->doc);
	elem->para_num = ftf_vector_max(elem->para_vec);
}


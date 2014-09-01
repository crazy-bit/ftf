/**
 * Purpose:	FTF means Function Test Frame, it provides a more effective and convenient way to Test
 * 			your APIs or functions. It provides visible terminal operations, that you can free from
 * 			writing a lot of test scripts, and FTF suggests you publish your APIs with FTF_XXX(your own
 * 			test frame), so that anyone can verify and understand your APIs easily.
 * 			Adding your APIs to test in api.c
 * Author:	yoyo
 * Date:	2012-03-26
 */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "ftf_cmd.h"
#include "ftf_vty.h"
#include "ftf_util.h"
#include "ftf_io.h"

struct ftf_vty *vty;

void sigint(int sig)
{
	restore_terminal();
	ftf_vty_deinit(vty);
	printf("\n");
	exit(0);
}

int main()
{
	// initial and install command
	cmd_init();
	
	// set terminal
	set_terminal();
	
	// set up signal
	signal_set(SIGINT, sigint);
	signal_set(SIGTERM, sigint);
	signal_set(SIGSEGV, sigint);
	
	vty = ftf_vty_init();
	if (vty == NULL)
		exit(1);	

	ftf_write("%s>", vty->prompt);
	while(1) {
		ftf_read(vty);
	}

	// never reached
	restore_terminal();
	ftf_vty_deinit(vty);
	printf("\n");

	return 0;
}

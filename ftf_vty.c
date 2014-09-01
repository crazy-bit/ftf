#include <stdlib.h>
#include <string.h>

#include "ftf_config.h"
#include "ftf_util.h"
#include "ftf_vty.h"

struct ftf_vty *ftf_vty_init()
{
	int i;
	struct ftf_vty *vty;

	vty = (struct ftf_vty *)calloc(1, sizeof(struct ftf_vty));
	if(vty == NULL) {
		ftf_debug(LOG_ERR, FNAME, "Not Enough Memory For vty%s", FTF_ENTER);
		return NULL;
	}

	memcpy(vty->prompt, FTF_PROMPT, sizeof(FTF_PROMPT));
	vty->prompt[3] = '\0';
	vty->buf_len = FTF_BUFFER_SIZE;
	vty->used_len = vty->cur_pos = 0;
	vty->hpos = vty->hindex = 0;

	for (i = 0; i < HISTORY_MAX_SIZE; i++) {
		vty->history[i] = NULL;
	}
	
	return vty;
}

void ftf_vty_deinit(struct ftf_vty *vty)
{
	int i;

	if (!vty)
		return;
	
	// free history
	for (i = 0; i < HISTORY_MAX_SIZE; i++) {
		if (vty->history[i] != NULL)
			free(vty->history[i]);
	}
	
	// free vty
	free(vty);
}

void ftf_vty_add_history(struct ftf_vty *vty)
{
	int idx =  vty->hindex ? vty->hindex - 1 : HISTORY_MAX_SIZE - 1;

	// if same as previous command, then ignore
	if (vty->history[idx] &&
			!strcmp(vty->buffer, vty->history[idx])) {
		vty->hpos = vty->hindex;
		return;
	}

	// insert into history tail
	if (vty->history[vty->hindex])
		free(vty->history[vty->hindex]);
	vty->history[vty->hindex] = strdup(vty->buffer);

	// History index rotation
	vty->hindex = (vty->hindex + 1) == HISTORY_MAX_SIZE ? 0 : vty->hindex + 1;
	vty->hpos = vty->hindex;
}


#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "ftf_config.h"
#include "getch.h"
#include "ftf_io.h"
#include "ftf_vector.h"
#include "ftf_cmd.h"
#include "ftf_resolve.h"

/* ------------------ Auxiliary Function ----------------- */
#define ANOTHER_LINE(i)	(((i) != 0) && ((i) % FTF_LINE_NUM == 0))

// insert a word into the tail of input buffer 
static void ftf_insert_word(struct ftf_vty *vty, const char *str)
{
	strcat(vty->buffer, str);
	vty->cur_pos += strlen(str);
	vty->used_len += strlen(str);
}

// delete the last word from input buffer
static void ftf_delete_word(struct ftf_vty *vty)
{
	int pos = strlen(vty->buffer);

	while (pos > 0 && vty->buffer[pos - 1] != ' ') {
		pos--;
	}

	vty->buffer[pos] = '\0';
	vty->cur_pos = strlen(vty->buffer);
	vty->used_len = strlen(vty->buffer);
}

static inline void free_matched(char **matched)
{
	int i;

	if (!matched)
		return;
	for (i = 0; matched[i] != NULL; i++)
		free(matched[i]);
	free(matched);
}



/* ------------------ Interface Function ----------------- */
int ftf_resolve(char c)
{
	int key_type = KEY_NOTCARE;	// default is not special key

	switch (c) {
		case FTF_KEY_ARROW1:
			if (getch() == FTF_KEY_ARROW2) {
				c = getch();	// get real key
				switch (c) {
					case FTF_KEY_UP:
						key_type = KEY_UP;
						break;
					case FTF_KEY_DOWN:
						key_type = KEY_DOWN;
						break;
					case FTF_KEY_RIGHT:
						key_type = KEY_RIGHT;
						break;
					case FTF_KEY_LEFT:
						key_type = KEY_LEFT;
						break;
					default:
						break;
				}
			}
			break;
		case FTF_KEY_BACKSPACE:
		case FTF_KEY_CTRL_H:
			key_type = KEY_DELETE;
			break;
		case '\t':
			key_type = KEY_TAB;
			break;
		case '\r':
		case '\n':
			key_type = KEY_ENTER;
			break;
		case '?':
			key_type = KEY_QUEST;
			break;
		default:
			break;
	}

	return key_type;
}


// resolve a whole line, including tab, enter, quest, up, down
void ftf_resolve_tab(struct ftf_vty *vty)
{
	int i;
	ftf_vector_t *v;
	struct para_desc *match[FTF_MAX_MATCH_SIZE];	// matched string
	int match_size = 0, match_type;
	char lcd_str[1024];	// if part match, then use this

	v = str2vec(vty->buffer);
	if (v == NULL) {
		v = ftf_vector_init(1);
		ftf_vector_insert(v, '\0');
	}

	if (isspace((int)vty->buffer[vty->used_len - 1]))
		ftf_vector_insert(v, '\0');
	
	match_type = cmd_match_command(v, vty, match, &match_size, lcd_str);
	ftf_vector_deinit(v, 1);

	ftf_write("%s", FTF_ENTER);
	switch (match_type) {
		case CMD_NO_MATCH:
			ftf_write("%s>", vty->prompt);
			ftf_write("%s", vty->buffer);
			break;
		case CMD_FULL_MATCH:
			ftf_delete_word(vty);
			ftf_insert_word(vty, match[0]->para);
			ftf_insert_word(vty, " ");
			ftf_write("%s>", vty->prompt);
			ftf_write("%s", vty->buffer);
			break;
		case CMD_PART_MATCH:
			ftf_delete_word(vty);
			ftf_insert_word(vty, lcd_str);
			ftf_write("%s>", vty->prompt);
			ftf_write("%s", vty->buffer);
			break;
		case CMD_LIST_MATCH:
			for (i = 0; i < match_size; i++) {
				if (ANOTHER_LINE(i))
					ftf_write("%s", FTF_ENTER);
				ftf_write("%-10s	", match[i]->para);
			}
			ftf_write("%s", FTF_ENTER);
			ftf_write("%s>", vty->prompt);
			ftf_write("%s", vty->buffer);
			break;
		default:
			break;
	}
}

void ftf_resolve_enter(struct ftf_vty *vty)
{
	int match_type = CMD_NO_MATCH;
	ftf_vector_t *v;

	v = str2vec(vty->buffer);
	if (v == NULL) {
		ftf_write("%s", FTF_ENTER);
		ftf_write("%s>", vty->prompt);
		return;
	}

	ftf_write("%s", FTF_ENTER);
	
	// do command
	match_type = cmd_execute_command(v, vty);
	// add executed command into history
	ftf_vty_add_history(vty);

	if (match_type == CMD_NO_MATCH) {
		ftf_write("Unknown Command");
		ftf_write("%s", FTF_ENTER);
	} else if (match_type == CMD_ERR_ARGU) {
		ftf_write("Too Many Arguments");
		ftf_write("%s", FTF_ENTER);
	}
	
	// ready for another command
	vty->cur_pos = vty->used_len = 0;
	memset(vty->buffer, 0, vty->buf_len);
	ftf_write("%s>", vty->prompt);
}

void ftf_resolve_quest(struct ftf_vty *vty)
{
	ftf_vector_t *v;
	char *doc = NULL;

	v = str2vec(vty->buffer);
	if (v == NULL) {
		v = ftf_vector_init(1);
		ftf_vector_insert(v, '\0');
	}

	cmd_complete_command(v, vty, &doc);
	ftf_vector_deinit(v, 1);

	ftf_write("%s", FTF_ENTER);
	if (doc) {
		ftf_write("%s", doc);
		ftf_write("%s>", vty->prompt);
		ftf_write("%s", vty->buffer);
	} else {
		ftf_write("Unknown Command%s", FTF_ENTER);
		ftf_write("%s>", vty->prompt);
		ftf_write("%s", vty->buffer);
	}
}

void ftf_resolve_up(struct ftf_vty *vty)
{
	int try_idx = vty->hpos == 0 ? (HISTORY_MAX_SIZE - 1) : vty->hpos - 1;
	
	// if no history
	if (vty->history[try_idx] == NULL)
		return;
	vty->hpos = try_idx;
	
	// print history command
	ftf_clear_line(vty);
	strcpy(vty->buffer, vty->history[vty->hpos]);
	vty->cur_pos = vty->used_len = strlen(vty->history[vty->hpos]);
	ftf_write("%s", vty->buffer);
}

void ftf_resolve_down(struct ftf_vty *vty)
{
	int try_idx = vty->hpos == (HISTORY_MAX_SIZE - 1) ? 0 : vty->hpos + 1;
	
	// if no history
	if (vty->history[try_idx] == NULL)
		return;
	vty->hpos = try_idx;
	
	// print history command
	ftf_clear_line(vty);
	strcpy(vty->buffer, vty->history[vty->hpos]);
	vty->cur_pos = vty->used_len = strlen(vty->history[vty->hpos]);
	ftf_write("%s", vty->buffer);
}

// handle in read buffer, including left, right, delete, insert
void ftf_resolve_left(struct ftf_vty *vty)
{
	// already at leftmost, cannot move more 
	if (vty->cur_pos <= 0)
		return;
	// move left one step
	vty->cur_pos--;
	ftf_back_one();
}

void ftf_resolve_right(struct ftf_vty *vty)
{
	// already at rightmost, cannot move more
	if (vty->cur_pos >= vty->used_len)
		return;
	// move right one step
	ftf_put_one(vty->buffer[vty->cur_pos]);
	vty->cur_pos++;
}

void ftf_resolve_delete(struct ftf_vty *vty)
{
	int i, size;
	
	// no more to delete
	if (vty->cur_pos <= 0)
		return;
	size = vty->used_len - vty->cur_pos;
	assert(size >= 0);

	// delete char
	vty->cur_pos--;
	vty->used_len--;
	ftf_back_one();
	
	// print left chars
	memmove(&vty->buffer[vty->cur_pos], &vty->buffer[vty->cur_pos + 1], size);
	vty->buffer[vty->used_len] = '\0';
	for (i = 0; i < size; i ++)
		ftf_put_one(vty->buffer[vty->cur_pos + i]);
	ftf_put_one(' ');
	for (i = 0; i < size + 1; i++)
		ftf_back_one();
}

void ftf_resolve_insert(struct ftf_vty *vty)
{
	int i, size;

	// no more to insert
	if (vty->used_len >= vty->buf_len)
		return;
	size = vty->used_len - vty->cur_pos;
	assert(size >= 0);

	memmove(&vty->buffer[vty->cur_pos + 1], &vty->buffer[vty->cur_pos], size);
	vty->buffer[vty->cur_pos] = vty->c;
	// print left chars, then back size
	for (i = 0; i < size + 1; i++)
		ftf_put_one(vty->buffer[vty->cur_pos + i]);
	for (i = 0; i < size; i++)
		ftf_back_one();
	
	vty->cur_pos++;
	vty->used_len++;
}

key_handler_t key_resolver[] = {
	// resolve a whole line
	{ KEY_TAB, 		ftf_resolve_tab },
	{ KEY_ENTER, 	ftf_resolve_enter },
	{ KEY_QUEST, 	ftf_resolve_quest },
	{ KEY_UP, 		ftf_resolve_up },
	{ KEY_DOWN, 	ftf_resolve_down },
	// resolve in read buffer
	{ KEY_LEFT, 	ftf_resolve_left },
	{ KEY_RIGHT, 	ftf_resolve_right },
	{ KEY_DELETE, 	ftf_resolve_delete },
	{ KEY_NOTCARE, 	ftf_resolve_insert },
};


/**
 * File:	ftf_resolve.h
 * Purpose:	To resolve raw input strings, handle special characters such as up
 * 			key, enter key....
 * Author:	yoyo
 * Date:	2013.03.27
 */

#ifndef _FTF_RESOLVE_H
#define _FTF_RESOLVE_H

#include "ftf_vty.h"

// definition of ASCII value of keys
// Arrow key is a sequenece started by 27, 91, XX
// Backspace value is 8, Ctrl+H equals backspace, so another value is 127
#define FTF_KEY_ARROW1	0x1b
#define FTF_KEY_ARROW2	0x5b
#define FTF_KEY_UP		0x41
#define FTF_KEY_DOWN 	0x42
#define FTF_KEY_RIGHT 	0x43
#define FTF_KEY_LEFT 	0x44
#define FTF_KEY_BACKSPACE 0x8
#define FTF_KEY_CTRL_H	(0x1f | 0x7f)

enum FTF_KEY_CODE {
	KEY_NONE = -1,
	KEY_TAB = 0,
	KEY_ENTER,
	KEY_QUEST,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT, 
	KEY_RIGHT,
	KEY_DELETE,
	KEY_NOTCARE = 8,
};

typedef struct key_handler {
	int key_value;
	void (*key_func)(struct ftf_vty *);
} key_handler_t;

/**
 * Resolve raw input characters, identify up, down, left, right keys, and
 * backspace key
 * 
 * @param c raw input characters
 * @return the type of the input character, FTF_KEY_CODE
 */
int ftf_resolve(char c);

void ftf_resolve_tab(struct ftf_vty *vty);
void ftf_resolve_enter(struct ftf_vty *vty);
void ftf_resolve_quest(struct ftf_vty *vty);
void ftf_resolve_up(struct ftf_vty *vty);
void ftf_resolve_down(struct ftf_vty *vty);
void ftf_resolve_left(struct ftf_vty *vty);
void ftf_resolve_right(struct ftf_vty *vty);
void ftf_resolve_delete(struct ftf_vty *vty);
void ftf_resolve_insert(struct ftf_vty *vty);

extern key_handler_t key_resolver[];

#endif


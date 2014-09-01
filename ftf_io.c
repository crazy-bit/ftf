#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#include "ftf_io.h"
#include "ftf_resolve.h"
#include "ftf_util.h"
#include "getch.h"

void ftf_back_one()
{
	printf("\b");
}

void ftf_put_one(char c)
{
	printf("%c", c);
}

void ftf_read(struct ftf_vty *vty)
{
	int key_type;

	while ((vty->c = getch()) != EOF) {
		// step 1: get input key type
		key_type = ftf_resolve(vty->c);

		if (key_type <= KEY_NONE || key_type > KEY_NOTCARE) {
			ftf_debug(LOG_WARNING, FNAME, "Unidentify Key Type, key = %d\n", key_type);
			continue;
		}
		// step 2: take actions according to input key
		key_resolver[key_type].key_func(vty);
	}
}

void ftf_write(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void ftf_clear_line(struct ftf_vty *vty)
{
	int size = vty->used_len - vty->cur_pos;

	assert(size >= 0);
	while (size--) {
		ftf_put_one(' ');
	}

	while (vty->used_len) {
		vty->used_len--;
		ftf_back_one();
		ftf_put_one(' ');
		ftf_back_one();
	}
	memset(vty->buffer, 0, HISTORY_MAX_SIZE);
}


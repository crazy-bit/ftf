#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ftf_util.h"
#include "ftf_vector.h"

// get an usable vector slot, if there is not, then allocate
// a new slot
static int ftf_vector_fetch(ftf_vector_t *v)
{
	int fetch_idx;

	// find next empty slot
	for (fetch_idx = 0; fetch_idx < v->used_size; fetch_idx++)
		if (v->data[fetch_idx] == NULL)
			break;

	// allocate new memory if not enough slot
	while (v->size < fetch_idx + 1) {
		v->data = realloc(v->data, sizeof(void *) * v->size * 2);
		if (!v->data) {
			ftf_debug(LOG_ERR, FNAME, "Not Enough Memory For data");
			return -1;
		}
		memset(&v->data[v->size], 0, sizeof(void *) * v->size);
		v->size *= 2;
	}

	return fetch_idx;
}

ftf_vector_t *ftf_vector_init(int size)
{
	ftf_vector_t *v = (ftf_vector_t *)calloc(1, sizeof(struct ftf_vector));
	if (v == NULL) {
		ftf_debug(LOG_ERR, FNAME, "Not Enough Memory For ftf_vector");
		return NULL;
	}

	/// allocate at least one slot
	if (size == 0)
		size = 1;
	v->data = calloc(1, sizeof(void *) * size);
	if (v->data == NULL) {
		ftf_debug(LOG_ERR, FNAME, "Not Enough Memory For data");
		return NULL;
	}

	v->used_size = 0;
	v->size = size;
	return v;
}

void ftf_vector_deinit(ftf_vector_t *v, int freeall)
{
	if (!v)
		return;
	if (v->data) {
		if (freeall) {
			int i;
			for (i = 0; i < ftf_vector_max(v); i++) {
				if (ftf_vector_slot(v, i))
					free(ftf_vector_slot(v, i));
			}
		}
		free(v->data);
	}
	free(v);
}

ftf_vector_t *ftf_vector_copy(ftf_vector_t *v)
{
	int size;
	ftf_vector_t *new_v = (ftf_vector_t *)calloc(1, sizeof(ftf_vector_t));

	new_v->used_size = v->used_size;
	new_v->size = v->size;

	size = sizeof(void *) * (v->size);
	new_v->data = calloc(1, sizeof(void *) * size);
	memcpy(new_v->data, v->data, size);

	return new_v;
}

void ftf_vector_insert(ftf_vector_t *v, void *val)
{
	int idx;

	idx = ftf_vector_fetch(v);
	v->data[idx] = val;
	if (v->used_size <= idx)
		v->used_size = idx + 1;
}


ftf_vector_t *str2vec(char *string)
{
	int strlen;
	char *cur, *start, *token;
	ftf_vector_t *vec;

	// empty string
	if (string == NULL)
		return NULL;

	cur = string;
	// skip white spaces
	while (isspace((int) *cur) && *cur != '\0')
		cur++;
	// only white spaces
	if (*cur == '\0')
		return NULL;
	// not care ! and #
	if (*cur == '!' || *cur == '#')
		return NULL;

	// copy each command pieces into vector
	vec = ftf_vector_init(1);
	while (1) 
	{
		start = cur;
		while (!(isspace((int) *cur) || *cur == '\r' || *cur == '\n') &&
			*cur != '\0')
			cur++;
		strlen = cur - start;
		token = (char *)malloc(sizeof(char) * (strlen + 1));
		memcpy(token, start, strlen);
		*(token + strlen) = '\0';
		ftf_vector_insert(vec, (void *)token);

		while((isspace ((int) *cur) || *cur == '\n' || *cur == '\r') &&
			*cur != '\0')
			cur++;
		if (*cur == '\0')
			return vec;
	}
}

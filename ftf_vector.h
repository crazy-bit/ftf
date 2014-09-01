/**
 * File:	ftf_vector.h
 * Purpose:	Including ftf_vector definition and operations
 * Author:	yoyo
 * Date:	2013.03.28
 */

#ifndef _FTF_VECTOR_H
#define _FTF_VECTOR_H

#include "ftf.h"

#define ftf_vector_slot(v, i)	((v)->data[(i)])
#define ftf_vector_max(v)	((v)->used_size)

/**
 * Initial ftf_vector
 *
 * @param size initial size of ftf_vector
 * @return initialized ftf_vector
 */
ftf_vector_t *ftf_vector_init(int size);

/**
 * Destroy ftf_vector
 *
 * @param v ftf_vector to destory
 * @param freeall if not set 0, then free data ftf_vector as well
 * @return none
 */
void ftf_vector_deinit(ftf_vector_t *v, int freeall);

/**
 * Get a copy of ftf_vector, this will allocate memory for ftf_vector
 *
 * @param v ftf_vector to be copied
 * @return copied ftf_vector
 * */
ftf_vector_t *ftf_vector_copy(ftf_vector_t *v);

/**
 * Insert a element 'val' into vector 'v'
 *
 * @param v ftf_vector to insert
 * @param val value to be inserted
 * @return none
 */
void ftf_vector_insert(ftf_vector_t *v, void *val);

/**
 * Turn a string into ftf_vector, this will alocate memory for ftf_vector
 *
 * @param string string to turn to vector
 * @return turned ftf_vector
 * **/
ftf_vector_t *str2vec(char *string);

#endif


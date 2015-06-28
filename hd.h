/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _HD_H_
#define _HD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_types.h"

/**
 * Flags for hd_init_* like functions.
 * \def HDFLG_DUPLKEYS
 * Indicates that you allow multiple values be stored undef the same key.
 * This implies a (potentially) random result on a normal hd_get(), and
 * full retrieval only using a cursor.
 * \def HDFLG_EXTEND
 * Indicates that extension of the resource should be possible.
 * \def HDFLG_ALLOCHDT
 * Indicates that htd_t pointers passed to the API that are elegible for
 * filling in, and that are set to { NULL, 0 }, may be allocated using the
 * memory-allocation function in the hd_t structure.
 */
#define HDFLG_MUSTEXIST   (1<<0)
#define HDFLG_DUPLKEYS    (1<<1)
#define HDFLG_EXTEND      (1<<2)
#define HDFLG_ALLOCHDT    (1<<3)
#define HDFLG_NOOVERWRITE (1<<4)

int hd_init_mem(hd_t* hd, unsigned int flags, void* mem, unsigned size);
int hd_init_fd(hd_t* hd, unsigned int flags, int fd);
int hd_init_malloc(hd_t* hd, unsigned int flags);
int hd_init_chunk(hd_t* hd, unsigned flags, int fd, unsigned off, unsigned siz);

/* Easily converts a char* to a key; the first argument can be NULL */
hdt_t hdt_string(hdt_t*, char* string);

int hd_get(hd_t* hd, hdt_t* key, hdt_t* value);
int hd_put(hd_t* hd, hdt_t* key, hdt_t* value, unsigned flags);
int hd_del(hd_t* hd, hdt_t* key, hdt_t* value);
int hd_pop(hd_t* hd, hdt_t* key, hdt_t* value);

/* Special function for getting an unknown quantity of data */
typedef int(*hdvalfnc_t)(hd_t*,hdt_t*,char*,unsigned,void*);
int hd_itr(hd_t* hd, hdt_t* key, hdvalfnc_t valuefnc, void* arg);

/**
 * Cursor functionality.  Mutations during iteration invalidate the cursor.
 */

int hdc_init(hd_t*, hdc_t*);
int hdc_mov(hdc_t*, hdt_t* key);
int hdc_nxt(hdc_t*, hdt_t* key, hdt_t* value);
int hdc_itr(hdc_t*, hdvalfnc_t valuefnc, void* arg);
int hdc_get(hdc_t*, hdt_t* key, hdt_t* value);
int hdc_rpl(hdc_t*, hdt_t* value);

int hd_debug(hd_t* hd);

/**
 * Errors; > 0 is recoverable; < 0 is not; 0 is success throughout the API
 */

#define HDERR_SPACE    4
#define HDERR_DUP      3
#define HDERR_INVAL    2
#define HDERR_NOTFOUND 1
#define HDERR_BOUNDS  -1
#define HDERR_LOCK    -2
#define HDERR_IO      -3
#define HDERR_INIT    -4
#define HDERR_NOMEM   -5

#ifdef __cplusplus
}
#endif

#endif

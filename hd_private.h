/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _HD_PRIVATE_H_
#define _HD_PRIVATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _HD_PRIVATE_
#include "hd.h"

#include <string.h>

#ifndef CHECK
#define CHECK(a) { int __r; if ((__r = (a))) { return __r; } }
#endif

#ifndef CHECK2
#define CHECK2(a,b) { int __r; if ((__r = (a))) { (b); return __r; } }
#endif

typedef int(*hd_itrfnc)(
  hd_t*,
  hdt_t* key,
  unsigned int hash,
  hdt_t* value,
  int found,
  unsigned int* buckets,
  unsigned int bucket,
  unsigned int off_last,
  unsigned int off_cur,
  struct keyhead* keyhead,
  void* arg
);

#include "hd_functions.h"
/*
int hd_init2(hd_t* hd, unsigned int flags);

int hd_read(hd_t*, unsigned off, char* buf, unsigned siz);
int hd_write(hd_t*, unsigned off, char* buf, unsigned siz);

int hd_init(hd_t* hd);

int hd_lock(hd_t* hd);
int hd_unlock(hd_t* hd);

int hd_rehash(hd_t* hd);
int hd_defrag(hd_t* hd);

int hd_iterate(hd_t* hd, hdt_t* key, hdt_t* value, hd_itrfnc fnc, void* arg);
int hd_read_header(hd_t* hd);
int hd_write_header(hd_t* hd);
int hd_read_buckets(hd_t* hd, unsigned int* buckets);
int hd_write_buckets(hd_t* hd, unsigned int* buckets);
int hd_read_chunkhead(hd_t* hd, unsigned int off, struct chunkhead* chunkhead);
int hd_write_chunkhead(hd_t* hd, unsigned int off, struct chunkhead* chunkhead);
int hd_read_keyhead(hd_t* hd, unsigned int off, struct keyhead* keyhead);
int hd_write_keyhead(hd_t* hd, unsigned int off, struct keyhead* keyhead);
int hd_read_uint(hd_t* hd, unsigned int where, unsigned int* what);
int hd_write_uint(hd_t* hd, unsigned int where, unsigned int what);

int hd_extend(hd_t* hd, unsigned int wanted);
int hd_yield(hd_t* hd, unsigned int off);
int hd_yield_size(hd_t* hd, unsigned int off, unsigned int size);
int hd_yield_all(hd_t* hd, unsigned int ptr);
int hd_claim(hd_t* hd, int contiguous, unsigned int* off, unsigned int* size);

int hd_read_value(hd_t* hd, struct keyhead* keyhead, hdt_t* value);
int hd_read_keydata(hd_t* hd, unsigned int keyoff, struct keyhead*, hdt_t*);

void hd_qsort(unsigned int* list, unsigned int length);
*/

#ifdef __cplusplus
}
#endif

#endif

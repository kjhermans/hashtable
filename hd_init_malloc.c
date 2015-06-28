/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "hd_private.h"

static
int hd_malloc_read
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  memcpy(buf, ((char*)(hd->resource.mem)) + off, siz);
  return 0;
}

static
int hd_malloc_write
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  memcpy(((char*)(hd->resource.mem)) + off, buf, siz);
  return 0;
}

static
unsigned hd_malloc_extend
  (hd_t* hd, unsigned d, void* arg)
{
  void* newmem = realloc(hd->resource.mem, hd->header.size + d);
  if (newmem) {
    hd->resource.mem = newmem;
    return d;
  }
  return 0;
}

static
void* hd_malloc_realloc
  (hd_t* hd, void* ptr, unsigned size, void* arg)
{
  return realloc(ptr, size);
}

/**
 * \ingroup hashtable
 *
 * Initializes a hd_t structure to use a malloc'ed piece of memory
 * as resource. This memory extends automatically to accomodate
 * insertions.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param flags Permanent flags to the dbm.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_init_malloc
  (hd_t* hd, unsigned flags)
{
  CHECK(hd_init(hd));
  hd->header.size = 4 * 1024;
  if ((hd->resource.mem = malloc(hd->header.size)) == NULL) {
    return HDERR_INIT;
  }
  hd->read = hd_malloc_read;
  hd->write = hd_malloc_write;
  if (flags & HDFLG_EXTEND) {
    hd->extend = hd_malloc_extend;
  }
  if (flags & HDFLG_ALLOCHDT) {
    hd->realloc = hd_malloc_realloc;
  }
  CHECK(hd_init2(hd, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif

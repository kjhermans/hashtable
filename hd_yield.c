/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

/**
 * Administrates space yielded to the empty list.
 */
int hd_yield
  (hd_t* hd, unsigned int off)
{
  unsigned int next = hd->header.off_e;
  hd->header.off_e = off;
  FAIL(hd_write_uint(hd, off, next));
  ++(hd->header.nempties);
  return 0;
}

int hd_yield_size
  (hd_t* hd, unsigned int off, unsigned int size)
{
  unsigned int next = hd->header.off_e;
  hd->header.off_e = off;
  struct chunkhead chunkhead = { next, size };
  FAIL(hd_write_chunkhead(hd, off, &chunkhead));
  ++(hd->header.nempties);
  return 0;
}

int hd_yield_all
  (hd_t* hd, unsigned int ptr)
{
  while (ptr) {
    unsigned int next;
    FAIL(hd_read_uint(hd, ptr, &next));
    FAIL(hd_yield(hd, ptr));
    --(hd->header.nchunks);
    ptr = next;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

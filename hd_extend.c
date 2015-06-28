/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

/**
 * \ingroup hashtable
 */
int hd_extend
  (hd_t* hd, unsigned int wanted)
{
  unsigned int granted = hd->extend(hd, wanted, hd->extendarg);
  if (granted > sizeof(struct chunkhead)) {
    unsigned int oldsize = hd->header.size;
    struct chunkhead chunkhead = {
      0,
      granted
    };
    hd->header.size += granted;
    FAIL(hd_write_chunkhead(hd, oldsize, &chunkhead));
    FAIL(hd_yield(hd, oldsize));
    return 0;
  }
  return HDERR_NOTFOUND;
}

#ifdef __cplusplus
}
#endif

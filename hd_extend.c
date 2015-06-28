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
 *
 * Extends the resource used by the hashtable.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param wanted The amount of bytes claimed.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_extend
  (hd_t* hd, unsigned wanted)
{
  unsigned granted = hd->extend(hd, wanted, hd->extendarg);
  if (granted > sizeof(struct chunkhead)) {
    unsigned oldsize = hd->header.size;
    struct chunkhead chunkhead = {
      0,
      granted
    };
    hd->header.size += granted;
    CHECK(hd_write_chunkhead(hd, oldsize, &chunkhead));
    CHECK(hd_yield(hd, oldsize));
    return 0;
  }
  return HDERR_NOTFOUND;
}

#ifdef __cplusplus
}
#endif

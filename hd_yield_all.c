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
 * \ingroup hashtable_private
 *
 * Yields a chain of chunks to the empty list.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param offset Offset to the first chunk to yield.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_yield_all
  (hd_t* hd, unsigned offset)
{
  while (offset) {
    unsigned next;
    CHECK(hd_read_uint(hd, offset, &next));
    CHECK(hd_yield(hd, offset));
    --(hd->header.nchunks);
    offset = next;
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

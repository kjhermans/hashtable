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
 * Yields a block of data from the dbm's resource to the empty list.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off Offset to the chunk.
 * \param size Size of the chunk.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_yield_size
  (hd_t* hd, unsigned off, unsigned size)
{
  unsigned next = hd->header.off_e;
  hd->header.off_e = off;
  struct chunkhead chunkhead = { next, size };
  CHECK(hd_write_chunkhead(hd, off, &chunkhead));
  ++(hd->header.nempties);
  return 0;
}

#ifdef __cplusplus
}
#endif

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
 * Administrates space yielded to the empty list.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off Offset of the chunk to administrate.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_yield
  (hd_t* hd, unsigned off)
{
  unsigned next = hd->header.off_e;
  hd->header.off_e = off;
  CHECK(hd_write_uint(hd, off, next));
  ++(hd->header.nempties);
  return 0;
}

#ifdef __cplusplus
}
#endif

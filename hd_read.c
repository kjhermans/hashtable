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
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \returns Zero on success, or non-zero on error.
 */
int hd_read
  (hd_t* hd, unsigned off, char* buf, unsigned size)
{
  if (off + size > hd->header.size) {
    return HDERR_BOUNDS;
  }
  return hd->read(hd, off, buf, size, hd->ioarg);
}

#ifdef __cplusplus
}
#endif

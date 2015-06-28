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
int hd_read
  (hd_t* hd, unsigned int off, char* buf, unsigned int size)
{
  if (off + size > hd->header.size) {
    return HDERR_BOUNDS;
  }
  return hd->read(hd, off, buf, size, hd->ioarg);
}

#ifdef __cplusplus
}
#endif

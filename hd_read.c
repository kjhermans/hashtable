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
 * Reads any amount of data from the dbm's resource.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off Offset within the resource to start reading.
 * \param buf The target buffer. This buffer will be filled with the
 *            data read from the resource on successful return.
 * \param size Size to be read from the resource.
 *
 * \returns Zero on success, or non-zero on error.
 * NB. Failure to read the full amount of data required results in error.
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

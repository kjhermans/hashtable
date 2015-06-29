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
 * Generic function to write data into the dbm's resource
 * at a given offset. All other hd_write_* functions use this
 * function in their underlying code.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off Offset within the resource to start writing.
 * \param buf Buffer to write.
 * \param size Size of the buffer.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_write
  (hd_t* hd, unsigned off, char* buf, unsigned size)
{
  if (off + size > hd->header.size) {
    return HDERR_BOUNDS;
  }
  return hd->write(hd, off, buf, size, hd->ioarg);
}

#ifdef __cplusplus
}
#endif

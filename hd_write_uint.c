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
 * Writes an unsigned integer at a specified offset into the dbm's resource.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param offset The offset at which to start writing.
 * \param uinteger The data to write.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_write_uint
  (hd_t* hd, unsigned offset, unsigned uinteger)
{
  CHECK(
    hd_write(
      hd,
      offset,
      (char*)(&uinteger),
      sizeof(unsigned)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

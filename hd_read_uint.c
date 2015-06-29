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
 * Reads an unsigned integer from the resource.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param offset Where to start reading.
 * \param uinteger On successful return, contains the value at
 *                 this place in the dbm's resource.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_read_uint
  (hd_t* hd, unsigned offset, unsigned* uinteger)
{
  CHECK(
    hd_read(
      hd,
      offset,
      (char*)uinteger,
      sizeof(unsigned)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

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
 * Reads a chunkhead from the resource.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off Offset within the resource to start reading,
 * \param chunkhead On successful return, will be filled with
 *                  the chunkhead data.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_read_chunkhead
  (hd_t* hd, unsigned off, struct chunkhead* chunkhead)
{
  CHECK(
    hd_read(
      hd,
      off,
      (char*)chunkhead,
      sizeof(struct chunkhead)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

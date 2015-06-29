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
 * Write a chunkhead at a certain offset into the dbm's resource.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off Offset to start writing at.
 * \param chunkhead The chunkhead to write.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_write_chunkhead
  (hd_t* hd, unsigned off, struct chunkhead* chunkhead)
{
  CHECK(
    hd_write(
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

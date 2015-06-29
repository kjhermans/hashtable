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
 * Writes a key header into the dbm's resource.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param off The offset at which to start writing.
 * \param keyhead The key header to write.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_write_keyhead
  (hd_t* hd, unsigned off, struct keyhead* keyhead)
{
  CHECK(
    hd_write(
      hd,
      off,
      (char*)keyhead,
      sizeof(struct keyhead)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

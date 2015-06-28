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
 *
 * Locks the dbm for access.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_lock
  (hd_t* hd)
{
  if (hd->lock) {
    if (++(hd->locked) == 1) {
      return hd->lock(hd, 1, hd->lockarg);
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

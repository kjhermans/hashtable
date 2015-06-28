/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

static
int hd_get_fnc(
  hd_t* hd,
  hdt_t* key,
  unsigned hash,
  hdt_t* value,
  int found,
  unsigned* buckets,
  unsigned bucket,
  unsigned off_last,
  unsigned off_cur,
  struct keyhead* keyhead,
  void* arg
) {
  if (!found) {
    return HDERR_NOTFOUND;
  } else {
    CHECK(hd_read_value(hd, keyhead, value));
    return 0;
  }
}

/**
 * \ingroup hashtable
 *
 * Retrieves the value of key.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 *            filled with the key to retrieve.
 * \param value Non-NULL pointer to an initialized tdt_t structure
 *              to be filled with the value on successful return.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_get
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  CHECK(hd_lock(hd));
  int r = hd_iterate(hd, key, value, hd_get_fnc, 0);
  CHECK(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

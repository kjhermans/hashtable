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
int hdc_mov_fnc(
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
    hdc_t* hdc = (hdc_t*)arg;
    hdc->bucket = bucket;
    hdc->ptr = off_cur;
    hdc->scn = hd->header.scn;
    return 0;
  }
}

/**
 * \ingroup hashtable
 *
 * Moves the cursor to the key.
 *
 * \param hdc Non-NULL pointer to an initialized hdc_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 *            containing the key to move to.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hdc_mov
  (hdc_t* hdc, hdt_t* key)
{
  CHECK(hd_lock(hdc->hd));
  int r = hd_iterate(hdc->hd, key, 0, hdc_mov_fnc, hdc);
  CHECK(hd_lock(hdc->hd));
  return r;
}

#ifdef __cplusplus
}
#endif

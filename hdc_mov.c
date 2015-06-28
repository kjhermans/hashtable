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
  unsigned int hash,
  hdt_t* value,
  int found,
  unsigned int* buckets,
  unsigned int bucket,
  unsigned int off_last,
  unsigned int off_cur,
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
 */
int hdc_mov
  (hdc_t* hdc, hdt_t* key)
{
  FAIL(hd_lock(hdc->hd));
  int r = hd_iterate(hdc->hd, key, 0, hdc_mov_fnc, hdc);
  FAIL(hd_lock(hdc->hd));
  return r;
}

#ifdef __cplusplus
}
#endif

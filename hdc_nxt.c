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
 * Iterates through the entire hashtable.
 * Returns possibly two recoverable errors; HDERR_INVAL and HDERR_NOTFOUND.
 * ...
 */
int hdc_nxt
  (hdc_t* hdc, hdt_t* key, hdt_t* value)
{
  hd_t* hd = hdc->hd;
  if (hdc->scn != hd->header.scn) {
    return HDERR_INVAL;
  }
  while (hdc->ptr == 0) {
    if (++(hdc->bucket) >= hd->header.nbuckets) {
      return HDERR_NOTFOUND;
    }
    FAIL(
      hd_read_uint(
        hd,
        hd->header.off_b + (hdc->bucket * sizeof(unsigned int)),
        &(hdc->ptr)
      )
    );
  }
  struct keyhead keyhead;
  FAIL(hd_read_keyhead(hd, hdc->ptr, &keyhead));
  FAIL(hd_read_value(hd, &keyhead, value));
  FAIL(hd_read_keydata(hd, hdc->ptr, &keyhead, key));
  hdc->ptr = keyhead.next;
  return 0;
}

#ifdef __cplusplus
}
#endif

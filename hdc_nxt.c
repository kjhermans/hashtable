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
 * Iterates through the entire hashtable.
 * Returns possibly two recoverable errors; HDERR_INVAL and HDERR_NOTFOUND.
 *
 * \param hdc Non-NULL pointer to an initialized hdc_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 *            ready to receive key data.
 * \param value Non-NULL pointer to an initialized tdt_t structure
 *              ready to receive value data.
 *
 * \returns zero on success, or non-zero on error.
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
    CHECK(
      hd_read_uint(
        hd,
        hd->header.off_b + (hdc->bucket * sizeof(unsigned)),
        &(hdc->ptr)
      )
    );
  }
  struct keyhead keyhead;
  CHECK(hd_read_keyhead(hd, hdc->ptr, &keyhead));
  CHECK(hd_read_value(hd, &keyhead, value));
  CHECK(hd_read_keydata(hd, hdc->ptr, &keyhead, key));
  hdc->ptr = keyhead.next;
  return 0;
}

#ifdef __cplusplus
}
#endif

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
 * \param hdc Non-NULL pointer to an initialized hdc_t structure.
 * \param valuefnc Callback function pointer to receive a potentially
 *                 large amount of data.
 * \param arg Argument provided by the caller, which will be passed to
 *            the callback for each value chunk.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hdc_itr
  (hdc_t* hdc, hdvalfnc_t valuefnc, void* arg)
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
  unsigned keysize = keyhead.size - sizeof(struct keyhead);
  char keydata[ keysize ];
  CHECK(hd_read(hd, hdc->ptr + sizeof(struct keyhead), keydata, keysize));
  hdt_t key = { keydata, keysize };
  unsigned ptr = keyhead.value;
  while (ptr) {
    struct chunkhead chunkhead;
    CHECK(hd_read_chunkhead(hd, ptr, &chunkhead));
    unsigned chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
    char chunkdata[ chunkdatasize ];
    CHECK(hd_read(hd, ptr + sizeof(struct chunkhead), chunkdata, chunkdatasize));
    CHECK(valuefnc(hd, &key, chunkdata, chunkdatasize, arg));
    ptr = chunkhead.next;
  }
  hdc->ptr = keyhead.next;
  return 0;
}

#ifdef __cplusplus
}
#endif

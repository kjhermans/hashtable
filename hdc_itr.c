/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

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
  unsigned int keysize = keyhead.size - sizeof(struct keyhead);
  char keydata[ keysize ];
  FAIL(hd_read(hd, hdc->ptr + sizeof(struct keyhead), keydata, keysize));
  hdt_t key = { keydata, keysize };
  unsigned int ptr = keyhead.value;
  while (ptr) {
    struct chunkhead chunkhead;
    FAIL(hd_read_chunkhead(hd, ptr, &chunkhead));
    unsigned int chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
    char chunkdata[ chunkdatasize ];
    FAIL(hd_read(hd, ptr + sizeof(struct chunkhead), chunkdata, chunkdatasize));
    FAIL(valuefnc(hd, &key, chunkdata, chunkdatasize, arg));
    ptr = chunkhead.next;
  }
  hdc->ptr = keyhead.next;
  return 0;
}

#ifdef __cplusplus
}
#endif

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
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \returns Zero on success, or non-zero on error.
 */
int hd_read_value
  (hd_t* hd, struct keyhead* keyhead, hdt_t* value)
{
  int reallocating = (
    value->data == 0 &&
    value->size == 0 &&
    (hd->header.flags & HDFLG_ALLOCHDT) &&
    hd->realloc != 0
  );
  unsigned off = 0;
  unsigned ptr = keyhead->value;
  while (ptr) {
    struct chunkhead chunkhead;
    CHECK(hd_read_chunkhead(hd, ptr, &chunkhead));
    unsigned chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
    char chunkdata[ chunkdatasize ];
    CHECK(hd_read(hd, ptr + sizeof(chunkhead), chunkdata, chunkdatasize));
    unsigned wantedsize = off + chunkdatasize;
    if (reallocating && wantedsize > value->size) {
      void* mem = hd->realloc(hd, value->data, wantedsize, hd->reallocarg);
      if (mem) {
        value->data = mem;
        value->size = wantedsize;
      } else {
        return HDERR_NOMEM;
      }
    }
    if (wantedsize > value->size) {
      memcpy(value->data + off, chunkdata, value->size - off);
      return 0;
    } else {
      memcpy(value->data + off, chunkdata, chunkdatasize);
      off += chunkdatasize;
    }
    ptr = chunkhead.next;
  }
  value->size = off;
  return 0;
}

#ifdef __cplusplus
}
#endif

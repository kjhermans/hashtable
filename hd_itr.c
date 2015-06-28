/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

struct hd_itr_helper {
  hdvalfnc_t fnc;
  void*      arg;
};

static
int hd_itr_fnc(
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
  struct hd_itr_helper* helper = (struct hd_itr_helper*)arg;
  if (!found) {
    return HDERR_NOTFOUND;
  } else {
    unsigned int ptr = keyhead->value;
    while (ptr) {
      struct chunkhead chunkhead;
      FAIL(hd_read_chunkhead(hd, ptr, &chunkhead));
      unsigned int chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
      char chunkdata[ chunkdatasize ];
      FAIL(hd_read(hd, ptr + sizeof(chunkhead), chunkdata, chunkdatasize));
      FAIL(helper->fnc(hd, key, chunkdata, chunkdatasize, helper->arg));
      ptr = chunkhead.next;
    }
    return 0;
  }
}

/**
 * \ingroup hashtable
 */
int hd_itr
  (hd_t* hd, hdt_t* key, hdvalfnc_t valuefnc, void* arg)
{
  struct hd_itr_helper helper = { valuefnc, arg };
  FAIL(hd_lock(hd));
  int r = hd_iterate(hd, key, 0, hd_itr_fnc, &helper);
  FAIL(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

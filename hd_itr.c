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
  struct hd_itr_helper* helper = (struct hd_itr_helper*)arg;
  if (!found) {
    return HDERR_NOTFOUND;
  } else {
    unsigned ptr = keyhead->value;
    while (ptr) {
      struct chunkhead chunkhead;
      CHECK(hd_read_chunkhead(hd, ptr, &chunkhead));
      unsigned chunkdatasize = chunkhead.size - sizeof(struct chunkhead);
      char chunkdata[ chunkdatasize ];
      CHECK(hd_read(hd, ptr + sizeof(chunkhead), chunkdata, chunkdatasize));
      CHECK(helper->fnc(hd, key, chunkdata, chunkdatasize, helper->arg));
      ptr = chunkhead.next;
    }
    return 0;
  }
}

/**
 * \ingroup hashtable
 *
 * Iterates through the hashtable, calling a user-provided callback
 * for each node.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 * \param valuefnc
 * \param arg
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_itr
  (hd_t* hd, hdt_t* key, hdvalfnc_t valuefnc, void* arg)
{
  struct hd_itr_helper helper = { valuefnc, arg };
  CHECK(hd_lock(hd));
  int r = hd_iterate(hd, key, 0, hd_itr_fnc, &helper);
  CHECK(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

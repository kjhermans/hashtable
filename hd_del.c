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
int hd_del_fnc(
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
    if (value) {
      CHECK(hd_read_value(hd, keyhead, value));
    }
    CHECK(hd_yield_all(hd, keyhead->value));
    CHECK(hd_yield(hd, off_cur));
    if (off_last == off_cur) {
      buckets[bucket] = keyhead->next;
      CHECK(hd_write_buckets(hd, buckets));
    } else {
      CHECK(hd_write_uint(hd, off_last, keyhead->next));
    }
    return 0;
  }
}

static
int hd_del_
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  int r = hd_iterate(hd, key, value, hd_del_fnc, 0);
  if (r == 0) {
    --(hd->header.nentries);
    ++(hd->header.scn);
    CHECK(hd_defrag(hd));
    CHECK(hd_write_header(hd));
  }
  return r;
}

/**
 * \ingroup hashtable
 *
 * Deletes an item from the hash, optionally filling in the data
 * contained therein.
 *
 * \param hd Pointer to an initialized hd_t structure.
 * \param key Pointer to an initialized hdt_t structure, containing the key.
 * \param value Pointer to an initialized hdt_t structure, which will be filled
 * with the removed value, or NULL if you're just interested in removal.
 *
 * \returns Zero on success, HDERR_NOTFOUND if the key cannot be found,
 * or any of the errors of the underlying functions.
 */
int hd_del
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  CHECK(hd_lock(hd));
  int r = hd_del_(hd, key, value);
  CHECK(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

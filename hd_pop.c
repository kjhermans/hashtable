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
int hd_pop__
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  CHECK(hd_read_header(hd));
  unsigned buckets[ hd->header.nbuckets ], i;
  CHECK(hd_read_buckets(hd, buckets));
  for (i=0; i < hd->header.nbuckets; i++) {
    if (buckets[i] != 0) {
      struct keyhead keyhead;
      CHECK(hd_read_keyhead(hd, buckets[i], &keyhead));
      CHECK(hd_read_value(hd, &keyhead, value));
      CHECK(hd_read_keydata(hd, buckets[i], &keyhead, key));
      CHECK(hd_yield_all(hd, keyhead.value));
      CHECK(hd_yield(hd, buckets[i]));
      buckets[i] = keyhead.next;
      CHECK(hd_write_buckets(hd, buckets));
      return 0;
    }
  }
  return HDERR_NOTFOUND;
}

static
int hd_pop_
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  CHECK(hd_read_header(hd));
  int r = hd_pop__(hd, key, value);
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
 * Pops a key/value pair off the hash.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 * \param value
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_pop
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  CHECK(hd_lock(hd));
  int r = hd_pop_(hd, key, value);
  CHECK(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

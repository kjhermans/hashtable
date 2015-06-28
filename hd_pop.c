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
  FAIL(hd_read_header(hd));
  unsigned int buckets[ hd->header.nbuckets ], i;
  FAIL(hd_read_buckets(hd, buckets));
  for (i=0; i < hd->header.nbuckets; i++) {
    if (buckets[i] != 0) {
      struct keyhead keyhead;
      FAIL(hd_read_keyhead(hd, buckets[i], &keyhead));
      FAIL(hd_read_value(hd, &keyhead, value));
      FAIL(hd_read_keydata(hd, buckets[i], &keyhead, key));
      FAIL(hd_yield_all(hd, keyhead.value));
      FAIL(hd_yield(hd, buckets[i]));
      buckets[i] = keyhead.next;
      FAIL(hd_write_buckets(hd, buckets));
      return 0;
    }
  }
  return HDERR_NOTFOUND;
}

static
int hd_pop_
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  FAIL(hd_read_header(hd));
  int r = hd_pop__(hd, key, value);
  if (r == 0) {
    --(hd->header.nentries);
    ++(hd->header.scn);
    FAIL(hd_defrag(hd));
    FAIL(hd_write_header(hd));
  }
  return r;
}

/**
 * Pops a key/value pair off the hash.
 */
int hd_pop
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  FAIL(hd_lock(hd));
  int r = hd_pop_(hd, key, value);
  FAIL(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

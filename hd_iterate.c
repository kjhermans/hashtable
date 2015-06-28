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
 * \ingroup hashtable_private
 *
 * Iterates to a given key and, when arrived at it, calls a
 * function supplied by the caller.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 * \param value
 * \param fnc
 * \param arg
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_iterate
  (hd_t* hd, hdt_t* key, hdt_t* value, hd_itrfnc fnc, void* arg)
{
  unsigned hash, bucket, last, ptr;
  CHECK(hd_read_header(hd));
  unsigned buckets[ hd->header.nbuckets ];
  CHECK(hd_read_buckets(hd, buckets));
  hash = hd->hash(hd, key, hd->hasharg);
  bucket = hash % hd->header.nbuckets;
  last = ptr = buckets[bucket];
  while (ptr) {
    struct keyhead keyhead;
    CHECK(hd_read_keyhead(hd, ptr, &keyhead));
    if (hash == keyhead.hash) {
      unsigned fsize = keyhead.size - sizeof(keyhead);
      char fkey[ fsize ];
      CHECK(hd_read(hd, ptr + sizeof(keyhead), fkey, fsize));
      hdt_t found = { fkey, fsize };
      int cmp;
      if ((cmp = hd->compare(hd, key, &found, hd->cmparg)) == 0) {
        return fnc(
          hd, key, hash, value, 1, buckets, bucket, last, ptr, &keyhead, arg
        );
      } else if (cmp > 0) {
        return fnc(
          hd, key, hash, value, 0, buckets, bucket, last, ptr, &keyhead, arg
        );
      }
    }
    last = ptr;
    ptr = keyhead.next;
  }
  return fnc(hd, key, hash, value, 0, buckets, bucket, last, ptr, 0, arg);
}

#ifdef __cplusplus
}
#endif

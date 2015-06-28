/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

/*
#define NPRIMES 10
static unsigned primes[] = {
  5, 11, 23, 47, 97, 193, 389, 787, 1583, 3187
};
*/

static
int hd_rehashing
  (hd_t* hd, unsigned newnbuckets, unsigned* newbuckets)
{
  unsigned i, top[ newnbuckets ], oldbuckets[ hd->header.nbuckets ];
  CHECK(hd_read_buckets(hd, oldbuckets));
  for (i = 0; i < newnbuckets; i++) {
    top[i] = 0;
    newbuckets[i] = 0;
  }
  for (i = 0; i < hd->header.nbuckets; i++) {
    unsigned ptr = oldbuckets[i];
    while (ptr) {
      struct keyhead keyhead;
      CHECK(hd_read_keyhead(hd, ptr, &keyhead));
      unsigned newbucket = keyhead.hash % newnbuckets;
      if (newbuckets[ newbucket ] == 0) {
        newbuckets[ newbucket ] = ptr;
      }
      if (top[ newbucket ] != 0) {
        CHECK(hd_write_uint(hd, top[ newbucket ], ptr));
      }
      top[ newbucket ] = ptr;
      ptr = keyhead.next;
    }
  }
  for (i = 0; i < newnbuckets; i++) {
    if (top[i] != 0) {
      CHECK(hd_write_uint(hd, top[i], 0));
    }
  }
  return 0;
}

static
int hd_do_rehash
  (hd_t* hd)
{
  unsigned nbuckets = (hd->header.nbuckets * 2) + 1;
  unsigned nbucketsize = nbuckets * sizeof(unsigned);
  unsigned buckets[ nbuckets ];
  unsigned off_b;
  CHECK(hd_claim(hd, 1, &off_b, &nbucketsize));
  CHECK(hd_rehashing(hd, nbuckets, buckets));
  CHECK(
    hd_yield_size(
      hd,
      hd->header.off_b,
      hd->header.nbuckets * sizeof(unsigned)
    )
  );
  hd->header.off_b = off_b;
  hd->header.nbuckets = nbuckets;
  CHECK(hd_write_buckets(hd, buckets));
  return 0;
}

/**
 * \ingroup hashtable
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \returns Zero on success, or non-zero on error.
 */
int hd_rehash
  (hd_t* hd)
{
  if (hd->header.nentries > (hd->header.nbuckets * hd->header.nbuckets)) {
    CHECK(hd_do_rehash(hd));
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

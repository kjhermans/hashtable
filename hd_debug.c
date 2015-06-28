/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "hd_private.h"

/**
 * \ingroup hashtable
 */
int hd_debug
  (hd_t* hd)
{
  FAIL(hd_read_header(hd));
  fprintf(stderr,
    "Header; size: %d, %d buckets, %d entries, %d empty chunks,\n"
    "  %d value chunks, %f fragmentation\n",
    hd->header.size, hd->header.nbuckets, hd->header.nentries,
    hd->header.nempties, hd->header.nchunks,
    (hd->header.nentries ?
      ((double)(hd->header.nchunks) / (double)(hd->header.nentries)) : 0)
  );
  unsigned int ptr = hd->header.off_e;
  while (ptr) {
    struct chunkhead chunkhead;
    FAIL(hd_read_chunkhead(hd, ptr, &chunkhead));
    fprintf(stderr, "Empty chunk at %d, next %d, size %d\n",
      ptr, chunkhead.next, chunkhead.size
    );
    ptr = chunkhead.next;
  }
  unsigned int buckets[ hd->header.nbuckets ];
  FAIL(hd_read_buckets(hd, buckets));
  unsigned int i=0;
  for (; i < hd->header.nbuckets; i++) {
    fprintf(stderr, "Bucket %d starts at %d\n", i, buckets[i]);
    ptr = buckets[i];
    while (ptr) {
      struct keyhead keyhead;
      FAIL(hd_read_keyhead(hd, ptr, &keyhead));
      fprintf(stderr, "  Key; size: %d, hash: %d, next: %d, value at %d\n",
        keyhead.size, keyhead.hash, keyhead.next, keyhead.value
      );
      unsigned int val = keyhead.value;
      while (val) {
        struct chunkhead chunkhead;
        FAIL(hd_read_chunkhead(hd, val, &chunkhead));
        fprintf(stderr, "    Value chunk at %d; size: %d, next %d\n",
          val,
          chunkhead.size,
          chunkhead.next
        );
        val = chunkhead.next;
      }
      ptr = keyhead.next;
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

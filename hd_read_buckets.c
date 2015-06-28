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
 */
int hd_read_buckets
  (hd_t* hd, unsigned int* buckets)
{
  FAIL(
    hd_read(
      hd,
      hd->header.off_b,
      (char*)buckets,
      sizeof(unsigned int) * hd->header.nbuckets
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

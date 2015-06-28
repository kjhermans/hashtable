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
int hd_write_buckets
  (hd_t* hd, unsigned int* buckets)
{
  FAIL(
    hd_write(
      hd,
      hd->header.off_b,
      (char*)buckets,
      hd->header.nbuckets * sizeof(unsigned int)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

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
 * Reads the bucket list from the dbm.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param buckets Contains the bucket list on successful return.
 *                This list should be at least hd->header.nbuckets
 *                * sizeof(unsigned) long.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_read_buckets
  (hd_t* hd, unsigned* buckets)
{
  CHECK(
    hd_read(
      hd,
      hd->header.off_b,
      (char*)buckets,
      sizeof(unsigned) * hd->header.nbuckets
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

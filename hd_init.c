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
 *
 * Initializer for a hd_t structure.
 *
 * \param hd Non-NULL pointer to an uninitialized hd_t structure.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_init
  (hd_t* hd)
{
  memset(hd, 0, sizeof(*hd));
  return 0;
}

#ifdef __cplusplus
}
#endif

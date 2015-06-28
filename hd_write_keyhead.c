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
int hd_write_keyhead
  (hd_t* hd, unsigned int off, struct keyhead* keyhead)
{
  FAIL(
    hd_write(
      hd,
      off,
      (char*)keyhead,
      sizeof(struct keyhead)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

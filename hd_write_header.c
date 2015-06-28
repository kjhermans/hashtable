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
int hd_write_header
  (hd_t* hd)
{
  FAIL(
    hd_write(
      hd,
      0,
      (char*)(&(hd->header)),
      sizeof(struct hd_header)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

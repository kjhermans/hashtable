/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

static
int hd_get_fnc(
  hd_t* hd,
  hdt_t* key,
  unsigned int hash,
  hdt_t* value,
  int found,
  unsigned int* buckets,
  unsigned int bucket,
  unsigned int off_last,
  unsigned int off_cur,
  struct keyhead* keyhead,
  void* arg
) {
  if (!found) {
    return HDERR_NOTFOUND;
  } else {
    FAIL(hd_read_value(hd, keyhead, value));
    return 0;
  }
}

int hd_get
  (hd_t* hd, hdt_t* key, hdt_t* value)
{
  FAIL(hd_lock(hd));
  int r = hd_iterate(hd, key, value, hd_get_fnc, 0);
  FAIL(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

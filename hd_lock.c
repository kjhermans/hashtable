/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

int hd_lock
  (hd_t* hd)
{
  if (hd->lock) {
    if (++(hd->locked) == 1) {
      return hd->lock(hd, 1, hd->lockarg);
    }
  }
  return 0;
}

int hd_unlock
  (hd_t* hd)
{
  if (hd->lock) {
    if (--(hd->locked) == 0) {
      return hd->lock(hd, 0, hd->lockarg);
    }
  }
  return 0;
}

#ifdef __cplusplus
}
#endif

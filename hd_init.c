/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

int hd_init
  (hd_t* hd)
{
  memset(hd, 0, sizeof(*hd));
  return 0;
}

#ifdef __cplusplus
}
#endif

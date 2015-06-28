/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

int hd_read_uint
  (hd_t* hd, unsigned int where, unsigned int* what)
{
  FAIL(
    hd_read(
      hd,
      where,
      (char*)what,
      sizeof(unsigned int)
    )
  );
  return 0;
}

#ifdef __cplusplus
}
#endif

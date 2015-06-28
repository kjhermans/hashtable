/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

int hdc_init
  (hd_t* hd, hdc_t* hdc)
{
  hdc->hd = hd;
  hdc->bucket = 0;
  FAIL(hd_read_uint(hd, hd->header.off_b, &(hdc->ptr)));
  hdc->scn = hd->header.scn;
  return 0;
}

#ifdef __cplusplus
}
#endif

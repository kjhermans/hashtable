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
 * Reads the resource header (the first n bytes) from the
 * resource into hd->header. This is done with each API access
 * so as to enable concurrent access to the file.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_read_header
  (hd_t* hd)
{
  CHECK(
    hd_read(
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

/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

static
int hd_mem_read
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  memcpy(buf, ((char*)(hd->resource.mem)) + off, siz);
  return 0;
}

static
int hd_mem_write
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  memcpy(((char*)(hd->resource.mem)) + off, buf, siz);
  return 0;
}

/**
 * \ingroup hashtable
 *
 * Initializes a hd_t structure to use a delimited piece of memory.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param flags Permanent flags for this dbm.
 * \param mem The memory on which to operate.
 * \param size The size of the memory.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_init_mem
  (hd_t* hd, unsigned flags, void* mem, unsigned size)
{
  if (flags & HDFLG_EXTEND) {
    return HDERR_INIT;
  }
  CHECK(hd_init(hd));
  hd->resource.mem = mem;
  hd->header.size = size;
  hd->read = hd_mem_read;
  hd->write = hd_mem_write;
  hd->ioarg = mem;
  CHECK(hd_init2(hd, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif

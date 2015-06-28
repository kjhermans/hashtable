/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>

extern void* realloc(void* ptr, unsigned int size);
extern void  free(void* ptr);

static
int hd_chunk_lock
  (hd_t* hd, int lock, void* lockarg)
{
  if (lock) {
    return flock(hd->resource.fd, LOCK_EX);
  } else {
    return flock(hd->resource.fd, LOCK_UN);
  }
}

static
int hd_chunk_read
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  if (lseek(hd->resource.fd, off + hd->resource.offset, SEEK_SET) != off) {
    return HDERR_IO;
  }
  if (read(hd->resource.fd, buf, siz) != siz) {
    return HDERR_IO;
  }
  return 0;
}

static
int hd_chunk_write
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  if (lseek(hd->resource.fd, off + hd->resource.offset, SEEK_SET) != off) {
    return HDERR_IO;
  }
  if (write(hd->resource.fd, buf, siz) != siz) {
    return HDERR_IO;
  }
  return 0;
}

static
void* hd_chunk_realloc
  (hd_t* hd, void* ptr, unsigned int size, void* arg)
{
  return realloc(ptr, size);
}

/**
 * \ingroup hashtable
 */
int hd_init_chunk
  (hd_t* hd, unsigned int flags, int fd, unsigned off, unsigned size)
{
  FAIL(hd_init(hd));
  hd->resource.fd = fd;
  hd->resource.offset = off;
  hd->header.size = size;
  hd->lock = hd_chunk_lock;
  hd->read = hd_chunk_read;
  hd->write = hd_chunk_write;
  if (flags & HDFLG_ALLOCHDT) {
    hd->realloc = hd_chunk_realloc;
  }
  FAIL(hd_init2(hd, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif

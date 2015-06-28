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

extern void* realloc(void* ptr, unsigned size);
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
  (hd_t* hd, void* ptr, unsigned size, void* arg)
{
  return realloc(ptr, size);
}

/**
 * \ingroup hashtable
 *
 * Initializes a hd_t structure based on a delimited file based resource.
 * Using this function will allow you to run multiple pieces chunks
 * next to one another in the same file.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param flags Permanent flags to the dbm.
 * \param fd Random access filedescriptor where the dbm data is to be stored.
 * \param off Offset within the file.
 * \param size Size that can be maximally used within the file.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_init_chunk
  (hd_t* hd, unsigned flags, int fd, unsigned off, unsigned size)
{
  CHECK(hd_init(hd));
  hd->resource.fd = fd;
  hd->resource.offset = off;
  hd->header.size = size;
  hd->lock = hd_chunk_lock;
  hd->read = hd_chunk_read;
  hd->write = hd_chunk_write;
  if (flags & HDFLG_ALLOCHDT) {
    hd->realloc = hd_chunk_realloc;
  }
  CHECK(hd_init2(hd, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif

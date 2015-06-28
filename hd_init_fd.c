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

static
int hd_fd_lock
  (hd_t* hd, int lock, void* lockarg)
{
  if (lock) {
    return flock(hd->resource.fd, LOCK_EX);
  } else {
    return flock(hd->resource.fd, LOCK_UN);
  }
}

static
int hd_fd_read
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  if (lseek(hd->resource.fd, off, SEEK_SET) != off) {
    return HDERR_IO;
  }
  if (read(hd->resource.fd, buf, siz) != siz) {
    return HDERR_IO;
  }
  return 0;
}

static
int hd_fd_write
  (hd_t* hd, unsigned off, char* buf, unsigned siz, void* ioarg)
{
  if (lseek(hd->resource.fd, off, SEEK_SET) != off) {
    return HDERR_IO;
  }
  if (write(hd->resource.fd, buf, siz) != siz) {
    return HDERR_IO;
  }
  return 0;
}

static
unsigned hd_fd_extend
  (hd_t* hd, unsigned d, void* arg)
{
  unsigned cursize;
  unsigned r = 0;
  char mem[1024];
  memset(mem, 0, 1024);
  if ((cursize = lseek(hd->resource.fd, 0, SEEK_END)) == (off_t)-1) {
    return 0;
  }
  hd->header.size = cursize;
  /* align d with kilobytes */
  if ((cursize + d) % 1024) {
    d += (1024 - ((cursize + d) % 1024));
  }
  while (d > 1024) {
    int w = write(hd->resource.fd, mem, 1024);
    if (w <= 0) break;
    r += w;
    d -= 1024;
  }
  {
    int w = write(hd->resource.fd, mem, d);
    r += w;
  }
  return r;
}

/**
 * \ingroup hashtable
 *
 * Initializes a hd_t structure to use a file decriptor as resource.
 *
 * \param hd Non-NULL pointer to an uninitialized hd_t structure.
 * \param flags Permanent flags to the dbm.
 * \param fd Random access file descriptor to be used by the dbm.
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_init_fd
  (hd_t* hd, unsigned flags, int fd)
{
  CHECK(hd_init(hd));
  hd->resource.fd = fd;
  if ((hd->header.size = lseek(fd, 0, SEEK_END)) == (off_t)-1) {
    return HDERR_INIT;
  }
  hd->lock = hd_fd_lock;
  hd->read = hd_fd_read;
  hd->write = hd_fd_write;
  if (flags & HDFLG_EXTEND) {
    hd->extend = hd_fd_extend;
  }
  CHECK(hd_init2(hd, flags));
  return 0;
}

#ifdef __cplusplus
}
#endif

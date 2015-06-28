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
int hd_store_value
  (hd_t* hd, hdt_t* value, unsigned* off)
{
  unsigned valueoff = 0;
  unsigned lastchunkoff = 0;
  *off = 0;
  while (1) {
    unsigned granted = (value->size - valueoff) + sizeof(struct chunkhead);
    unsigned chunkoff;
    unsigned chunkdatasize;
    CHECK2(hd_claim(hd, 0, &chunkoff, &granted), hd_yield_all(hd, *off));
    struct chunkhead chunkhead = {
      0,
      granted
    };
    chunkdatasize = granted - sizeof(struct chunkhead);
    if (*off == 0) {
      *off = chunkoff;
    }
    CHECK(hd_write_chunkhead(hd, chunkoff, &chunkhead));
    CHECK(
      hd_write(
        hd,
        chunkoff + sizeof(struct chunkhead),
        value->data + valueoff,
        chunkdatasize
      )
    );
    ++(hd->header.nchunks);
    if (lastchunkoff) {
      CHECK(hd_write_uint(hd, lastchunkoff, chunkoff));
    }
    if ((valueoff += chunkdatasize) == value->size) {
      return 0;
    }
    lastchunkoff = chunkoff;
  }
}

static
int hd_put_new(
  hd_t* hd,
  hdt_t* key,
  unsigned hash,
  hdt_t* value,
  unsigned* buckets,
  unsigned bucket,
  unsigned off_last,
  unsigned off_cur
) {
  unsigned keyoff, valueoff, keysize = sizeof(struct keyhead) + key->size;
  CHECK(hd_store_value(hd, value, &valueoff));
  CHECK(hd_claim(hd, 1, &keyoff, &keysize));
  struct keyhead keyhead = {
    off_cur,
    keysize,
    hash,
    valueoff
  };
  CHECK(hd_write_keyhead(hd, keyoff, &keyhead));
  CHECK(hd_write(hd, keyoff + sizeof(struct keyhead), key->data, key->size));
  if (off_last == off_cur) {
    buckets[ bucket ] = keyoff;
    CHECK(hd_write_buckets(hd, buckets));
  } else {
    CHECK(hd_write_uint(hd, off_last, keyoff));
  }
  return 0;
}

static
int hd_put_repl
  (hd_t* hd, hdt_t* value, unsigned off_cur, struct keyhead* keyhead)
{
  CHECK(hd_yield_all(hd, keyhead->value));
  CHECK(hd_store_value(hd, value, &(keyhead->value)));
  CHECK(hd_write_keyhead(hd, off_cur, keyhead));
  return 0;
}

static
int hd_put_fnc(
  hd_t* hd,
  hdt_t* key,
  unsigned hash,
  hdt_t* value,
  int found,
  unsigned* buckets,
  unsigned bucket,
  unsigned off_last,
  unsigned off_cur,
  struct keyhead* keyhead,
  void* arg
) {
  unsigned flags = (((unsigned)arg) | hd->header.flags);
  if (!found || (flags & HDFLG_DUPLKEYS)) {
    CHECK(hd_put_new(hd, key, hash, value, buckets, bucket, off_last, off_cur));
    ++(hd->header.nentries);
    ++(hd->header.scn);
  } else if (flags & HDFLG_NOOVERWRITE) {
    return HDERR_DUP;
  } else {
    CHECK(hd_put_repl(hd, value, off_cur, keyhead));
  }
  return 0;
}

static
int hd_put_
  (hd_t* hd, hdt_t* key, hdt_t* value, unsigned flags)
{
  int r = hd_iterate(hd, key, value, hd_put_fnc, (void*)flags);
  if (r == 0) {
    CHECK(hd_rehash(hd));
  }
  if (r >= 0) {
    CHECK(hd_write_header(hd));
  }
  return r;
}

/**
 * \ingroup hashtable
 *
 * Inserts an entry into the hashtable.
 *
 * \param hd Non-NULL pointer to an initialized hd_t structure.
 * \param key Non-NULL pointer to an initialized tdt_t structure
 * \param value Non-NULL pointer to an initialized tdt_t structure
 * \param flags
 *
 * \returns Zero on success, or non-zero on error.
 */
int hd_put
  (hd_t* hd, hdt_t* key, hdt_t* value, unsigned flags)
{
  CHECK(hd_lock(hd));
  int r = hd_put_(hd, key, value, flags);
  CHECK(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

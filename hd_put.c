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
  (hd_t* hd, hdt_t* value, unsigned int* off)
{
  unsigned int valueoff = 0;
  unsigned int lastchunkoff = 0;
  *off = 0;
  while (1) {
    unsigned int granted = (value->size - valueoff) + sizeof(struct chunkhead);
    unsigned int chunkoff;
    unsigned int chunkdatasize;
    FAIL2(hd_claim(hd, 0, &chunkoff, &granted), hd_yield_all(hd, *off));
    struct chunkhead chunkhead = {
      0,
      granted
    };
    chunkdatasize = granted - sizeof(struct chunkhead);
    if (*off == 0) {
      *off = chunkoff;
    }
    FAIL(hd_write_chunkhead(hd, chunkoff, &chunkhead));
    FAIL(
      hd_write(
        hd,
        chunkoff + sizeof(struct chunkhead),
        value->data + valueoff,
        chunkdatasize
      )
    );
    ++(hd->header.nchunks);
    if (lastchunkoff) {
      FAIL(hd_write_uint(hd, lastchunkoff, chunkoff));
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
  unsigned int hash,
  hdt_t* value,
  unsigned int* buckets,
  unsigned int bucket,
  unsigned int off_last,
  unsigned int off_cur
) {
  unsigned int keyoff, valueoff, keysize = sizeof(struct keyhead) + key->size;
  FAIL(hd_store_value(hd, value, &valueoff));
  FAIL(hd_claim(hd, 1, &keyoff, &keysize));
  struct keyhead keyhead = {
    off_cur,
    keysize,
    hash,
    valueoff
  };
  FAIL(hd_write_keyhead(hd, keyoff, &keyhead));
  FAIL(hd_write(hd, keyoff + sizeof(struct keyhead), key->data, key->size));
  if (off_last == off_cur) {
    buckets[ bucket ] = keyoff;
    FAIL(hd_write_buckets(hd, buckets));
  } else {
    FAIL(hd_write_uint(hd, off_last, keyoff));
  }
  return 0;
}

static
int hd_put_repl
  (hd_t* hd, hdt_t* value, unsigned int off_cur, struct keyhead* keyhead)
{
  FAIL(hd_yield_all(hd, keyhead->value));
  FAIL(hd_store_value(hd, value, &(keyhead->value)));
  FAIL(hd_write_keyhead(hd, off_cur, keyhead));
  return 0;
}

static
int hd_put_fnc(
  hd_t* hd,
  hdt_t* key,
  unsigned int hash,
  hdt_t* value,
  int found,
  unsigned int* buckets,
  unsigned int bucket,
  unsigned int off_last,
  unsigned int off_cur,
  struct keyhead* keyhead,
  void* arg
) {
  unsigned flags = (((unsigned)arg) | hd->header.flags);
  if (!found || (flags & HDFLG_DUPLKEYS)) {
    FAIL(hd_put_new(hd, key, hash, value, buckets, bucket, off_last, off_cur));
    ++(hd->header.nentries);
    ++(hd->header.scn);
  } else if (flags & HDFLG_NOOVERWRITE) {
    return HDERR_DUP;
  } else {
    FAIL(hd_put_repl(hd, value, off_cur, keyhead));
  }
  return 0;
}

static
int hd_put_
  (hd_t* hd, hdt_t* key, hdt_t* value, unsigned flags)
{
  int r = hd_iterate(hd, key, value, hd_put_fnc, (void*)flags);
  if (r == 0) {
    FAIL(hd_rehash(hd));
  }
  if (r >= 0) {
    FAIL(hd_write_header(hd));
  }
  return r;
}

/**
 * \ingroup hashtable
 */
int hd_put
  (hd_t* hd, hdt_t* key, hdt_t* value, unsigned flags)
{
  FAIL(hd_lock(hd));
  int r = hd_put_(hd, key, value, flags);
  FAIL(hd_unlock(hd));
  return r;
}

#ifdef __cplusplus
}
#endif

/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

#include <stdint.h>

#define FNV_OFFSET_BASIS 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

/**
 * \ingroup hashtable
 *
 * Uses FNV algorithm to get a hashed value of the memory presented.
 *
 * \param mem The memory pointer over which the hash must be created.
 * \param size The size of the meory over which the hash must be created.
 * \param hash The resultant hash on return.
 */
void hd_hash
  (unsigned char* mem, unsigned size, unsigned* hash)
{
  unsigned i;
  uint64_t workingmem = FNV_OFFSET_BASIS;

  for (i=0; i < size; i++) {
    workingmem = workingmem ^ mem[ i ];
    workingmem = workingmem * FNV_PRIME;
  }
  (*hash) = (unsigned)workingmem;
}

#ifdef __cplusplus
}
#endif

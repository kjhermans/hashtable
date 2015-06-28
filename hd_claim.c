/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

#define NFITRECORDS 8

typedef struct fitrec {
  unsigned ptr;
  unsigned nxt;
  unsigned siz;
  unsigned lst;
  unsigned dif;
} fitrec_t;

static
int hd_claim_fit
  (unsigned int size, unsigned int diff, unsigned int nicest)
{
  if (diff > nicest) {
    return 1;
  }
  return 0;
}

static
int hd_claim_(
  hd_t* hd,
  int contiguous,
  unsigned int* off,
  unsigned int* size
) {

  /* Building the fitness table (or finding a perfect fit */

  unsigned int ptr = hd->header.off_e, last = ptr;
  fitrec_t fitrec[ NFITRECORDS ];
  unsigned int noptions = 0;
  while (ptr) {
    struct chunkhead chunkhead;
    FAIL(hd_read_chunkhead(hd, ptr, &chunkhead));

    /* bingo, perfect fit ! */
    if (*size == chunkhead.size) {
      if (last == ptr) {
        hd->header.off_e = chunkhead.next;
      } else {
        FAIL(hd_write_uint(hd, last, chunkhead.next));
      }
      *off = ptr;
      --(hd->header.nempties);
      return 0;

    /* otherwise, a chunk may be conveniently longer */
    } else if (*size <= chunkhead.size - sizeof(struct chunkhead)) {
      fitrec[ noptions++ ] = (fitrec_t){
        ptr,
        chunkhead.next,
        chunkhead.size,
        last,
        (chunkhead.size - sizeof(struct chunkhead)) - *size
      };

    /* otherwise, a chunk may be conveniently shorter */
    } else if (*size > chunkhead.size && !contiguous) {
      fitrec[ noptions++ ] = (fitrec_t){
        ptr,
        chunkhead.next,
        chunkhead.size,
        last,
        *size - chunkhead.size
      };
    }
    if (noptions == NFITRECORDS) {
      break;
    }
    last = ptr;
    ptr = chunkhead.next;
  }

  /* Judging and using the fitness table */

  if (!noptions) {
    return HDERR_SPACE;
  } else {
    unsigned int i = 0, nicest = 0, choice = 0;
    for (; i < noptions; i++) {
      if (hd_claim_fit(*size, fitrec[i].dif, nicest)) {
        nicest = fitrec[i].dif;
        choice = i;
      }
    }

    /* split the empty chunk in two */
    if (fitrec[choice].siz > *size) {
      unsigned int emptyoff = fitrec[choice].ptr + *size;
      struct chunkhead emptychunk = {
        fitrec[choice].nxt,
        fitrec[choice].siz - *size
      };
      FAIL(hd_write_chunkhead(hd, emptyoff, &emptychunk));
      if (fitrec[choice].lst == fitrec[choice].ptr) {
        hd->header.off_e = emptyoff;
      } else {
        FAIL(hd_write_uint(hd, fitrec[choice].lst, emptyoff));
      }
      *off = fitrec[choice].ptr;
      return 0;

    /* use up this chunk, but change *size */
    } else {
      if (fitrec[choice].lst == fitrec[choice].ptr) {
        hd->header.off_e = fitrec[choice].nxt;
      } else {
        FAIL(hd_write_uint(hd, fitrec[choice].lst, fitrec[choice].nxt));
      }
      *off = fitrec[choice].ptr;
      *size = fitrec[choice].siz;
      --(hd->header.nempties);
      return 0;
    }
  }
}

/**
 * \par Private function:
 * Claims an amount of resources.  While iterating through the linked list
 * of empty chunks, fills up a little table denoting the fittingness of
 * this chunk to be used.  Perfect fits are used immediately, while
 * not-so-perfect fits fill up the fitness table.  In the end, when either
 * the table is full or the end of the list has been reached, it makes a
 * choice for the nicest record of the table, and uses the data in this
 * record to make changes.
 *
 * \param hd Pointer to the initialized hd_t structure.
 * \param contiguous Whether or not the value of *size must be met whole.
 * \param off Returns the offset on success
 * \param size Filled with requested size on calling, filled with granted
 * size on return.  NB. The requested size must be the full amount of space
 * requested, not diminished with chunk- or keyheader sizes.
 * \returns Zero on success, HDERR_SPACE when no piece can be found, or
 * any of the errors of the underlying functions.
 *
 * \par NB: This function assumes that the header will be written again
 * when this function returns successfully.
 */
int hd_claim
  (hd_t* hd, int contiguous, unsigned int* off, unsigned int* size)
{
  int r = hd_claim_(hd, contiguous, off, size);
  if (r) {
    if (r == HDERR_SPACE && hd->extend) {
      unsigned int wanted = hd->header.size;
      if (wanted < *size) {
        wanted = *size;
      }
      FAIL(hd_extend(hd, wanted));
      FAIL(hd_claim_(hd, contiguous, off, size));
      return 0;
    }
  }
  return r;
}

#ifdef __cplusplus
}
#endif

/*
** Copyright 2015 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
** License: BSD
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

void hd_qsort
  (unsigned int* list, unsigned int length)
{
  if (length <= 1) {
    return;
  } else if (length == 2) {
    if (list[0] > list[1]) {
      unsigned int tmp = list[0];
      list[0] = list[1];
      list[1] = tmp;
    }
    return;
  } else {
    unsigned int pivotpos = length / 2;
    unsigned int pivotval = list[pivotpos];
    unsigned int smaller[ length ], smallerlength = 0;
    unsigned int equal[ length ], equallength = 0;
    unsigned int bigger[ length ], biggerlength = 0;
    unsigned int i, l;
    for (i=0; i < length; i++) {
      if (i == pivotpos) {
        equal[equallength++] = pivotval;
      } else {
        if (list[i] < pivotval) {
          smaller[smallerlength++] = list[i];
        } else if (list[i] > pivotval) {
          bigger[biggerlength++] = list[i];
        } else {
          equal[equallength++] = pivotval;
        }
      }
    }
    hd_qsort(smaller, smallerlength);
    hd_qsort(bigger, biggerlength);
    for (i=0, l=0; i<smallerlength; i++, l++) {
      list[l] = smaller[i];
    }
    for (i=0; i<equallength; i++, l++) {
      list[l] = equal[i];
    }
    for (i=0; i<biggerlength; i++, l++) {
      list[l] = bigger[i];
    }
  }
}

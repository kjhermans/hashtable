/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "hd_private.h"

extern unsigned int strlen(char*);

hdt_t hdt_string
  (hdt_t* hdt, char* string)
{
  if (hdt) {
    hdt->data = string;
    hdt->size = strlen(string);
    return *hdt;
  } else {
    return (hdt_t){ string, strlen(string) };
  }
}

#ifdef __cplusplus
}
#endif

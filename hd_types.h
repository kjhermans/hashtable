/*
** Copyright 2008 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _HD_TYPES_H_
#define _HD_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hd hd_t;
typedef struct hdt hdt_t;

struct hdt {

  void*             data;      /* Memory */
  unsigned          size;      /* Its size */

};

#define CHUNKHEAD                                                            \
  unsigned          next;      /* Pointer to next chunk, or next key */      \
  unsigned          size;      /* This chunk's size, including these 2 ints */

#ifdef _HD_PRIVATE_

struct chunkhead {
  CHUNKHEAD
};

#define KEYHEAD                                                              \
  CHUNKHEAD                                                                  \
  unsigned          hash;      /* The hash of this key */                    \
  unsigned          value;     /* Pointer to first value chunk */

struct keyhead {
  KEYHEAD
};

#endif // _HD_PRIVATE_

struct hd_header {

  char              magic[4];  /* Spells: { 'h', 'd', 'i', '\0' } */
  unsigned          nbuckets;  /* The number of buckets */
  unsigned          flags;     /* Tweak behaviour */
  unsigned          size;      /* Our working limit */
  unsigned          off_b;     /* Where our buckets list starts */
  unsigned          off_e;     /* Where our empty list starts */
  unsigned          scn;       /* Every change causes this to increase */

  /* Statistics */

  unsigned          nentries;  /* The total amount of entries */
  unsigned          nempties;  /* Amount of empty chunks */
  unsigned          nchunks;   /* Amount of chunks */
};

struct hd
  {

    struct hd_header  header;
    unsigned          id;
    int               locked;

    /* Callbacks to implement locking; not obligatory */
    int             (*lock)(hd_t*,int,void*);
    void*             lockarg;

    /* Callbacks to implement retrieval and storage; obligatory */
    int             (*read)(hd_t*, unsigned off, char*, unsigned siz, void*);
    int             (*write)(hd_t*, unsigned off, char*, unsigned siz, void*);
    void*             ioarg;

    /* Callback to implement (a different form of) sorting; not obligatory */
    int             (*compare)(hd_t*,hdt_t*,hdt_t*,void*);
    void*             cmparg;

    /* Callback to implement (a different) hashing algorithem; not obligatory */
    unsigned        (*hash)(hd_t*,hdt_t*,void*);
    void*             hasharg;

    /* Callback to implement extension of the working area; not obligatory */
    unsigned        (*extend)(hd_t*,unsigned,void*);
    void*             extendarg;

    /* Callback to implement (m)allocating memory in keys and values; not ob. */
    void*           (*realloc)(hd_t*,void*,unsigned,void*);
    void*             reallocarg;

    struct {
      int               fd;
      void*             mem;
      unsigned          offset;
    }                 resource;

  };

/* Cursor */

typedef struct hdc {

  hd_t*             hd;
  unsigned          bucket;
  unsigned          ptr;
  unsigned          scn;

} hdc_t;

#ifdef __cplusplus
}
#endif

#endif

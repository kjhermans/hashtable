#include <stdio.h>
#include "hd_private.h"

#define MEMSIZE 1024

static
char mem[ MEMSIZE ];

extern void debug_mem(void*,int);

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting init\n\n");
  hd_t hd;
  hd_init_mem(&hd, 0, (void*)mem, MEMSIZE);
  debug_mem(mem, MEMSIZE);
  return 0;
}

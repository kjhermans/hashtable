#include <stdio.h>
#include "hd_private.h"

#define MEMSIZE 1024

static
char mem[ MEMSIZE ];

extern void debug_mem(void*,int);

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting one put\n\n");
  hd_t hd;
  hdt_t key = hdt_string(0, "foo"), value = hdt_string(0, "bar");
  hd_init_mem(&hd, 0, (void*)mem, MEMSIZE);
  FAIL(hd_put(&hd, &key, &value, 0));
  hd_debug(&hd);
  debug_mem(mem, 512);
  return 0;
}

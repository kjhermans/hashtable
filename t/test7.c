#include <stdio.h>
#include "hd_private.h"

#define MEMSIZE 1024

static
char area[ MEMSIZE ];

extern void debug_mem(void*,int);

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting a put, a get, a del and one more put\n\n");
  hd_t hd;
  hdt_t key1 = hdt_string(0, "foo"), value1 = hdt_string(0, "bar");
  hdt_t key2 = hdt_string(0, "pus"), value2 = hdt_string(0, "acs");
  char mem[100];
  hdt_t rvalue = { mem, 100 };
  memset(area, 0, MEMSIZE);
  hd_init_mem(&hd, 0, (void*)area, MEMSIZE);
  CHECK(hd_put(&hd, &key1, &value1, 0));
  CHECK(hd_get(&hd, &key1, &rvalue));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  CHECK(hd_del(&hd, &key1, &rvalue));
  if (hd_get(&hd, &key1, &rvalue) != HDERR_NOTFOUND) {
    return ~0;
  }
  CHECK(hd_put(&hd, &key2, &value2, 0));
  hd_debug(&hd);
  debug_mem(area, 512);
  return 0;
}

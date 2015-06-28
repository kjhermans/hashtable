#include <stdio.h>
#include "hd_private.h"

#define MEMSIZE 1024

static
char area[ MEMSIZE ];

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting one put and one get\n\n");
  hd_t hd;
  hdt_t key = hdt_string(0, "foo"), value = hdt_string(0, "bar");
  char mem[100];
  hdt_t rvalue = { mem, 100 };
  hd_init_mem(&hd, 0, (void*)area, MEMSIZE);
  CHECK(hd_put(&hd, &key, &value, 0));
  CHECK(hd_get(&hd, &key, &rvalue));
  fprintf(stderr, "Received '%-.*s'\n", rvalue.size, (char*)(rvalue.data));
  return 0;
}

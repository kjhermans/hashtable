#include <stdio.h>
#include "hd_private.h"

#define MEMSIZE (1024 * 1024)

static
char area[ MEMSIZE ];

static
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

extern void debug_mem(void*,int);

int main(int argc, char* argv[]) {
  fprintf(stderr, "\n\nTesting rehash\n\n");
  hd_t hd;
  hdt_t key, value;
  memset(area, 0, MEMSIZE);
  hd_init_mem(&hd, 0, (void*)area, MEMSIZE);
  int i=0; for (; i<30; i++) {
    char keymem[3], valmem[3];
    keymem[0] = alphabet[i % 7];
    keymem[1] = alphabet[i % 13];
    keymem[2] = alphabet[i % 23];
    valmem[0] = alphabet[25 - (i % 7)];
    valmem[1] = alphabet[25 - (i % 13)];
    valmem[2] = alphabet[25 - (i % 23)];
    key = (hdt_t){ keymem, 3 };
    value = (hdt_t){ valmem, 3 };
    fprintf(stderr, "\nPutting %-.*s -> %-.*s\n", 3, keymem, 3, valmem);
    FAIL(hd_put(&hd, &key, &value, 0));
    hd_debug(&hd);
  }
  hd_debug(&hd);
  debug_mem(area, 1024);
  return 0;
}

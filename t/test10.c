#include <stdlib.h>
#include <stdio.h>
#include "hd_private.h"

static
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

extern void debug_mem(void*,int);

int main(int argc, char* argv[]) {
  hd_t hd;
  hd_init_malloc(&hd, HDFLG_ALLOCHDT|HDFLG_DUPLKEYS|HDFLG_EXTEND);
  {
    hdt_t key, value;
    int i;
    for (i=0; i<30; i++) {
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
      CHECK(hd_put(&hd, &key, &value, 0));
    }
  }
  int n = 0;
  while (1) {
    hdt_t key = { 0, 0 }, value = { 0, 0 };
    int r = hd_pop(&hd, &key, &value);
    if (r == HDERR_NOTFOUND) {
      break;
    }
    fprintf(stderr, "Popping '%-.*s' -> '%-.*s'\n",
      key.size, (char*)(key.data), value.size, (char*)(value.data)
    );
    free(key.data);
    free(value.data);
    ++n;
  }
  if (n != 30) {
    fprintf(stderr, "Didn't pop 30 !\n");
    return ~0;
  }
  hd_debug(&hd);
  return 0;
}

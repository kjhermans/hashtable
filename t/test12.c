#include "hd_private.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void debug_mem
  (void* _mem, unsigned int size);

int nrecords = 20000;

int main
  (int argc, char* argv[])
{
  fprintf(stderr, "Putting %d records into a single btree.\n", nrecords);
  fprintf(stderr, "Opening db file at /tmp/foo\n");
  int fd = open("/tmp/foo", O_RDWR|O_CREAT|O_TRUNC, 0664);
  if (fd >= 0) {
    hd_t hd;
    {
      char buf[1024];
      memset(buf, 0, 1024);
      write(fd, buf, 1024);
    }
    hd_init_fd(&hd, HDFLG_DUPLKEYS|HDFLG_EXTEND, fd);
    char chars[] = "abcdefghijklmnopqrstuvwxyz";
    srand(time(NULL));
    int i=0; for (; i<nrecords; i++) {
      char keymem[ 64 ], valuemem[ 64 ];
      int keylen = 1 + (int) (64.0 * (rand() / (RAND_MAX + 1.0)));
      int j=0; for (; j<keylen; j++) {
        int ord = (int) (26.0 * (rand() / (RAND_MAX + 1.0)));
        keymem[j] = chars[ord];
      }
      int valuelen = 1 + (int) (64.0 * (rand() / (RAND_MAX + 1.0)));
      j=0; for (; j<valuelen; j++) {
        int ord = (int) (26.0 * (rand() / (RAND_MAX + 1.0)));
        valuemem[j] = chars[ord];
      }
      hdt_t key = { keymem, keylen }, value = { valuemem, valuelen };
//      fprintf(stderr, "%.8d put\n", i);
      int r = hd_put(&hd, &key, &value, 0);
      if (r) {
        fprintf(stderr, "Ends with code %d\n", r);
        hd_debug(&hd);
        return r;
      }
    }
  }
  return 0;
}

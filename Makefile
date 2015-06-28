OBJECTS=\
  hd_claim.o \
  hd_debug.o \
  hd_defrag.o \
  hd_del.o \
  hd_extend.o \
  hd_get.o \
  hd_init.o \
  hd_init2.o \
  hd_init_chunk.o \
  hd_init_fd.o \
  hd_init_malloc.o \
  hd_init_mem.o \
  hd_iterate.o \
  hd_itr.o \
  hd_lock.o \
  hd_pop.o \
  hd_put.o \
  hd_qsort.o \
  hd_read.o \
  hd_read_buckets.o \
  hd_read_chunkhead.o \
  hd_read_header.o \
  hd_read_keydata.o \
  hd_read_keyhead.o \
  hd_read_uint.o \
  hd_read_value.o \
  hd_rehash.o \
  hd_write.o \
  hd_write_buckets.o \
  hd_write_chunkhead.o \
  hd_write_header.o \
  hd_write_keyhead.o \
  hd_write_uint.o \
  hd_yield.o \
  hdc_init.o \
  hdc_itr.o \
  hdc_mov.o \
  hdc_nxt.o \
  hdt_string.o \

CFLAGS=-g -O2 -Wall -fno-builtin

TARGET=libhashtable.a

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Creating library .. Ok"; ar -rcs $(TARGET) $(OBJECTS)

test:
	@cd t && make

clean:
	@rm -f $(OBJECTS) $(TARGET)
	@cd t && make clean

archive: clean
	@RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/hashtable-$$RELEASE.tar.gz"; \
	cd .. && tar czf ~/hashtable-$$RELEASE.tar.gz hashtable/

%.o: %.c
	@echo -n "Compiling $< .. "; \
	if $(CC) $(CFLAGS) -c -o $@ $<; then \
	  echo "Ok"; \
	else \
	  echo "Failed: $(CC) $(CFLAGS) -c -o $@ $<"; \
	  exit -1; \
	fi

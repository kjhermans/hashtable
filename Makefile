OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')

CFLAGS=-g -O2 -Wall -fno-builtin

TARGET=libhashtable.a

all: headers $(TARGET)

headers: hd_functions.h

hd_functions.h: *.c
	@echo "  [GENFUNCTION]"
	@perl ./gen_functions_h.pl > hd_functions.h

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

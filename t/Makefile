OBJECTS=\
  debug.o \

MAINS=\
  test1.o \
  test2.o \
  test3.o \
  test4.o \
  test5.o \
  test6.o \
  test7.o \
  test8.o \
  test9.o \
  test10.o \
  test12.o \

TARGETS=\
  test1 \
  test2 \
  test3 \
  test4 \
  test5 \
  test6 \
  test7 \
  test8 \
  test9 \
  test10 \
  test12 \
  hell

CFLAGS=-I.. -g -O2 -Wall

all: $(OBJECTS) $(MAINS) $(TARGETS)
	@rm -f log; for TARGET in $(TARGETS); do \
	  echo -n "Running $$TARGET .. "; \
	  echo "Running $$TARGET" >>log; \
	  if ./$$TARGET >>log 2>&1; then \
	    echo "Ok"; \
	  else \
	    echo "Failed"; \
	  fi; \
	done

clean:
	@rm -f log $(OBJECTS) $(MAINS) $(TARGETS) hell.c

test1: test1.o
	@$(CC) $(CFLAGS) -o test1 test1.o $(OBJECTS) ../*.o

test2: test2.o
	@$(CC) $(CFLAGS) -o test2 test2.o $(OBJECTS) ../*.o

test3: test3.o
	@$(CC) $(CFLAGS) -o test3 test3.o $(OBJECTS) ../*.o

test4: test4.o
	@$(CC) $(CFLAGS) -o test4 test4.o $(OBJECTS) ../*.o

test5: test5.o
	@$(CC) $(CFLAGS) -o test5 test5.o $(OBJECTS) ../*.o

test6: test6.o
	@$(CC) $(CFLAGS) -o test6 test6.o $(OBJECTS) ../*.o

test7: test7.o
	@$(CC) $(CFLAGS) -o test7 test7.o $(OBJECTS) ../*.o

test8: test8.o
	@$(CC) $(CFLAGS) -o test8 test8.o $(OBJECTS) ../*.o

test9: test9.o
	@$(CC) $(CFLAGS) -o test9 test9.o $(OBJECTS) ../*.o

test10: test10.o
	@$(CC) $(CFLAGS) -o test10 test10.o $(OBJECTS) ../*.o

test12: test12.o
	@$(CC) $(CFLAGS) -o test12 test12.o $(OBJECTS) ../*.o

hell:
	@echo "Generating hell.c .."
	@/usr/bin/perl hell.pl > hell.c
	@echo "Compiling hell.c .."
	@$(CC) $(CFLAGS) -o hell hell.c $(OBJECTS) ../libhashtable.a

CC = gcc
MAKEVV = @
CFLAG = -fPIC
AR = ar
PWD=$(shell pwd)
.PHONY: all so a clean

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))


all: so a bin_a bin_so
#$(OBJ)
#$(OBJ): %.o : %.c
#	$(MAKEVV)$(CC) $(FLAG) $< -o $@

so: 
	$(CC) $(CFLAG) -shared test_a.c -o test_a_s.o
	$(CC) $(CFLAG) -shared test_b.c -o test_b_s.o
	$(CC) $(CFLAG) -shared test_c.c -o test_c_s.o
	$(CC) -shared -o libtest.so test_a_s.o test_b_s.o test_c_s.o

a: test_a.o test_b.o test_c.o
	$(AR) r libtest.a test_a.o test_b.o test_c.o

bin_a:
	$(CC) test.c -o test_a libtest.a

bin_so:
	$(CC) -o test_so -L$(PWD) -ltest test.c

test_a.o: test_a.c
test_b.o: test_b.c
test_c.o: test_c.c

clean : 
	rm -f *.o *.a *.so test_a test_so


CC = gcc
MAKEVV = @
CFLAG = -fPIC
AR = ar
PWD=$(shell pwd)
.PHONY: all so a clean

SRC = $(wildcard *.c)
OBJ = $(subst .c, .o, $(SRC))

all: so a

so: 
	$(CC) $(CFLAG) -c test_a.c
	$(CC) $(CFLAG) -c test_b.c
	$(CC) $(CFLAG) -c test_c.c
	$(CC) -shared -o libmytest.so test_a.o test_b.o test_c.o
	$(CC) -o test_so test.c -L$(PWD) -lmytest

a:
	$(CC) -c test_a.c
	$(CC) -c test_b.c
	$(CC) -c test_c.c
	$(AR) crv libtest.a test_a.o test_b.o test_c.o
	$(CC) -o test_a test.c libtest.a

clean : 
	rm -f *.o *.a *.so test_a test_so


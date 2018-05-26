CC = gcc
MAKEVV = @
CFLAG = -fPIC
PWD=$(shell pwd)
.PHONY: all so a clean

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))

all: so bin
#$(OBJ)
#$(OBJ): %.o : %.c
#	$(MAKEVV)$(CC) $(FLAG) $< -o $@

so: 
	$(CC) $(CFLAG) libshare.c -shared -o libshare.so 

bin:
	$(CC) testdl.c -o testdl.o -ldl

clean : 
	rm -f *.o *.a *.so


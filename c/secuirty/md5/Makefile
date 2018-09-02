CC = gcc
MAKEVV = @
CFLAG = -fPIC
PWD=$(shell pwd)
.PHONY: all so app clean

all: so app

so: 
	$(CC) -shared -o libmd5.so md5.c $(CFLAG)

app:
	$(CC) -o test_md5 test_md5.c -I. -L. -lmd5 

clean : 
	rm -f *.o *.a *.so test_md5


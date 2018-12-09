CC 		:= gcc
CFLAGS 	:= -fPIC

.PHONY: all so app clean

all: so app

so:
	$(CC) $(CFLAGS) -shared -o libmd5.so md5.c

app:
	$(CC) $(CFLAGS) -o test_md5.elf test_md5.c -I. -L. -lmd5

clean:
	rm -f *.o *.a *.so *.elf

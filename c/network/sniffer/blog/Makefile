CC = gcc
MAKEVV = @
FLAG = -lpcap

.PHONY: all  clean

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))

all: $(OBJ)

$(OBJ): %.o : %.c
	$(MAKEVV)$(CC) $< $(FLAG) -o $@

clean : 
	rm -f *.o


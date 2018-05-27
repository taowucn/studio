CC = gcc
MAKEVV = @
FLAG = -lm -static

.PHONY: all  clean

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))

all: $(OBJ)

$(OBJ): %.o : %.c
	$(MAKEVV)$(CC) $< -o $@ $(FLAG) 

clean : 
	rm -f *.o


CC = gcc
MAKEVV = @
FLAG = 

.PHONY: all  clean

SRC = $(wildcard *.c)
OBJ = $(subst .c,.o,$(SRC))

all: $(OBJ)

$(OBJ): %.o : %.c
	$(MAKEVV)$(CC) $< -o $@ $(FLAG)

clean : 
	rm -f *.o


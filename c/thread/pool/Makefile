
.PHONY: all clean

CC=gcc
CFLAGS=-Wall -g
LIB=-lpthread
OBJS=main.o threadpool.o condition.o
BIN=proc.o

all:$(BIN)
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

# to acquire the rules
#use: gcc -MM *.c
condition.o: condition.c condition.h pool_util.h
main.o: main.c threadpool.h condition.h
threadpool.o: threadpool.c threadpool.h condition.h pool_util.h

clean:
	@rm -rf *.dSYM *.o $(BIN)


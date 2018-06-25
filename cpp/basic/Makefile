CC = g++
MAKEVV = @
FLAG = -std=c++11 -lpthread

.PHONY: all  clean

SRC = $(wildcard *.cpp)
OBJ = $(subst .cpp,.o,$(SRC))

all: $(OBJ)

$(OBJ): %.o : %.cpp
	$(MAKEVV)$(CC) $< -o $@ $(FLAG)

clean : 
	rm -f *.o


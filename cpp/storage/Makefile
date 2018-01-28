CC = g++
MAKEVV = @
FLAG = -lpthread -std=c++11 

.PHONY: all  clean

SRC = $(wildcard *.cpp)
OBJ = $(subst .cpp,.o,$(SRC))

all: $(OBJ)

$(OBJ): %.o : %.cpp
	$(MAKEVV)$(CC)  $< -o $@  $(FLAG)

clean : 
	rm -f *.o


CC = g++
MAKEVV = @
FLAG = 

.PHONY: all  clean

SRC = $(wildcard *.cpp)
OBJ = $(subst .cpp,.o,$(SRC))

all: $(OBJ)

$(OBJ): %.o : %.cpp
	$(MAKEVV)$(CC) $(FLAG) $< -o $@

clean : 
	rm -f *.o


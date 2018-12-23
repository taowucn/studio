CC 			:= g++
TRIP		:= strip
CFLAGS 		:= -Wall -Werror
LDFLAGS		:=
###############
SRCS		:= $(wildcard *.cpp)
OBJS		:= $(subst .cpp,.o,$(SRCS))
TARGETS	:= $(subst .cpp,.elf,$(SRCS))
###############

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %.elf : %.o
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@$(TRIP) $@
	@echo "Build $@ Done"

$(OBJS): %.o : %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -rf $(TARGETS) $(OBJS)

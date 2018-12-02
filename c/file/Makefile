CC 			:= gcc
TRIP		:= strip
CFLAGS 		:= -Wall -Werror

###############
SRCS		:= $(wildcard *.c)
OBJS		:= $(subst .c,.o,$(SRCS))
TARGETS	:= $(subst .c,.elf,$(SRCS))
###############

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %.elf : %.o
	@$(CC) -o $@ $< $(CFLAGS)
	@$(TRIP) $@
	@echo "Build $@ Done"

$(OBJS): %.o : %.c
	@$(CC) -c $< -o @

clean :
	@rm -rf $(TARGETS) $(OBJS) core

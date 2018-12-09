CC 			:= gcc
TRIP		:= strip
CFLAGS 		:= -Wall -Werror
LDFLAGS	:=
###############
SRCS		:= $(wildcard *.c)
OBJS		:= $(subst .c,.o,$(SRCS))
TARGETS	:= $(subst .c,.elf,$(SRCS))
###############

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %.elf : %.o
	@$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@$(TRIP) $@
	@echo "Build $@ Done"

$(OBJS): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	@rm -rf $(TARGETS) $(OBJS) core

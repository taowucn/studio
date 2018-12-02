CC 			:= gcc
TRIP		:= strip
CFLAGS 		:= -Wall -Werror

###############
SRCS		:= $(wildcard *.c)
OBJS		:= $(subst .c,.o,$(SRCS))
TARGETS	:= g.elf
###############

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)
	@$(TRIP) $@
	@echo "Build $@ Done"

$(OBJS): %.o : %.c
	@$(CC) -c $< -o $@

clean :
	@rm -rf $(TARGETS) $(OBJS) core

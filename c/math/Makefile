CC 			:= gcc
TRIP		:= strip
CFLAGS 		:= -Wall -Werror -lm

###############
OBJSDIR 	:= .objs
SRCS		:= $(wildcard *.c)
OBJS		:= $(subst .c,.o,$(SRCS))
TARGETS	:= $(subst .c,.elf,$(SRCS))
###############

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): %.elf : %.o
	@$(CC) -o $@ $(OBJSDIR)/$< $(CFLAGS)
	@$(TRIP) $@
	@echo "Build $@ Done"

$(OBJS): %.o : %.c
	@mkdir -p $(OBJSDIR)
	@$(CC) -c $< -o $(OBJSDIR)/$@

clean :
	@rm -rf $(TARGETS) $(OBJSDIR)

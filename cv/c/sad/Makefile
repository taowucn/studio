LIBDIR := /home/taowu/data/vmdisk/code/github/cv/opencv/opencv/build/lib
export LD_LIBRARY_PATH=/home/taowu/data/vmdisk/code/github/cv/opencv/opencv/build/lib:$LD_LIBRARY_PATH

CC 			:= g++
TRIP		:= strip
CFLAGS 		:= -O3 
LDFLAGS		:= -L$(LIBDIR) \
	-lopencv_highgui -lopencv_calib3d -lopencv_core -lopencv_imgproc -lopencv_imgcodecs \

#	-fsanitize=address -fno-omit-frame-pointer

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

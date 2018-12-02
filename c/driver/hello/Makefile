export BCMDIR=/home/taowu/data/code/github/raspberry/tools/arm-bcm2708/arm-linux-gnueabihf
export CCPREFIX=${BCMDIR}/bin/arm-linux-gnueabihf-
##CC := /usr/local/linaro-aarch64-2016.02-gcc5.3/bin/aarch64-linux-gnu-gcc

CC		:= ${CCPREFIX}gcc
obj-m	:= hello.o
hello-y	:= hello_drv.o 

KVERSION = $(shell uname -r)
#KDIR	:= /lib/modules/$(KVERSION)/build
KDIR 	:= /home/taowu/data/code/github/raspberry/linux
ccflags	+= ARCH=arm CROSS_COMPILE=${CCPREFIX}

all:
	make ${ccflags} -C $(KDIR) M=`pwd` modules
clean:
	make ${ccflags} -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order *.o

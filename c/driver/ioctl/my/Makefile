##CC := /usr/local/linaro-aarch64-2016.02-gcc5.3/bin/aarch64-linux-gnu-gcc
CC ?= gcc
obj-m := my_ioctl.o
my_ioctl-y := my_ioctl_drv.o 

KVERSION = $(shell uname -r)

KDIR := /lib/modules/$(KVERSION)/build

all:
	make -C $(KDIR) M=`pwd` modules
	$(CC) my_ioctl_app.c -o my_ioctl
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order *.o my_ioctl_app

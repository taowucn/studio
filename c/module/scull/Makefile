
PWD:=$(shell pwd)
KVERSION = $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build

scull-objs := main.o pipe.o access.o
obj-m	:= scull.o
#obj-m	:= main.o

modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions test




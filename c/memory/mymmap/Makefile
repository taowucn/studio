
ifeq ($(KERNELRELEASE),)

PWD :=$(shell pwd)
KERSRC := /lib/modules/$(shell uname -r)/build/

modules:
	$(MAKE) -C $(KERSRC) M=$(PWD) modules
moules_install:
	$(MAKE) -C $(KERSRC) M=$(PWD) modules_install
.PHONY: modules modules_install clean
clean:
	-rm -rf *.o *.cmd.* *.ko Module.symvers modules.order *.mod.c 
else
modules-objs :=mmap.o
obj-m := mmap.o
endif

app: 
	gcc -o test_mm test_mm.c -lpthread

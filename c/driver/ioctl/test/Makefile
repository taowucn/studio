obj-m += test.o

KVERSION = $(shell uname -r)

KDIR := /lib/modules/$(KVERSION)/build

all:
	gcc app.c -o app
	make -C $(KDIR) M=`pwd` modules
clean:
	make -C $(KDIR) M=`pwd` modules clean
	rm -f modules.order rm -f *.o

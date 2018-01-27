
obj-m += list.o
mod1-y := list.o

KVERSION = $(shell uname -r)

all: 
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
	rm -f *.o *.ko *.cmd

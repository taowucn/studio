obj-m += mod2.o
mod2-y := mod_b.o

KVERSION = $(shell uname -r)

all: 
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
	rm -f *.o *.ko *.cmd


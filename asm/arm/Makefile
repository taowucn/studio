export BCMDIR=/home/taowu/data/code/github/raspberry/tools/arm-bcm2708/arm-linux-gnueabihf
export CCPREFIX=${BCMDIR}/bin/arm-linux-gnueabihf-

CC	:=${CCPREFIX}gcc
AS	:=${CCPREFIX}as
LD	:=${CCPREFIX}ld

#/usr/lib/crtbegin_dynamic.o /usr/lib/crtend_android.o

all:
	$(AS) -k m.S -o m.o
	$(LD) m.o -l ${BCMDIR}/arm-linux-gnueabihf/sysroot/usr/lib/libc.so -pie -I /system/bin/linker -o m.out

clean:
	rm *.o

#ifndef __MY_CMD__
#define __MY_CMD__
#include <linux/ioctl.h>

typedef unsigned int u32;

#define MAX_BUFF_SIZE 100

#define MY_MAGIC_A 'A'
#define MY_MAGIC_B 'B'

#define MY_IOR(nr, size)	_IOR(MY_MAGIC_A, nr, size)
#define MY_IOW(nr, size)	_IOW(MY_MAGIC_A, nr, size)

#define MY_IO(nr)			_IO(MY_MAGIC_B, nr)

typedef enum {
	SET=0x0001,
	GET=0x0002,
	INFO=0X0003,
} IOC;

#define MY_IO_SET	MY_IOW(SET, int)
#define MY_IO_GET	MY_IOR(GET, int *)

#define MY_IO_INFO	MY_IO(INFO)

#endif


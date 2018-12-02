
#ifndef __POOL_UTIL_H__
#define __POOL_UTIL_H__

#include <stdio.h>

#define ERROR(_str_, _arg_...) do { \
	printf( _str_, ##_arg_); \
}while(0)

#endif

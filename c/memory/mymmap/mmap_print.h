#ifndef __MMAP_PRINT_H__
#define __MMAP_PRINT_H__

enum {
	LOG_ERR = 0,
	LOG_INFO = 1,
	LOG_DEBUG = 2,
};

extern uint32_t debug_level;

#define prt_err(str, arg...)	do { \
	if (LOG_ERR <= debug_level ) {\
		printk(KERN_ERR "%s(%d): "str, __func__, __LINE__, ##arg); \
	} \
} while (0)

#define prt_info(str, arg...)	do { \
	if (LOG_INFO <= debug_level ) { \
		printk(KERN_INFO "%s(%d): "str, __func__, __LINE__, ##arg); \
	} \
} while (0)

#define prt_debug(str, arg...)	do { \
	if (LOG_DEBUG <= debug_level ) {\
		printk(KERN_DEBUG "%s(%d): "str, __func__, __LINE__, ##arg); \
	} \
} while (0)

#endif

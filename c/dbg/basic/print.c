#include <stdio.h>
#include <stdint.h>

#define LOG(fmt, args) do { \
	printf("%s " fmt, __func__, args); \
	printf("\n"); \
} while(0)

static char *name = "abc";

struct idata {
	uint32_t a : 1;
	uint32_t b : 1;
	uint32_t c : 1;
	uint32_t d : 1;
	uint32_t e : 1;
	uint32_t res : 27;
};

int main(int argc, char **argv)
{
	int i= 0x12345678;
	char *p = (char *)&i;
	uint8_t bitmap = 0x11;
	struct idata d;

	LOG("%s", name);
	printf("p=%x, p+1=%x\n", *p, *(p+1));

	d.a = (bitmap & (1 << 0)) >> 0;
	d.b = (bitmap & (1 << 1)) >> 1;
	d.c = (bitmap & (1 << 2)) >> 2;
	d.d = (bitmap & (1 << 3)) >> 3;
	d.e = (bitmap & (1 << 4)) >> 4;

	printf("bitmap: 0x%x, data: (%d, %d, %d, %d, %d)\n",
		bitmap, d.e, d.d, d.c, d.b, d.a);

	return 0;
}

#include <stdio.h>

#define LOG(fmt, args) do {printf("%s " fmt, __func__, args);printf("\n");} while(0)

static char *name = "abc";

int main(int argc, char **argv)
{
	int i= 0x12345678;
	char *p = (char *)&i;

	LOG("%s", name);
	printf("p=%x, p+1=%x\n", *p, *(p+1));

	return 0;
}

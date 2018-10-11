#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int bssvar;

int main(int argc, char ** argv)
{
	void *ptr;
	char *mem;

	printf("main start = %p\n", main);
	printf("bss end = %ld\n", (long)&bssvar + 4);

	ptr = sbrk(0);
	printf("current brk = %p\n", (long*)ptr);
	sleep(8);

	mem = malloc(10);
	if (mem) {
		free(mem);
	}

	return 0;
}

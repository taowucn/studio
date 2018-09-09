#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "cma_mem_ioctl.h"

int main(int argc, char *argv[])
{
	struct mem_block region;
	void *cmem_base = NULL;
	char str[10];
	unsigned long size = 0;
	int cmem_fd = 0;
	int i = 0, j = 0;
	int rval = 0;

	cmem_fd = open("/dev/cma_mem", O_RDWR, 0);
	if (cmem_fd < 0) {
		perror("open cma_mem");
		return -1;
	}
	printf("cmem_fd: %d\n", cmem_fd);

	memset(&region, 0, sizeof(struct mem_block));
	printf("sizeof struct mem_block: %lu\n", sizeof(struct mem_block));

	j = 0;
	while (j <= 2) {
		j++;
		sprintf(str, "mem_%d", j);
		memset(&region, 0, sizeof(struct mem_block));
		region.len = 4 << 10; // 4K one page
		region.is_use_buffer = 1;
		strncpy(region.name, str, strlen(str));
		if (ioctl(cmem_fd, CMEM_ALLOCATE, &region) < 0) {
			perror("CMEM_ALLOCATE");
			rval =  -1;
			break;
		}
		printf("base: 0x%lx, len: 0x%lx, offset: 0x%lx\n", region.mem_base, region.len, region.offset);

		for (i = 0; i < 8; i++) {
			printf("%d, ", ((unsigned int *)(region.mem_base))[i]);
		}
		printf("\n");

		#if 0
		cmem_base = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, cmem_fd, 0);
		if (cmem_base == MAP_FAILED) {
			perror("mmap");
			rval = -1;
			break;
		}
		#endif

		for (i = 0; i < 8; i++) {
			((unsigned int *)region.mem_base)[i] = i;
		}
		for (i = 0; i < 8; i++) {
			printf("%d, ", ((unsigned int *)region.mem_base)[i]);
		}
		printf("\n");
		printf("*********************\n");
   	}
	getchar();

	#if 1
	printf("Free all mem\n");
	j = 0;
	while (j <= 2) {
		j++;
		sprintf(str, "mem_%d", j);
		memset(&region, 0, sizeof(struct mem_block));
		region.id = j;
		region.is_use_buffer = 1;
		strncpy(region.name, str, strlen(str));
		printf("user free mem: %s, id: %d\n", str, region.id);
		if (ioctl(cmem_fd, CMEM_UNMAP, &region) < 0) {
			perror("CMEM_UNMAP");
			rval = -1;
			break;
		}
	}
	getchar();
	#endif

	if (cmem_fd > 0) {
		close(cmem_fd);
		cmem_fd = -1;
	}

	return rval;
}

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "cma_mem_ioctl.h"

int main(int argc, char *argv)
{
	struct cmamem_info region;
	void *cmem_base = NULL;
	unsigned long size = 0;
	int cmem_fd = 0;
	int i = 0, rval = 0;

	cmem_fd = open("/dev/cma_mem", O_RDWR, 0);
	if (cmem_fd < 0) {
		perror("open");
		return -1;
	}

	printf("cmem_fd: %d\n", cmem_fd);
	size = 4 << 10; // 4K one page
	memset(&region, 0, sizeof(struct cmamem_info));
	region.len = size;

	do {
		if (ioctl(cmem_fd, CMEM_ALLOCATE, &region) < 0) {
			perror("CMEM_ALLOCATE");
			rval = -1;
			break;
		}

		cmem_base = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, cmem_fd, 0);
		if (cmem_base == MAP_FAILED) {
			perror("mmap cma");
			rval = -1;
			break;
		}
		printf("virt_addr: %p, size: 0x%lx, offset: 0x%lx\n",
			cmem_base, size, region.offset);

		for (i = 0; i < 4; i++) {
			((unsigned int *)cmem_base)[i] = i;
		}

		for (i = 0; i < 4; i++) {
			printf("%d\n", ((unsigned int *)cmem_base)[i]);
		}
	} while (0);

	if (cmem_fd > 0) {
		close(cmem_fd);
		cmem_fd = -1;
	}

	return rval;
}

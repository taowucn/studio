#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096

int main(int argc , char *argv[])
{
	int fd;
	int i;
	unsigned char *p_map;

	fd = open("/dev/mymap",O_RDWR);
	if(fd < 0) {
		printf("open fail\n");
		exit(1);
	}

	p_map = (unsigned char *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
	if(p_map == MAP_FAILED) {
		printf("mmap fail\n");
		goto here;
	}

	for(i=0;i<10;i++) {
		printf("%d\n",p_map[i]);
		p_map[i] = i + 10;
	}

here:
	munmap(p_map, PAGE_SIZE);

	return 0;
}

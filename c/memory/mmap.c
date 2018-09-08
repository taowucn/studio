#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SIZE (4)

int main(int argc, char **argv)
{
	int fd = 0;
	char *start = NULL;
	char name[120] = "Hello, mmap";

	fd = open("/tmp/mm", O_RDONLY, 0666);
	if (fd < 0 ){
		perror("open");
		return -1;
	}

	start = mmap (NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (start == MAP_FAILED) {
		perror("mmap");
		return -1;
	} else {
		printf("Virt addr: %p\n", start);
		memcpy(name, start, 4);
		printf("name: %s\n", name);
		munmap(start, SIZE);
	}
	if (fd >= 0) {
		close(fd);
		fd = -1;
	}

	return 0;
}

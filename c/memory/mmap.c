#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

static int DATA =1024 * 1024;

int main(int argc, char ** argv)
{
	int fd;
	char *start;
	char name[120]="taowu";
	if ((fd = open("/tmp/sda1/data", O_CREAT | O_TRUNC | O_RDWR, 0666)) < 0 ){
		perror("open");
		return ;
	}

	start = mmap (NULL, DATA, PROT_READ, MAP_PRIVATE, fd, 0);
	if (start == MAP_FAILED)
		return;
	munmap(start, DATA);
	close(fd);
}

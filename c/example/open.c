#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd, len, size;
	char *buf = "hello wto";
	char buf_read[10] = {0};

	len = strlen(buf);
	fd = open("/tmp/open.log", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if(fd < 0) {
		perror("open");
		return -1;
	} else {
		printf("open %d file sucessed\n", fd);
	}

	do {
		size = write(fd, buf, len);
		if (size < 0) {
			perror("write");
			break;
		} else {
			printf("sucess write: %s\n", buf);
		}

		lseek(fd,0,SEEK_SET);
		size = read(fd,buf_read,10);
		if (size < 0) {
			perror("read");
			break;
		} else {
			printf("read %d byte from file: %s\n", size, buf_read);
		}
	} while (0);

	if (fd >= 0) {
		close(fd);
		fd = -1;
	}

	return 0;
}

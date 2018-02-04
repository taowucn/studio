#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define FIFO "/tmp/myfifo"

int main(int argc,char *argv[])
{
	int fd = 0;
	int nread = 0;
	char buf_r[100] = {0};

	if (access(FIFO, F_OK) < 0) {
		perror("access");
		return -1;
	} else {
		printf("fifo ok\n");
	}
	memset(buf_r,0,sizeof(buf_r));

	fd = open(FIFO, O_RDONLY, 0);
	if (fd < 0) {
		perror("open");
		exit(0);
	}

	memset(buf_r, 0, sizeof(buf_r));
	nread = read(fd, buf_r, sizeof(buf_r));
	if (nread < 0) {
		if (errno == EAGAIN) {
			printf("no data yet\n");
		} else {
			perror("read");
		}
	} else {
		printf("read %s from fifo\n", buf_r);
	}

	close(fd);
	fd = -1;
	return 0;
}

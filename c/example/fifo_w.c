#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define FIFO "/tmp/myfifo"

int main(int argc, char *argv[])
{
	int fd = 0;
	int nwrite = 0;
	char buf_w[100] = {0};

	if (argc == 1) {
		printf("please send something\n");
		return -1;
	}

	if((mkfifo(FIFO, 0644) < 0 )) {
		perror("mkfifo");
		return -1;
	}
	printf("Create FIFO OK\n");

	strncpy(buf_w, argv[1], sizeof(buf_w));
	fd = open(FIFO, O_WRONLY, 0777);
	nwrite = write(fd, buf_w, strlen(buf_w));
	if (nwrite < 0) {
		if (errno == EAGAIN) {
			printf("the fifo has not been read yet\n");
			return -1;
		}
	} else {
		printf("write %s to fifo ok\n", buf_w);
	}
	unlink(FIFO);

	return 0;
}


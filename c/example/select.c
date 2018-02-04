#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int fd[2] = {0};
	int maxfd, readc, writec;
	char buf[7] = "hello!\n";
	fd_set inset1, inset2;
	struct timeval tv;

	fd[0] = open("/tmp/hello1", O_CREAT | O_RDWR, 0666);
	if (fd[0] < 0) {
		perror("open hello1");
		return -1;
	}
	fd[1] = open("/tmp/hello2", O_CREAT | O_RDWR, 0666);
	if (fd[1] < 0) {
		perror("open hello2");
		return -1;
	}
	printf("open hello1 and hello2 ok\n");

	maxfd = fd[0] > fd[1] ? fd[0] : fd[1];
	writec = write(fd[0], buf, sizeof(buf));
	if (writec < 0) {
		perror("write hello1");
	} else {
		printf("write %d byte to hello1\n", writec);
	}

	lseek(fd[0], 0, SEEK_SET);
	FD_ZERO(&inset1);
	FD_SET(fd[0], &inset1);
	FD_ZERO(&inset2);
	FD_SET(fd[1], &inset2);
	tv.tv_sec = 2;
	tv.tv_usec = 0;

	while (FD_ISSET(fd[0], &inset1) || FD_ISSET(fd[1], &inset2)) {
		if (select(maxfd+1, &inset1, &inset2, NULL, &tv) < 0) {
			perror("select");
			break;
		} else {
			if (FD_ISSET(fd[0], &inset1)) {
				if((readc = read(fd[0], buf, 7)) < 0) {
					perror("read");
				} else {
					buf[7] = '\0';
					printf("read %d byte from hello1: %s\n",readc,buf);
				}
			}
			if (FD_ISSET(fd[1], &inset2)) {
				if((writec = write(fd[1], buf, 7)) < 0) {
					perror("write");
				} else {
					buf[7]='\0';
					printf("write %d byte to hello2: %s\n",writec,buf);
				}
				sleep(10);
			}
			buf[0] = '\0';
		}
	}

	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include<sys/mman.h>

#include "my_cmd.h"

#define MY_DEV "/dev/mydrv"

int main(int argc, char **argv)
{
	int ret = 0;
	int fd = -1;
	int value = 0;
	char buf[10];
	char *buf_user;
	char *buf_start;

	fd = open(MY_DEV, O_RDWR);
	if (fd < 0) {
		perror("open");
		return 0;
	}
	printf("Open OK, fd [%d]\n", fd);

	/* write/read */
	do {
		ret = write(fd, "abc", 3);
		if (ret < 0) {
			perror("write");
			break;
		}
		ret = write(fd, "ef", 2);
		if (ret < 0) {
			perror("write");
			break;
		}
		memset(buf, 0, sizeof(buf));
		lseek(fd, 0, SEEK_SET);
		ret = read(fd, buf, sizeof(buf));
		if (ret < 0) {
			perror("read");
			break;
		}
		printf("read buf[%s]\n", buf);
		value = 123;
		ret = ioctl(fd, MY_IO_SET, value);
		if (ret < 0) {
			perror("ioctl");
			break;
		}
		value = 456;
		ret = ioctl(fd, MY_IO_GET, &value);
		if (ret < 0) {
			perror("ioctl");
			break;
		}

		printf("value[%d]\n", value);
	} while(0);

	/* mmap */
	do {
		buf_user = (char *)malloc(MAX_BUFF_SIZE);
		if (!buf_user) {
			perror("malloc");
			break;
		}
		memset(buf_user, 0, MAX_BUFF_SIZE);
		buf_start = mmap(NULL, MAX_BUFF_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		if (!buf_start) {
			perror("mmap");
			break;
		}
		printf("buf_start addr[%p]\n", buf_start);
	} while(0);

	strcpy(buf_user ,buf_start);
	printf("Buf User 1: %s\n", buf_user);

	strcpy(buf_user, "Hello world~");
	strcpy(buf_start, buf_user);

	memset(buf_user, 0, MAX_BUFF_SIZE);
	strcpy(buf_user, buf_start);
	printf("Buf User 2: %s\n", buf_user);

	ret = ioctl(fd, MY_IO_INFO, &value);
	if (ret < 0) {
		perror("ioctl");
	}

	/* close */
	munmap(buf_start, MAX_BUFF_SIZE);
	free(buf_user);
	close(fd);
	fd = -1;

	return 0;
}

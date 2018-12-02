#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "test_cmd.h"

int main(void)
{
	char buf[20];
	int fd;
	int ret;

	struct ioctl_data my_data= {
		.size = 10,
		.buf = "123456789"
	};

	fd = open("/dev/test", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	write(fd, "xiao bai", 10);

	ret = ioctl(fd, TEST_KBUF, (void *)&my_data);
	if(ret < 0)
	{
		perror("ioctl");
	}

	ret = read(fd, buf, 10);
	printf("<app> buf is [%s]\n", buf);
	if(ret < 0)
	{
		perror("read");
	}

	close(fd);
	return 0;
}

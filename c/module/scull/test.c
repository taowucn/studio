#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
	char buffer1[]="hello,lingd!";
	char buffer2[10];
	int fd_scull;
	int ret = 0;
	int size = sizeof(buffer1) / sizeof(char) - 1;

	fd_scull = open("/dev/scull0", O_WRONLY);

	while (size > 0)
	{
		ret = write(fd_scull, buffer1 + ret, size);
		if (ret < 0)
		{
			perror("write error");
			exit(1);
		}

		printf("write size: %d\n", ret);
		size = size - ret;
	}

 	close(fd_scull);
	fd_scull = open("/dev/scull0", O_RDONLY);

	size = sizeof(buffer1) / sizeof(char) - 1;
	while (size > 0)
	{
		ret = read(fd_scull, buffer2, size);
		if (ret < 0)
		{
			perror("read error");
			exit(1);
		}

		buffer2[ret] = '\0';
		printf("read: %s\n", buffer2);
		size = size - ret;
	}

 	close(fd_scull);

	return 0;
}

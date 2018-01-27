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
	char buf_r[100];
	int fd;
	int nread;
	if((mkfifo(FIFO,O_CREAT | O_EXCL)<0)&&(errno!=EEXIST))
	{	printf("create fifo error! ");	}
	printf("fifo create sucessed!");
	memset(buf_r,0,sizeof(buf_r));
	fd=open(FIFO,O_RDONLY | O_NONBLOCK,0);
	if(fd==-1)
	{
		perror("open");
		exit(0);
	}
	while(1)
	{
		memset(buf_r,0,sizeof(buf_r));
		if(nread==read(fd,buf_r,100)==-1)
		{
			if(errno==EAGAIN)
				printf("no data yet\n");

		}
		printf("read %s from fifo\n",buf_r);
		sleep(1);
	}
	pause();
	unlink(FIFO);


}

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#define FIFO_SVR "/tmp/myfifo"

main(int argc,char *argv[])
{
	int fd;
	int nwrite;
	char buf_w[100];
	if(fd==-1)
		if(errno==ENXIO)
			printf("open error:no read process!\n");
	fd=open(FIFO_SVR,O_WRONLY | O_NONBLOCK,0);
	if(argc==1)
		printf("please send something\n");
	strcpy(buf_w,argv[1]);
	if((nwrite=write(fd,buf_w,100)==-1))
	{
		if(errno==EAGAIN)
			printf("the fifo has not been read yet\n");
	}
	else
		printf("write %s to fifo",buf_w);


}


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
int main()
{
	int fd,len,size;
	char *buf="hello wto";
	char buf_read[10];
	buf_read[10]='\0';
	len=strlen(buf);
	if((fd=open("/tmp/open.log",O_CREAT | O_RDWR | O_TRUNC ,0666))<0)
	{
		perror("open");
		exit(1);
	}
	else
		printf("open %d file sucessed \n",fd);
	if((size=write(fd,buf,len))<0)
	{
		perror("write");
		exit(1);
	}
	else
		printf("sucess write :%s ;in file\n",buf);
	lseek(fd,0,SEEK_SET);
	if((size=read(fd,buf_read,10)),0)
	{
		perror("read");
		exit(1);
	}
	else
	{
		printf("read %d num char from file ,string is %s\n",size,buf_read);
	}

	if(close(fd)<0)
	{
		perror("close");
		exit(1);
	}
	else
		printf("sucess close file\n");
	exit(0);
}

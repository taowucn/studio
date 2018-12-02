#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAXFILE 65535

int main()
{
	pid_t pc;
	int len,fd,i;
	char *buf="There is a dameon\n";
	len=strlen(buf);
	pc=fork();
	if(pc<0)
	{
		printf("fork error!");
		exit(1);
	}
	else if(pc>0)
	{
		printf("fork pid is %d \n",pc);
		exit(0);
	}
	setsid();
	chdir("/");
	umask(0);
	for(i=0;i<MAXFILE;i++)
		close(i);
	while(1)
	{
		if(fd=open("/tmp/dameon.log", O_CREAT | O_RDONLY | O_APPEND, 0600)<0)
		{
			perror("open");
			exit(1);
		}
		write(fd,buf,len+1);
		close(fd);
		sleep(2);
	}
}

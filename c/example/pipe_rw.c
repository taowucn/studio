#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
	int pipe_fd[2];
	pid_t pid;
	char buf_r[100];
	char *buf_w;
	int num_return;
	memset(buf_r,0,sizeof(buf_r));

	if(pipe(pipe_fd)<0)
	{
		printf("pipe create error!");
		return -1;
	}
	if(pid=fork()<0)
	{
		printf("create child process error!");
		return -1;
	}
	else if(pid==0)
	{
		printf("\n");
		close(pipe_fd[1]);
		sleep(2);
		if(num_return=read(pipe_fd[0],buf_r,100)>0)
		{
			printf("child:read %d num from pipe char is %s\n",num_return,buf_r);
		}
		close(pipe_fd[0]);
		exit(0);
	}
	else
	{
		printf("\n");
		sleep(2);
		close(pipe_fd[0]);
		if(write(pipe_fd[1],"hello",5)!=-1)
			printf("parent: write1 sucessed!");
		if(write(pipe_fd[1],"wto",3)!=-1)
			printf("parent: write2 sucessed!");
		close(pipe_fd[1]);
		sleep(3);
		waitpid(pid,NULL,0);
		exit(0);
	}

}

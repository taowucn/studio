#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	pid_t result;
	result = fork();
	if(-1 == result)
		{
		 	perror("fork");
			exit;
		}
	else if(0 == result)
		{
			printf("result is %d \n proess in child\n pid is %d!\n",result,getpid());
			//return 0;
		}
	else
		{
			printf("result is %d \n process in parent\n pid is %d!\n",result,getpid());
			//return 0;
		}

}

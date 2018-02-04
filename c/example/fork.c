#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	pid_t result;
	result = fork();

	if (result < 0) {
	 	perror("fork");
		return -1;
	} else if(0 == result) {
		printf("result is %d \n proess in child, pid is %d!\n",result, getpid());
	} else {
		printf("result is %d \n process in parent, pid is %d!\n",result, getpid());
	}

	return 0;
}

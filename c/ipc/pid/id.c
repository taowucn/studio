#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	printf("process id=%d\n",getpid());
	printf("parent process id=%d\n",getppid());
	printf("process group id=%d\n",getpgrp());
	printf("calling process's real user id=%d\n",getuid());
	printf("calling process's real group id=%d\n",getgid());
	printf("calling process's effective user id=%d\n",geteuid());
	printf("calling process's effective group id=%d\n",getegid());

	return 0;
}
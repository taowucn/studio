#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int pid = 0;
	int ret = 0;
	int status = 0;

	printf("fork start\n");
	pid = fork();
	if (pid == 0) {
		printf("Child: pgrp=%d, ppid=%d, pid=%d\n", getpgrp(), getppid(), getpid());
		printf("sleep...\n");
		sleep(3);
		printf("sleep end\n");
		exit(0);
	} else {
		printf("Parent: pgrp=%d, ppid=%d, pid=%d\n", getpgrp(), getppid(), getpid());
		printf("wait...\n");
		#if 0
		ret = wait(&status);
		#else
		ret = waitpid(pid, &status, 0);
		#endif
		printf("wait end, ret=%d, status=%d\n", ret, status);
		exit(0);
	}
}

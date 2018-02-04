#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int ret;
	pid = fork();
	if (pid < 0) {
		perror("fork");
		return -1;
	}
	if (pid == 0) {
		raise(SIGSTOP);
		return 0;
	} else {
		printf("Parent process, pid is %d\n", pid);
		if (waitpid(pid, NULL, WNOHANG) == 0) {
			ret = kill(pid, SIGKILL);
			if (ret == 0) {
				printf("kill pid %d\n", pid);
			} else {
				perror("kill");
			}
			waitpid(pid, NULL, 0);
			return 0;
		}
	}
}

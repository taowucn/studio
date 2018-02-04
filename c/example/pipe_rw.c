#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int pipe_fd[2];
	char buf_r[100] = {0};
	char *buf_w = NULL;
	int ret = 0;

	if (pipe(pipe_fd) < 0) {
		perror("pipe");
		return -1;
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		printf("This Children\n");
		close(pipe_fd[1]);
		//sleep(2);
		ret = read(pipe_fd[0], buf_r, 100);
		if (ret > 0) {
			printf("Child: read %d byte pipe: %s\n",ret, buf_r);
		}
		close(pipe_fd[0]);
		return 0;
	} else {
		printf("This Parent\n");
		//sleep(2);
		close(pipe_fd[0]);
		if (write(pipe_fd[1], "hello", 5) > 0) {
			printf("Parent: write1 ok\n");
		}
		if (write(pipe_fd[1],"wto", 3) > 0) {
			printf("parent: write2 ok\n");
		}

		close(pipe_fd[1]);
		//sleep(1);
		waitpid(pid, NULL, 0);
		return 0;
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 1024

int main(int argc, char *argv[])
{
	int shmid = 0;
	int pid = 0;
	char *shm_w, *shm_r;

	shmid = shmget(IPC_PRIVATE, BUFSIZE, 0666);
	if (shmid < 0) {
		perror("shmget");
		return -1;
	} else {
		printf("create a share memory %d\n", shmid);
		//system("ipcs -m");
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		shm_w = shmat(shmid, NULL, 0);
		if (shm_w == NULL) {
			perror("shmat");
			return -1;
		} else {
			strncpy(shm_w, "123abc", 6);
			printf("attach a share memory\n");
			system("ipcs -m > /tmp/1");
		}

		if (shmdt(shm_w) < 0) {
			perror("shmdt");
			return -1;
		} else {
			printf("sucess delete share memory\n");
			system("ipcs -m > /tmp/2");
		}
	} else {
		sleep(1);
		shm_r = shmat(shmid, NULL, 0);
		if (shm_r == NULL) {
			perror("shmat");
			return -1;
		} else {
			printf("Read memory: %s\n", shm_r);
		}
		waitpid(pid, NULL, 0);

		if (shmdt(shm_r) < 0) {
			perror("shmdt");
			return -1;
		} else {
			printf("sucess delete share memory\n");
			system("ipcs -m > /tmp/2");
		}
	}

	return 0;
}

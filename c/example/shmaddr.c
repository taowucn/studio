#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#define BUFSIZE 1024
int main()
{
	int shmid;
	char *shmaddr;

	if((shmid=shmget(IPC_PRIVATE,BUFSIZE,0666))<0)
	{
		perror("shmget");
		exit(1);
	}
	else
	{
		printf("create a share memory %d \n",shmid);
		system("ipcs -m");
	}
	if((shmaddr=shmat(shmid,0,0))<(char *)0)
	{
		perror("shmat");
		exit(1);
	}
	else
	{
		printf("attach a share memory");
		system("ipcs -m");
	}
	if(shmdt(shmaddr)<0)
	{
		perror("shmdt");
		exit(1);
	}
	else
	{
		printf("sucess delete share memory");
		system("ipcs -m");
	}
	exit(0);
}

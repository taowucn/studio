#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int val; /* value for SETVAL */
	struct semid_ds *buf; /* buffer for IPC_STAT, IPC_SET */
	unsigned short *array; /* array for GETALL, SETALL */
	struct seminfo *__buf; /* buffer for IPC_INFO */
};

int P(int semid, int semnum)
{
	struct sembuf sops = {semnum, -1, SEM_UNDO};
	return (semop(semid, &sops, 1));
}

int V(int semid, int semnum)
{
	struct sembuf sops = {semnum, +1, SEM_UNDO};
	return (semop(semid, &sops, 1));
}

int main(int argc, char **argv)
{
	int key;
	int semid, ret;
	union semun arg;
	struct sembuf semop;
	int flag;

	key = ftok("/tmp", 0x66 );
	if (key < 0) {
		perror("ftok key error") ;
		return -1 ;
	}

	semid = semget(key, 3, IPC_CREAT | 0600);
	if (semid == -1) {
		perror("create semget error");
		return -1;
	}

	if (argc == 1) {
		arg.val = 1;
		ret = semctl(semid,0,SETVAL,arg);
		if (ret < 0) {
			perror("ctl sem error");
			semctl(semid,0,IPC_RMID,arg);
			return -1 ;
		}
	}

	ret = semctl(semid, 0, GETVAL, arg);
	printf("after semctl setval sem[0].val =[%d]\n",ret);
	system("date") ;
	printf("P operate begin\n") ;
	flag = P(semid, 0);
	if (flag) {
		perror("P operate error") ;
		return -1 ;
	}
	printf("P operate end\n") ;
	ret = semctl(semid,0,GETVAL,arg);
	printf("after P sem[0].val=[%d]\n",ret);
	system("date") ;
	if (argc == 1) {
		sleep(1);
	}
	printf("V operate begin\n") ;
	if (V(semid, 0) < 0) {
		perror("V operate error") ;
		return -1 ;
	}
	printf("V operate end\n") ;
	ret = semctl(semid, 0, GETVAL, arg);
	printf("after V sem[0].val = %d\n", ret);
	system("date") ;
	if ( argc > 1) {
		semctl(semid, 0, IPC_RMID, arg);
	}

	return 0;
}

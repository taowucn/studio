#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>

typedef union semun
{
    int	val;
    struct semid_ds *buf;
    ushort *array;
} SEMCTL_UNION;

int main(int argc, char *argv)
{
    int n,semid;
    key_t semkey;
    SEMCTL_UNION semctl_arg;
    struct sembuf buf;
    if((semkey = ftok("/etc/profile",1)) < 0)
    {
        perror("ftok");
        exit(1);
    }
    if((semid = semget(semkey,1,0)) < 0)
    {
        perror("semget");
        exit(2);
    }
    semctl_arg.val = 2;
    if (semctl(semid,0,SETVAL,semctl_arg) < 0)ʼֵ
    {
        perror("semctl");
        exit(3);
    }
    memset(&buf,0x00,sizeof(struct sembuf));
    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = IPC_NOWAIT;

    if(semop(semid,&buf,1) == -1)
        {
            perror("semop");
        }

    buf.sem_op = -1;
    for(n=0;;n++)
    {
        if(semop(semid,&buf,1) == -1)
        {
            perror("semop");
            break;
        }
        printf("semop[%d]:current semphore value=%d\n",
        		n,semctl(semid,0,GETVAL,semctl_arg));
    }
}

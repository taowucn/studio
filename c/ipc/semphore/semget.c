#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
main()
{
    int semid;
    key_t semkey;
    if((semkey = ftok("/etc/profile",1)) < 0)
    {
        perror("ftok");
        exit(1);
    }
    if((semid = semget(semkey,1,IPC_CREAT|0666)) < 0)//创建包含信号量的信号集，权限666，信号量集中的信号量数目是1个
    {
        perror("semget");
        exit(1);
    }
    printf("semid=%d\n",semid);
}
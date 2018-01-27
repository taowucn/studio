#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>
typedef union semun//semctl需要的
{
    int	val;//保存信号量值
    struct semid_ds *buf;//信号量控制结构指针
    ushort *array;//无符号短整形变量
} SEMCTL_UNION;

main()
{
    int n,semid;//信号量标示符变量
    key_t semkey;//键值变量
    SEMCTL_UNION semctl_arg;//联合类型变量
    struct sembuf buf;//semop调用所需的结构变量
    if((semkey = ftok("/etc/profile",1)) < 0)//创建键值
    {
        perror("ftok");
        exit(1);
    }
    if((semid = semget(semkey,1,0)) < 0)//创建信号量
    {
        perror("semget");
        exit(2);
    }
    semctl_arg.val = 2;//初始化
    if (semctl(semid,0,SETVAL,semctl_arg) < 0)//设置信号量初始值
    {
        perror("semctl");
        exit(3);
    }
    memset(&buf,0x00,sizeof(struct sembuf));//清空
    buf.sem_num = 0;//信号量序号从0开始，第一个
    buf.sem_op = 1;//P操作，所以-1
    buf.sem_flg = IPC_NOWAIT;//非阻塞

    if(semop(semid,&buf,1) == -1)//P操作
        {
            perror("semop");
        }

    buf.sem_op = -1;//P操作，所以-1
    for(n=0;;n++)//循环调用P操作，直到信号量变为0
    {
        if(semop(semid,&buf,1) == -1)//P操作
        {
            perror("semop");
            break;
        }
        printf("semop[%d]:current semphore value=%d\n",n,semctl(semid,0,GETVAL,semctl_arg));
    }
}
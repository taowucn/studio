#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
typedef struct
{
    int n;
    char str[256];
} ShmStru;

main( int argc, char ** argv)
{
    key_t lKey;
    int nShmId;
    ShmStru *pstru;
    if((lKey = ftok("/etc/profile",2)) < 0)
    {
        perror("ftok");
        exit(1);
    }
    if((nShmId = shmget(lKey,sizeof(ShmStru),IPC_CREAT|0666)) == -1)//创建共享内存
    {
        perror("shmget");
        exit(2);
    }
    if((pstru = shmat(nShmId,NULL,0)) == (void *)-1)//映射共享内存到本地
    {
        perror("shmat");
        exit(3);
    }

    if ( !strncmp(argv[1], "w", 10)) {
        pstru->n = 1;//修改共享内存
        strcpy(pstru->str,"123456");//向共享内存写入数据
        printf("write ok\n");
    }else if (!strncmp(argv[1], "r", 10)) {
        printf("read str=%s\n", pstru->str);
    }

    if( shmdt(pstru) == -1)//解除共享内存映射
    {
        perror("shmdt");
        exit(4);
    }
    return 0;
}
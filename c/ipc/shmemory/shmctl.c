#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
typedef struct
{
    int n;
    char str[256];
} ShmStru;
main()
{
    key_t lKey;
    int nShmId;
    struct shmid_ds sds;
    if((lKey = ftok("/etc/profile",3)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if((nShmId = shmget(lKey,sizeof(ShmStru),IPC_CREAT|0666)) == -1)
    {
        perror("shmget");
        exit(2);
    }
    memset(&sds,0x00,sizeof(struct shmid_ds));
    if(shmctl(nShmId,IPC_STAT,&sds) < 0)//获取共享内存属性
    {
        perror("shmctl IPC_STAT");
        exit(3);
    }
    printf("First shm_perm.mode=0%o\n",sds.shm_perm.mode);
    sds.shm_perm.mode &= (~0002);//去除其他用户的写权限
    if(shmctl(nShmId,IPC_SET,&sds) < 0)
    {
        perror("shmctl IPC_SET");
        exit(4);
    }
    memset(&sds,0x00,sizeof(struct shmid_ds));
    if(shmctl(nShmId,IPC_STAT,&sds) < 0)
    {
        perror("shmctl IPC_STAT");
        exit(5);
    }
    printf("Second shm_perm.mode=0%o\n",sds.shm_perm.mode);//输出共享内存的访问权限信息
    if(shmctl(nShmId,IPC_RMID,NULL) < 0)//删除共享内存
    {
        perror("shmctl IPC_RMID");
        exit(6);
    }
    return 0;
}
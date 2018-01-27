#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

main()
{
    key_t lKey;
    int nShmId;
    if((lKey = ftok("/etc/profile",1)) < 0)
    {
        perror("ftok");
        exit(1);
    }
    if((nShmId = shmget(lKey,256,IPC_CREAT|0666)) == -1)//´´½¨
    {
        perror("shmget");
        exit(2);
    }
    printf("Shmid=%d\n",nShmId);
    return 0;
}
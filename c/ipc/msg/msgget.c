#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(int argc, char **argv)
{
    key_t lKey;
    int nMsgId;
    if((lKey = ftok("/etc/profile",1)) == -1)
    {
        perror("ftok");
        exit(1);
    }
	//带参数IPC_CREAT和IPC_EXCL，如果队列不存在则创建队列，已存在则返回EEXIST
    if((nMsgId = msgget(lKey,IPC_CREAT|IPC_EXCL|0666)) == -1)
    {
        if(errno != EEXIST)//创建失败且不是由于队列已存在
        {
            perror("msgget");
            exit(2);
        }
        if((nMsgId = msgget(lKey,0)) == -1)//已存在
        {
            perror("msgget");
            exit(3);
        }
    }
    printf("MsgID=%d\n",nMsgId);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
typedef struct
{
    long int nType;
    char szText[256];
}MSG;

main()
{
    key_t lKey;
    int n,nMsgId;
    MSG msg;
    if((lKey = ftok("/etc/profile",1)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if((nMsgId = msgget(lKey,0)) == -1)
    {
        perror("ftok");
        exit(2);
    }
    memset(&msg,0x00,sizeof(MSG));
    if((n = msgrcv(nMsgId,(void *)&msg,sizeof(msg.szText),2L,0)) < 0)//从队列接收消息，读出以后就不存在了
    {
        perror("msgrcv");
    }
    else
    {
        printf("msgrcv return length=[%d] text=[%s]\n",n,msg.szText);//输出
    }
    return 0;
}
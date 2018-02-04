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
	//������IPC_CREAT��IPC_EXCL��������в������򴴽����У��Ѵ����򷵻�EEXIST
    if((nMsgId = msgget(lKey,IPC_CREAT|IPC_EXCL|0666)) == -1)
    {
        if(errno != EEXIST)//����ʧ���Ҳ������ڶ����Ѵ���
        {
            perror("msgget");
            exit(2);
        }
        if((nMsgId = msgget(lKey,0)) == -1)//�Ѵ���
        {
            perror("msgget");
            exit(3);
        }
    }
    printf("MsgID=%d\n",nMsgId);
    return 0;
}

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
    struct msqid_ds qds;
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
    memset(&qds,0x00,sizeof(struct msqid_ds));
    if(msgctl(nMsgId,IPC_STAT,&qds) < 0)//��ȡ��Ϣ�������ԣ���ȡ״̬��pds��
    {
        perror("msgctl IPC_STAT");
        exit(3);
    }
    printf("msg_perm.mode=%d\n",qds.msg_perm.mode);
    //qds.msg_perm.mode &= (~0222);//ȥ����Ϣ���е�дȨ��
    if(msgctl(nMsgId,IPC_SET,&qds) < 0)//������Ϣ����Ȩ��
    {
        perror("msgctl IPC_SET");
        exit(4);
    }
    memset(&msg,0x00,sizeof(MSG));
    msg.nType = 2;
    memcpy(msg.szText,"12345",5);
    if(msgsnd(nMsgId,(void *)&msg,5,0) < 0)//������Ϣ
    {
        perror("msgsnd");
    }
    if(msgctl(nMsgId,IPC_RMID,NULL) < 0)//ɾ����Ϣ
    {
        perror("msgctl IPC_RMID");
        exit(5);
    }
    return 0;
}
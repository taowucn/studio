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
} MSG;

int main(int argc, char *argv[])
{
    key_t lKey;
    int n,nMsgId;
    MSG msg;
    struct msqid_ds qds;

    lKey = ftok("/etc/profile", 1);
    if (lKey < 0) {
        perror("ftok");
        return -1;
    }

    nMsgId = msgget(lKey, 0);
    if (nMsgId < 0) {
        perror("ftok");
        return -1;
    }

    memset(&qds, 0, sizeof(struct msqid_ds));
    if (msgctl(nMsgId, IPC_STAT, &qds) < 0) {
        perror("msgctl IPC_STAT");
        return -1;
    }
    printf("msg_perm.mode=%d\n", qds.msg_perm.mode);
    if (msgctl(nMsgId, IPC_SET, &qds) < 0) {
        perror("msgctl IPC_SET");
        return -1;
    }

    memset(&msg, 0x0, sizeof(MSG));
    msg.nType = 2;
    memcpy(msg.szText, "12345", 5);
    if (msgsnd(nMsgId, (void *)&msg, 5, 0) < 0) {
        perror("msgsnd");
    }

    if (msgctl(nMsgId, IPC_RMID, NULL) < 0) {
        perror("msgctl IPC_RMID");
        return -1;
    }

    return 0;
}

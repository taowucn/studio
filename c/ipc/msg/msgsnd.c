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
    int nMsgId;
    MSG msg;

    lKey = ftok("/etc/profile", 1);
    if (lKey < 0) {
        perror("ftok");
        return -1;
    }
    nMsgId = msgget(lKey, IPC_CREAT | IPC_EXCL | 0666);
    if (nMsgId < 0) {
        if (errno != EEXIST) {
            perror("msgget");
            return -1;
        }
        nMsgId = msgget(lKey, 0);
        if (nMsgId < 0) {
            perror("msgget");
            return -1;
        }
    }

    memset(&msg, 0, sizeof(MSG));
    msg.nType = 2;
    memcpy(msg.szText, "123456", 6);
    if (msgsnd(nMsgId, (const void *)&msg, strlen(msg.szText), IPC_NOWAIT) < 0) {
        perror("msgsnd");
    }

    return 0;
}

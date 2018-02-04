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

    printf("MsgID=%d\n", nMsgId);
    return 0;
}

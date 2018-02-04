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

    memset(&msg, 0x0, sizeof(MSG));
    n = msgrcv(nMsgId, (void *)&msg, sizeof(msg.szText), 2, 0);
    if(n < 0) {
        perror("msgrcv");
    } else {
        printf("msgrcv return length=[%d] text=[%s]\n",n,msg.szText);
    }

    return 0;
}

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define KEY	1234
#define TEXT_SIZE 48

struct msgbuffer
{
	long mtype;
	char mtext[TEXT_SIZE];
} msgp;

int main(int argc, char **argv)
{
	int msqid;

	msqid = msgget (KEY, IPC_CREAT | 0600);
	if (fork() == 0) {
		msgp.mtype = 1;
		strcpy (msgp.mtext, "Hi, I am a children!\n");
		msgsnd(msqid, &msgp, TEXT_SIZE , 0);
		return 0;
	} else {
		sleep (3);
		msgrcv (msqid, &msgp, TEXT_SIZE, 0, 0);
		printf ("parent recv :%s", msgp.mtext );
		msgctl (msqid, IPC_RMID, NULL);
	}
}

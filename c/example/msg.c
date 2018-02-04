#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/msg.h>

#define MSGSIZE 1024
struct message
{
	long mtype;
	char mtext[MSGSIZE];
};

int main(int argc, char *argv[])
{
	int qid = 0, len = 0;
	key_t key;
	struct message msg;

	if ((key = ftok(".", 'a') < 0)) {
		perror("ftok");
		return -1;
	}
	qid = msgget(key, 0666 | IPC_CREAT);
	if (qid < 0) {
		perror("msgget");
		return -1;
	} else {
		printf("open a queue %d\n", qid);
	}

	memset(msg.mtext, 0, sizeof(msg.mtext));
	strncpy(msg.mtext, "hi, msg Q", sizeof(msg.mtext));
	msg.mtype = getpid();
	len = strlen(msg.mtext);

	do {
		if (msgsnd(qid, &msg, len, 0) < 0) {
			perror("msgsnd");
			break;
		}
		if (msgrcv(qid, &msg, MSGSIZE, 0, 0) < 0) {
			perror("msgrcv");
			break;
		}
	} while(0);

	printf("you send message is :%s \n", msg.mtext);
	if (msgctl(qid, IPC_RMID, NULL) < 0) {
		perror("msgctl");
	}

	return 0;
}

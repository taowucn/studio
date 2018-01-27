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
int main()
{
	int qid,len;
	key_t key;
	struct message msg;

	if((key=ftok(".",'a')==-1))
	{
		perror("ftok");
		exit(1);
	}
	if(qid=msgget(key,IPC_PRIVATE)==-1)
	{
		perror("msgget");
		exit(1);
	}
	else
		printf("open a queue %d \n",qid);
	puts("please enter you message to queue:");
	if(fgets(msg.mtext,MSGSIZE,stdin)==NULL)
	{
		perror("fgets");
		exit(1);
	}
	else
	{
		msg.mtype=getpid();
		//msg_rcv.mtype=getpid();
		len=strlen(msg.mtext);
	}
	if(msgsnd(qid,&msg,len,0)<0)
	{
		perror("msgsnd");
		exit(1);
	}
	if(msgrcv(qid,&msg,MSGSIZE,0,0)<0)
	{
		perror("msgrcv");
		exit(1);
	}
	printf("you send message is :%s \n",msg.mtext);
	if(msgctl(qid,IPC_RMID,NULL)<0)
	{
		perror("msgctl");
		exit(1);
	}
	exit(0);
}

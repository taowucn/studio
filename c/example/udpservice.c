#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 3333
#define MAXDATA 100
int main()
{
	int srvfd,recvLen;
	char *sendBuf,recvBuf[MAXDATA];
	struct sockaddr_in srvAddr,clientAddr;
	if(srvfd=socket(AF_INET,SOCK_DGRAM,0)<0)
	{
		perror("socket");
		exit(1);
	}
	printf("socket %d create sucess\n");
	bzero(&srvAddr,sizeof(struct sockaddr_in));
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_port=htons(PORT);
	srvAddr.sin_addr.s_addr=INADDR_ANY;

	if(bind(srvfd,(struct sockaddr *)&srvAddr,sizeof(struct sockaddr))<0)
	{
		perror("bind");
		exit(1);
	}
	recvLen=sizeof(struct sockaddr);
	while(1)
	{
		recvfrom(srvfd,recvBuf,MAXDATA,0,(struct sockaddr *)&clientAddr,&recvLen);
		printf("receive %s from %s\n",recvBuf,inet_ntoa(clientAddr.sin_addr.s_addr));
	}
	close(srvfd);

}

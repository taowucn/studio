#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#define PORT 6666
#define MAXCONN 10
#define MAXDATA 100
int main()
{
	int srvfd,clientfd;
	int recvLen;
	struct sockaddr_in addr_srv,addr_client;
	char recvBuf[MAXDATA];
	memset(recvBuf,0,MAXDATA);
	if((srvfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	printf("socket %d create sucess!\n",srvfd);
	addr_srv.sin_family=AF_INET;
	addr_srv.sin_port=htons(PORT);
	addr_srv.sin_addr.s_addr=INADDR_ANY;
	bzero(&(addr_srv.sin_zero),8);

	if(bind(srvfd,(struct sockaddr *)&addr_srv,sizeof(struct sockaddr))==-1)
	{
		perror("bind");
		exit(1);
	}
	printf("bind sucess\n");
	if(listen(srvfd,MAXCONN)==-1)
	{
		perror("listen");
		exit(1);
	}
	printf("we are listening...\n");
	recvLen=sizeof(struct sockaddr);
	while(1)
	{
		if((clientfd=accept(srvfd,(struct sockaddr *)&addr_client,&recvLen))==-1)
		{
			perror("accept");
			exit(1);
		}
		if(recv(clientfd,recvBuf,MAXDATA,0)==-1)
		{
			perror("recv");
			exit(1);
		}
		printf("receive %s form %s\n",recvBuf,inet_ntoa(addr_client));
	}
	close(srvfd);

}

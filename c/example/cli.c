#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXDATA 100
#define SRVPORT 6666
int main(int argc,char *argv[])
{
	int clientfd;
	char sendBuf[6]="hello";
	struct sockaddr_in addr_srv;
	struct hostent *host;

	if(argc<2)
	{
		printf("pleas enter IP address!\n");
		exit(1);
	}
	if((host=gethostbyname(argv[1]))==NULL)
	{
		perror("gethostbyname");
		exit(1);
	}

	if((clientfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	printf("socket %d create sucess\n",clientfd);
	addr_srv.sin_family=AF_INET;
	addr_srv.sin_port=htons(SRVPORT);
	addr_srv.sin_addr=*((struct in_addr *)host->h_addr);
	bzero(&(addr_srv.sin_zero),8);
	if(connect(clientfd,(struct sockaddr *)&addr_srv,sizeof(struct sockaddr))==-1)
	{
		perror("connect");
		exit(1);
	}
	printf("connect sucess");
	if(send(clientfd,sendBuf,6,0)==-1)
	{
		perror("send");
		exit(1);
	}
	close(clientfd);
}

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
	int clientfd = -1, ret = 0;
	char sendBuf[6]="hello";
	struct sockaddr_in addr_srv;
	struct hostent *host;

	if (argc<2) {
		printf("pleas enter IP address!\n");
		return -1;
	}

	host = gethostbyname(argv[1]);
	if (host == NULL) {
		perror("gethostbyname");
		return -1;
	}

	clientfd = socket(AF_INET,SOCK_STREAM,0);
	if (clientfd == -1) {
		perror("socket");
		return -1;
	}

	printf("socket %d create sucess\n", clientfd);
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(SRVPORT);
	addr_srv.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(addr_srv.sin_zero), 8);

	do {
		if (connect(clientfd,(struct sockaddr *)&addr_srv,sizeof(struct sockaddr)) < 0) {
			perror("connect");
			ret = -1;
			break;
		}
		printf("connect OK\n");
		if (send(clientfd,sendBuf,6,0) < 0) {
			perror("send");
			ret = -1;
			break;
		}
		printf("send OK\n");
	} while (0);

	close(clientfd);
	clientfd = -1;
	return ret;
}

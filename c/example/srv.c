#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 6666
#define MAXCONN 10
#define MAXDATA 100

int main(int argc, char *argv[])
{
	int srvfd = 0, clientfd = 0;
	int recvLen = 0;
	struct sockaddr_in addr_srv, addr_client;
	char recvBuf[MAXDATA];

	memset(recvBuf,0,MAXDATA);
	srvfd = socket(AF_INET,SOCK_STREAM,0);
	if (srvfd < 0) {
		perror("socket");
		return -1;
	}

	printf("socket %d create sucess!\n", srvfd);
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(PORT);
	addr_srv.sin_addr.s_addr = INADDR_ANY;
	bzero(&(addr_srv.sin_zero), 8);

	do {
		if (bind(srvfd,(struct sockaddr *)&addr_srv,sizeof(struct sockaddr)) < 0) {
			perror("bind");
			break;
		}
		printf("bind sucess\n");
		if (listen(srvfd,MAXCONN) < 0) {
			perror("listen");
			break;
		}

		printf("we are listening...\n");
		recvLen = sizeof(struct sockaddr);
		while (1) {
			if ((clientfd = accept(srvfd,(struct sockaddr *)&addr_client, &recvLen)) < 0) {
				perror("accept");
				break;
			}
			if (recv(clientfd,recvBuf,MAXDATA,0) < 0) {
				perror("recv");
				break;
			}
			printf("receive %s from %s\n", recvBuf, inet_ntoa(addr_client.sin_addr));
		}
	} while (0);

	close(srvfd);
	srvfd = -1;
}

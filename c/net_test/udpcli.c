#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXLINE 80
#define SERV_PORT 7877

void do_cli(FILE *fp, int sockfd, struct sockaddr *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE] = "1234567890";
	char recvline[MAXLINE + 1];

	if(connect(sockfd, (struct sockaddr *)pservaddr, servlen) == -1) {
		perror("connect error");
		exit(1);
	}

//	while(fgets(sendline, MAXLINE, fp) != NULL) {

		write(sockfd, sendline, strlen(sendline));
#if 0
		n = read(sockfd, recvline, MAXLINE);
		if(n == -1) {
			perror("read error");
			exit(1);
		}
		recvline[n] = '\0';
		fputs(recvline, stdout);
#endif
		printf("OK\n");
//	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	if(argc != 2) {
		printf("usage: udpclient <IPaddress>\n");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		printf("[%s] is not a valid IPaddress\n", argv[1]);
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	printf("Port = %d\n", SERV_PORT);
	do_cli(stdin, sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	close(sockfd);

	return 0;
}

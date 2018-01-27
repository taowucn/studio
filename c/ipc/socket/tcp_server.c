#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

typedef struct{
   int client_fd;
   struct sockaddr_in client_addr;
   int timeout_seconds;
   int timer_id_valid;
   timer_t timer_id;
   volatile int disconnect;
}client_info_t;

typedef struct{
    int timeout_seconds;//client_connection_timeout
    //now only one client supported
    client_info_t client;
    struct sockaddr_in client_addr;
}keepalive_handle_t;

#define CLOCKID CLOCK_MONOTONIC
#define SIG   SIGRTMIN
static void sig_handler(int sig, siginfo_t *si, void *uc){
    client_info_t *h = (client_info_t *)si->si_value.sival_ptr;
    //printf("***********sig_handler***************\n");
    h->disconnect = 1;
    signal(sig, SIG_IGN);
}

static int create_posix_timer(client_info_t  *client){
    timer_t timerid;
    struct sigevent sev;
    struct sigaction sa;

    if(client->timer_id_valid){
        return -1;
    }
    client->timer_id_valid = 0;
    /* Establish handler for timer signal */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sig_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1){
        printf("create_posix_timer --- sigaction error, errno = %d\n",errno);
        fflush(stdout);
        return -1;
    }
    /* Create the timer */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = client;
    if (timer_create(CLOCKID, &sev, &timerid) == -1){
        printf("create_posix_timer --- timer_create error, errno = %d\n",errno);
        fflush(stdout);
        return -1;
    }
    client->timer_id = timerid;
    client->timer_id_valid = 1;
    return 0;
}

static int start_posix_timer(client_info_t  *client){
    if(!client->timer_id_valid){
        return -1;
    }
    struct itimerspec its;
    /* Start the timer */
    its.it_value.tv_sec = client->timeout_seconds;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    if (timer_settime(client->timer_id, 0, &its, NULL) == -1){
        printf("start_posix_timer --- timer_settime error, errno = %d\n",errno);
        fflush(stdout);
        timer_delete(client->timer_id);
        return -1;
    }
    return 0;
}
static void stop_posix_timer(client_info_t *client){
    if(client->timer_id_valid){
        timer_delete(client->timer_id);
        client->timer_id_valid = 0;
    }
}

typedef struct _iphdr
{
    unsigned char h_verlen;
    unsigned char tos;
    unsigned short total_len;
    unsigned short ident;
    unsigned short frag_and_flags;
    unsigned char ttl;
    unsigned char proto;
    unsigned short checksum;
    unsigned int sourceIP;
    unsigned int destIP;
}IP_HEADER;

typedef struct _tcphdr
{
    unsigned short th_sport;
    unsigned short th_dport;
    unsigned int th_seq;
    unsigned int th_ack;
    unsigned char th_lenres;
    unsigned char th_flag;
    unsigned short th_win;
    unsigned short th_sum;
    unsigned short th_urp;
}TCP_HEADER;
/*
static void analyseTCP(IP_HEADER *ip, TCP_HEADER *tcp)
{
    printf("TCP -----\n");
    unsigned char* p = (unsigned char*)&ip->sourceIP;
    printf("Source IP\t: %u.%u.%u.%u\n",p[0],p[1],p[2],p[3]);
    p = (unsigned char*)&ip->destIP;
    printf("Destination IP\t: %u.%u.%u.%u\n",p[0],p[1],p[2],p[3]);
    printf("Source port: %u Dest port: %u\n", ntohs(tcp->th_sport), ntohs(tcp->th_dport));
    printf("th_seq/ack: %u %u\n", ntohl(tcp->th_seq), ntohl(tcp->th_ack));
}
*/
static int tcp_server(int argc, char *argv[])
{
    int raw_fd = 0,server_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    short  listen_port = 6666;
    char buf[10240];
    keepalive_handle_t keepalive_handle;
    memset(&keepalive_handle,0,sizeof(keepalive_handle_t));
    keepalive_handle.timeout_seconds = 15;

    signal(SIGPIPE,SIG_IGN);
	printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
    /* capture ip datagram without ethernet header */
    if ((raw_fd = socket(PF_PACKET,  SOCK_DGRAM, htons(ETH_P_IP)))== -1){
		perror("socket");
        goto err_exit;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
        goto err_exit;
    }

    int yes = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
         goto err_exit;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(listen_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero,0, sizeof(server_addr.sin_zero));
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
       goto err_exit;
    }

    if (listen(server_fd, 5) == -1) {
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
         goto err_exit;
    }

    printf("tcp-keep-alive test --- listen port %d\n",listen_port);

    fd_set fdset;
    socklen_t sin_size = sizeof(client_addr);
    int max_fd;

    while (1) {
        //check client alive or not
        do{
            client_info_t *client = &keepalive_handle.client;
            if(client->client_fd && client->disconnect){
                printf("client [%d], timeout disconnect\n",client->client_fd);
                stop_posix_timer(client);
                close(client->client_fd);
                memset(client,0,sizeof(client_info_t));
            }
        }while(0);

        FD_ZERO(&fdset);
        FD_SET(raw_fd, &fdset);
        FD_SET(server_fd, &fdset);
        max_fd = (raw_fd > server_fd) ? raw_fd : server_fd;
        do{
            client_info_t *client = &keepalive_handle.client;
            if(client->client_fd){
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
                FD_SET(client->client_fd, &fdset);
                max_fd = (max_fd > client->client_fd) ? max_fd: client->client_fd;
            }
        }while(0);

        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        int ret = select(max_fd + 1, &fdset,NULL, NULL, &tv);
        if (ret < 0) {
            break;
        } else if (ret == 0) {
            continue;//TIMEOUT
        }

        if (FD_ISSET(server_fd, &fdset)) {
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
            int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd <= 0) {
                printf("failed to accept, ....\n");
            }else{
                printf("***** new_client is comming, fd = %d\n",new_fd);
                //check to add client
                client_info_t *client = &keepalive_handle.client;
                if(client->client_fd){
                    stop_posix_timer(client);
                    close(client->client_fd);
                }
                client->client_fd = new_fd;
                client->timeout_seconds = keepalive_handle.timeout_seconds;
                memcpy(&client->client_addr,&client_addr,sizeof(struct sockaddr_in));
                if(create_posix_timer(client) < 0){
                    printf("failed to create timer for this client, close it, TODO\n");
                    close(new_fd);
                    memset(&client,0,sizeof(client_info_t));
                }else{
                    unsigned char* p = (unsigned char*)&client_addr.sin_addr.s_addr;
                    printf("*****IP:PORT : %u.%u.%u.%u:%u\n",p[0],p[1],p[2],p[3],ntohs(client_addr.sin_port));
                }
            }
        }
	//printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
        //check all the clients
        client_info_t *client = &keepalive_handle.client;
        if (FD_ISSET(client->client_fd, &fdset)) {
            ssize_t n = recv(client->client_fd, buf, sizeof(buf), 0);
            if (n <= 0){
                printf("client_fd recv error! n = %d, errno = %d, disconnect\n",(int)n,errno);
                stop_posix_timer(client);
                close(client->client_fd);
                memset(client,0,sizeof(client_info_t));
            }else{
                start_posix_timer(client);
                printf("client_fd recv ok! n = %d, restart timer\n",(int)n);
            }
        }
	//printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
        if (FD_ISSET(raw_fd, &fdset)) {
            ssize_t n = recv(raw_fd, buf, sizeof(buf), 0);
            if (n == -1){
                printf("raw_fd recv error! BREAK, TODO\n");
                break;
            }
            else if (n > 0){
                client_info_t *client = &keepalive_handle.client;
                if(!client->client_fd) continue;
                IP_HEADER *ip = ( IP_HEADER *)(buf);
                size_t iplen =  (ip->h_verlen&0x0f)*4;
                if (ip->proto == IPPROTO_TCP){
                    TCP_HEADER *tcp = (TCP_HEADER *)(buf +iplen);
                    //analyseTCP(ip,tcp);
                    size_t tcplen = ((tcp->th_lenres >> 4)& 0x0f) * 4;
                    //printf("sizeof(IP_HEADER) = %d, sizeof(TCP_HEADER) = %d, iplen = %d, tcplen = %d,n = %d\n",
                    //    sizeof(IP_HEADER),sizeof(TCP_HEADER),iplen,tcplen,n);
                    if( (n <= iplen + tcplen + 1) && ( (tcp->th_flag & 0x3F) == 0x10/*ACK*/)
                        && (client->client_addr.sin_addr.s_addr == ip->sourceIP)
                        && (client->client_addr.sin_port == tcp->th_sport)){
                       printf("raw_fd, client [%d] keepalive received, restart timer\n",client->client_fd);
                       start_posix_timer(client);
                    }else{
                       // FIN/RST should be handled here ?
                    }
                }
	        }
        }
    }//while(1)
	printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
err_exit:
	printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
    if(raw_fd) close(raw_fd);
    if(server_fd) close(server_fd);
    do{
       client_info_t *client = &keepalive_handle.client;
       if(client->client_fd) close(client->client_fd);
       stop_posix_timer(client);
    }while(0);
    return 0;
}

#include <netinet/tcp.h>
static int tcp_client(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if(argc != 2){
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return -1;
    }
	printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
    memset(recvBuff, '0',sizeof(recvBuff));

    signal(SIGPIPE,SIG_IGN);
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Error : Could not create socket \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6666);
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){
        printf("\n inet_pton error occured\n");
        return -1;
    }

    int keepalive = 1;
    int keepidle = 10;
    int keepinterval = 5;
    int keepcount = 3;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
    setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
       printf("\n Error : Connect Failed \n");
       return -1;
    }

	write(sockfd, "hello", sizeof("hello"));

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0){
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF){
            printf("\n Error : Fputs error\n");
        }
    }

    if(n < 0){
        printf("\n Read error \n");
    }

    close(sockfd);
    return 0;
}

int main(int argc, char *argv[]){
	printf("Date Version : %s\n", __TIME__);

    if(argc == 1){
	printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
        return tcp_server(argc,argv);
    }
	printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
    return tcp_client(argc,argv);
}


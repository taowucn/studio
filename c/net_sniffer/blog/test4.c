#define __USE_BSD         /* Using BSD IP header           */
#include <netinet/ip.h>   /* Internet Protocol             */
#define __FAVOR_BSD       /* Using BSD TCP header          */
#include <netinet/tcp.h>  /* Transmission Control Protocol */

#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>       /* String operations             */

#include <arpa/inet.h>

void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
{
  int * id = (int *)arg;
 struct ip *iphdr = NULL;          /* IPv4 Header                            */
 struct tcphdr *tcphdr = NULL;     /* TCP Header                             */
 unsigned int *cp;
 unsigned int *cp1;

 if(*id==1) {
 	printf("+-------------------------+\n");
 }

  printf("id: %d\n", ++(*id));
  printf("Packet length: %d\n", pkthdr->len);
  printf("Number of bytes: %d\n", pkthdr->caplen);
  printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));

#if 1
  int i;
  for(i=0; i<pkthdr->len; ++i)
  {
    printf(" %02x", packet[i]);
    if( (i + 1) % 16 == 0 )
    {
      printf("\n");
    }
  }
  printf("\n");
#endif

 iphdr = (struct ip *)(packet+14);
 tcphdr = (struct tcphdr *)(packet+14+20);
 cp = (unsigned int *)(packet+58);
 cp1 = (unsigned int *)(packet+62);

// printf("Received Packet No.%d:\n", ++count);
  printf("   DST MAC: ");
  for(i=0; i<6;i++){
	printf("%02X:", packet[i]);
   }
  printf("\n");

 printf("   ACK: %u\n", ntohl(tcphdr->th_ack) );
 printf("   SEQ: %u\n", ntohl(tcphdr->th_seq) );
 printf("   ID: %d\n", ntohs(iphdr->ip_id));

 printf("   DST IP: %s\n", inet_ntoa(iphdr->ip_dst));
 printf("   SRC IP: %s\n", inet_ntoa(iphdr->ip_src));
 printf("   SRC PORT: %d\n", ntohs(tcphdr->th_sport) );
 printf("   DST PORT: %d\n", ntohs(tcphdr->th_dport) );

 printf("   Win: %d\n", ntohs(tcphdr->th_win) );
// printf("   Window: %d\n", ntohs(tcphdr->window) );

 printf("   TVar: %u\n", ntohl(*cp));
 printf("   TVarR: %u\n",  ntohl(*cp1));

 printf("\n\n");

}

int main(int argc, char** argv)
{
  char errBuf[PCAP_ERRBUF_SIZE], * devStr;
 struct tcphdr *tcphdr = NULL;
  /* get a device */
  if( argc > 1){
    /* If user supplied interface name, use it. */
    devStr = argv[1];
  } else {
    devStr = pcap_lookupdev(errBuf);
  }

  if(devStr)
  {
    printf("success: device: %s\n", devStr);
  }
  else
  {
    printf("error: %s\n", errBuf);
    exit(1);
  }

  /* open a device, wait until a packet arrives */
  pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);

  if(!device)
  {
    printf("error: pcap_open_live(): %s\n", errBuf);
    exit(1);
  }

  /* construct a filter */
  struct bpf_program filter;
  pcap_compile(device, &filter, "tcp src port 7878", 1, 0);
  pcap_setfilter(device, &filter);

  printf("TCP header Size: %lu\n\n", sizeof(*tcphdr));

  /* wait loop forever */
  int id = 0;
  pcap_loop(device, -1, getPacket, (u_char*)&id);

  pcap_close(device);

  return 0;
}
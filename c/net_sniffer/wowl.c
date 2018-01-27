#define __USE_BSD		/* Using BSD IP header 	*/
#include <netinet/ip.h>	/* Internet Protocol 		*/
#define __FAVOR_BSD	/* Using BSD TCP header	*/
#include <netinet/tcp.h>	/* Transmission Control Protocol	*/

#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ether.h>
#include <pthread.h>

typedef unsigned char	u8;
typedef unsigned int	u32;
typedef unsigned char	uint8;	/**< UNSIGNED 8-bit data type */
typedef unsigned short	uint16;/**< UNSIGNED 16-bit data type */
typedef unsigned int	uint;	/**< UNSIGNED 32-bit data type */
typedef unsigned int 	uint32;/**< UNSIGNED 32-bit data type */
typedef signed int		int32;	/**< SIGNED 32-bit data type */
typedef unsigned int	uintptr;

#define MAXBYTES2CAPTURE	(2048)
#define DATA_MAXSIZE 		(32)
#define FILTER_MAXSIZE		(1024)
#define DEFAULT_HOST		"127.0.0.1"
#define DEFAULT_PORT		(7877)
#define DEFAULT_INTERVAL	(10)
#define DEFAULT_MSG		"*"
#define DEFAULT_NET_FILTER	"tcp src port 7877"

#define NO_ARG		(0)
#define HAS_ARG		(1)

#define PROP_VALUE_MAX	(16)
#define WLC_GET_VERSION	(1)
#define WLC_GET_REVINFO	(98)

#define WLC_IOCTL_MAXLEN	(8192)
#define WLC_GET_VAR		(262)
#define WLC_SET_VAR		(263)

#define IPV4_ADDR_LEN		(4)
typedef struct ipv4_addr {
        uint8   addr[IPV4_ADDR_LEN];
} ipv4_addr_t;

/* Linux network driver ioctl encoding */
typedef struct wl_ioctl {
	uint cmd;	/* common ioctl definition */
	void *buf;	/* pointer to user buffer */
	uint len;		/* length of user buffer */
	unsigned char set;		/* 1=set IOCTL; 0=query IOCTL */
	uint used;	/* bytes read or written (optional) */
	uint needed;	/* bytes needed (optional) */
} wl_ioctl_t;

#pragma pack(1)

typedef struct tcpka_conn {
	uint32 sess_id;
	struct ether_addr dst_mac; /* Destinition Mac */
	struct in_addr  src_ip;   	/* Sorce IP */
	struct in_addr  dst_ip;   	/* Destinition IP */

	uint16 ipid;		/* Ip Identification */
	uint16 srcport;	/* Source Port Address */
	uint16 dstport;	/* Destination Port Address */
	uint32 seq;		/* TCP Sequence Number */
	uint32 ack;		/* TCP Ack Number */
	uint16 tcpwin;	/* TCP window */
	uint32 tsval;		/* Timestamp Value */
	uint32 tsecr;		/* Timestamp Echo Reply */
} tcpka_conn_t;

typedef struct wl_mtcpkeep_alive_timers_pkt {
	uint16 interval;		/* interval timer */
	uint16 retry_interval;	/* retry_interval timer */
	uint16 retry_count;	/* retry_count */
} wl_mtcpkeep_alive_timers_pkt_t;

typedef struct tcpka_conn_sess {
	uint32 sess_id;	/* session id */
	uint32 flag;		/* enable/disable flag */
	wl_mtcpkeep_alive_timers_pkt_t  tcp_keepalive_timers;
} tcpka_conn_sess_t;

#define WL_PKT_FILTER_FIXED_LEN		  OFFSETOF(wl_pkt_filter_t, u)
#define WL_PKT_FILTER_PATTERN_FIXED_LEN	  OFFSETOF(wl_pkt_filter_pattern_t, mask_and_pattern)

typedef enum {
	wowl_pattern_type_bitmap = 0,
	wowl_pattern_type_arp,
	wowl_pattern_type_na
} wowl_pattern_type_t;

typedef struct wl_wowl_pattern_s {
	uint		masksize;	/* Size of the mask in #of bytes */
	uint		offset;		/* Pattern byte offset in packet */
	uint		patternoffset;/* Offset of start of pattern in the structure */
	uint		patternsize;	/* Size of the pattern itself in #of bytes */
	ulong	id;			/* id */
	uint		reasonsize;	/* Size of the wakeup reason code */
	//wowl_pattern_type_t type;		/* Type of pattern */
	/* Mask follows the structure above */
	/* Pattern follows the mask is at 'patternoffset' from the start */
} wl_wowl_pattern_t;

#define	OFFSETOF(type, member)	((uint)(uintptr)&((type *)0)->member)
typedef struct wl_mkeep_alive_pkt {
	uint16	version;		/* Version for mkeep_alive */
	uint16	length;		/* length of fixed parameters in the structure */
	uint32	period_msec;
	uint16	len_bytes;
	uint8	keep_alive_id; /* 0 - 3 for N = 4 */
	uint8	data[1];
} wl_mkeep_alive_pkt_t;

#define WL_MKEEP_ALIVE_VERSION		1
#define WL_MKEEP_ALIVE_FIXED_LEN	OFFSETOF(wl_mkeep_alive_pkt_t, data)
typedef struct wl_pkt_filter_pattern {
	uint32	offset;	/* Offset within received packet to start pattern matching.
					 * Offset '0' is the first byte of the ethernet header.*/
	uint32	size_bytes;	/* Size of the pattern.  Bitmask must be the same size. */
	uint8   mask_and_pattern[1]; /* Variable length mask and pattern data.  mask starts
								* at offset 0.  Pattern immediately follows mask.*/
} wl_pkt_filter_pattern_t;

typedef struct wl_pkt_filter {
	uint32	id;		/* Unique filter id, specified by app. */
	uint32	type;	/* Filter type (WL_PKT_FILTER_TYPE_xxx). */
	uint32	negate_match;	/* Negate the result of filter matches */
	union {			/* Filter definitions */
		wl_pkt_filter_pattern_t pattern;	/* Pattern matching filter */
	} u;
} wl_pkt_filter_t;

#define WL_PKT_FILTER_FIXED_LEN		  OFFSETOF(wl_pkt_filter_t, u)
#define WL_PKT_FILTER_PATTERN_FIXED_LEN	  OFFSETOF(wl_pkt_filter_pattern_t, mask_and_pattern)
typedef struct wl_pkt_filter_enable {
	uint32	id;		/* Unique filter id */
	uint32	enable;	/* Enable/disable bool */
} wl_pkt_filter_enable_t;

typedef struct wl_pkt_filter_stats {
	uint32	num_pkts_matched;		/* # filter matches for specified filter id */
	uint32	num_pkts_forwarded;	/* # packets fwded from dongle to host for all filters */
	uint32	num_pkts_discarded;	/* # packets discarded by dongle for all filters */
} wl_pkt_filter_stats_t;

#pragma pack(4)

typedef struct net_socket_s
{
	int is_session;
	int is_close;
	int is_tcp;
	int port;
	int interval;
	int is_sys_exec;
	int verbose;

	char iface[PROP_VALUE_MAX];
	char srv_ip[DATA_MAXSIZE];
	char client_ip[DATA_MAXSIZE];
	char send_msg[DATA_MAXSIZE];

	char net_filter[FILTER_MAXSIZE];
} net_socket_t;

static net_socket_t net_socket;

struct hint_s {
	const char *arg;
	const char *str;
};

static const char *short_options = "i:c:p:tusm:f:ev";
static struct option long_options[] = {
	{"interface",	HAS_ARG, 0, 'i'},
	{"host",		HAS_ARG, 0, 'c'},
	{"port",		HAS_ARG, 0, 'p'},
	{"udp",		NO_ARG, 0, 'u'},
	{"tcp",		NO_ARG, 0, 't'},
	{"session",	NO_ARG, 0, 's'},
	{"session",	HAS_ARG, 0, 'm'},
	{"filter", 		HAS_ARG,  0, 'f'},
	{"close",		NO_ARG, 0, 'e'},
	{"system",	NO_ARG, 0, 'x'},
	{"verbose",	NO_ARG, 0, 'v'},

	{0, 0, 0, 0}
};

static const struct hint_s hint[] = {
	{"", "Listen on interface"},
	{"", "Host IP address."},
	{"", "Host Port, default is [7877]"},
	{"", "UDP Keep Alive and Wakeup"},
	{"", "TCP Keep Alive and Wakeup"},
	{"", "Create one TCP/UDP session"},
	{"", "Keep Alive interval time, in seconds"},
	{"", "Setting Sniffer Network Filter"},
	{"",	"Close fd after exit, it is disable by default"},
	{"", "Using system exec to call wl(Keep-alive, Wakeup Pattern)"},
	{"",	"Print verbose info"},
};

static void usage(void)
{
	u32 i = 0;
	char *itself = "wowl";
	printf("This program used to set wake up on wireless (include keepalive and wakeup pattern)\n");
	printf("\n");
	for (i = 0; i < sizeof(long_options) / sizeof(long_options[0]) - 1; i++) {
		if (isalpha(long_options[i].val))
			printf("-%c ", long_options[i].val);
		else
			printf("   ");
		printf("--%s", long_options[i].name);
		if (hint[i].arg[0] != 0)
			printf(" [%s]", hint[i].arg);
		printf("\t%s\n", hint[i].str);
	}

	printf("Example:\n");
	printf("\t TCP # %s -i wlan0 -c 127.0.0.1 -p 7877 -t -s -m 10 -f \"tcp dst port 7877\" \n", itself);
	printf("\t UDP # %s -i wlan0 -c 127.0.0.1 -p 7877 -u -s -m 10 -f \"udp dst port 7877\" \n", itself);
}

static int init_param(int argc, char **argv)
{
	int ch = 0;
	int value = 0;
	int option_index = 0;

	opterr = 0;
	while ((ch = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
		switch (ch) {
		case 'i':
			strncpy(net_socket.iface, optarg, sizeof(net_socket.iface));
			break;
		case 'c':
			strncpy(net_socket.srv_ip, optarg, sizeof(net_socket.srv_ip));
			break;
		case 'p':
			value = atoi(optarg);
			if ((value < 0) || (value > 65535)) {
				printf("Please set port in %d ~ %d.\n", 0, 65535);
				return -1;
			}
			net_socket.port = value;
			break;
		case 'm':
			value = atoi(optarg);
			if ((value < 0) || (value > 65535)) {
				printf("Please set interval in %d ~ %d.\n", 0, 65535);
				return -1;
			}
			net_socket.interval = value;
			break;
		case 't':
			net_socket.is_tcp = 1;
			break;
		case 'u':
			net_socket.is_tcp = 0;
			break;
		case 's':
			net_socket.is_session = 1;
			break;
		case 'f':
			strncpy(net_socket.net_filter, optarg, sizeof(net_socket.net_filter));
			break;
		case 'e':
			net_socket.is_close = 1;
		case 'x':
			net_socket.is_sys_exec = 1;
		case 'v':
			net_socket.verbose = 1;
			break;
		default:
			printf("unknown option found: %c\n", ch);
			return -1;
		}
	}

	return 0;
}

void in_addr_to_ipv4(struct ipv4_addr *ipa, u32 ip)
{
	ipa->addr[0] = (u8)(ip >> 0) & 0xFF;
	ipa->addr[1] = (u8)(ip >> 8) & 0xFF;
	ipa->addr[2] = (u8)(ip >> 16) & 0xFF;
	ipa->addr[3] = (u8)(ip >> 24) & 0xFF;
}

static void close_fd(int *fd)
{
	if (net_socket.is_close && (*fd > 0)) {
		close(*fd);
		printf("Close [%d].\n", *fd);
		*fd = -1;
	}
}

ssize_t readn( int inSock, void *outBuf, size_t inLen )
{
	size_t  nleft = 0;
	ssize_t nread = 0;
	char *ptr;

	assert( inSock >= 0 );
	assert( outBuf != NULL );
	assert( inLen > 0 );

	ptr   = (char*) outBuf;
	nleft = inLen;

	while ( nleft > 0 ) {
		nread = read( inSock, ptr, nleft );
		if ( nread < 0 ) {
			if ( errno == EINTR ) {
				nread = 0;  /* interupted, call read again */
			}
			else{
				return -1;  /* error */
			}
		} else if ( nread == 0 ) {
			break;        /* EOF */
		}
		nleft -= nread;
		ptr   += nread;
	}

	return(inLen - nleft);
} /* end readn */

ssize_t writen( int inSock, const void *inBuf, size_t inLen )
{
	size_t  nleft = 0;
	ssize_t nwritten = 0;
	const char *ptr;

	assert( inSock >= 0 );
	assert( inBuf != NULL );
	assert( inLen > 0 );

	ptr   = (char*) inBuf;
	nleft = inLen;

	while ( nleft > 0 ) {
		nwritten = write( inSock, ptr, nleft );
		if ( nwritten <= 0 ) {
			if ( errno == EINTR ) {
				nwritten = 0; /* interupted, call write again */
			} else {
				return -1;    /* error */
			}
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}

	return inLen;
} /* end writen */

static int wl_pattern_atoh(char *src, char *dst)
{
	int i = 0;

	if (strncmp(src, "0x", 2) != 0 && strncmp(src, "0X", 2) != 0) {
		printf("Data invalid format. Needs to start with 0x\n");
		return -1;
	}
	src = src + 2; /* Skip past 0x */
	if (strlen(src) % 2 != 0) {
		printf("Data invalid format. Needs to be of even length\n");
		return -1;
	}
	for (i = 0; *src != '\0'; i++) {
		char num[3];
		strncpy(num, src, 2);
		num[2] = '\0';
		dst[i] = (uint8)strtoul(num, NULL, 16);
		src += 2;
	}

	return i;
}

static uint wl_iovar_mkbuf(const char *name, char *data, uint datalen,
	char *iovar_buf, uint buflen, int *perr)
{
	uint iovar_len = 0;

	iovar_len = strlen(name) + 1;

	/* check for overflow */
	if ((iovar_len + datalen) > buflen) {
		*perr = -1;
		return 0;
	}

	/* copy data to the buffer past the end of the iovar name string */
	if (datalen > 0) {
		memmove(&iovar_buf[iovar_len], data, datalen);
	}

	/* copy the name to the beginning of the buffer */
	strcpy(iovar_buf, name);

	*perr = 0;
	return (iovar_len + datalen);
}

/*  TCP Keep alive */
int wl_tcpka_conn_add(tcpka_conn_t tcpka)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	char buf_src_addr[DATA_MAXSIZE];
	char buf_dst_addr[DATA_MAXSIZE];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	int ret = 0;
	int s = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	wl_iovar_mkbuf("tcpka_conn_add", (char *)&tcpka, sizeof(tcpka_conn_t),
		bufdata, WLC_IOCTL_MAXLEN, &ret);

	if (ret) {
		printf("Failed to build buffer, %s.\n", __FUNCTION__);
		return -1;
	}

	ioc.cmd = WLC_GET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 0;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail tcpka_conn_add, %s\n", strerror(errno));
		return ret;
	}

	if (net_socket.verbose) {
		strncpy(buf_src_addr, inet_ntoa(tcpka.src_ip), sizeof(buf_src_addr));
		strncpy(buf_dst_addr, inet_ntoa(tcpka.dst_ip), sizeof(buf_dst_addr));
		printf("Session ID [%d]\n", tcpka.sess_id);
		printf("CMD: tcpka_conn_add %s %s %s %d %d %d %u %u %d %u %u\n",
			ether_ntoa(&tcpka.dst_mac), buf_src_addr, buf_dst_addr,
			tcpka.ipid, tcpka.srcport, tcpka.dstport, tcpka.seq, tcpka.ack, tcpka.tcpwin,
			tcpka.tsecr, tcpka.tsval);
	}

	return ret;
}

int wl_tcpka_conn_enable(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	tcpka_conn_sess_t tcpka_conn;
	int ret = 0;
	int s = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* init tcpka_conn */
	tcpka_conn.sess_id = 1;
	tcpka_conn.flag = 1;
	tcpka_conn.tcp_keepalive_timers.interval = net_socket.interval; //senconds
	tcpka_conn.tcp_keepalive_timers.retry_interval = 2;
	tcpka_conn.tcp_keepalive_timers.retry_count = 2;

	wl_iovar_mkbuf("tcpka_conn_enable", (char *)&tcpka_conn, sizeof(tcpka_conn_sess_t),
		bufdata, WLC_IOCTL_MAXLEN, &ret);

	if (ret) {
		printf("Failed to build buffer, %s.\n", __FUNCTION__);
		return -1;
	}

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set tcpka_conn_enable, %s\n", strerror(errno));
	}

	if (net_socket.verbose) {
		printf("TCP Interval [%d] seconds\n", tcpka_conn.tcp_keepalive_timers.interval);
	}

	return ret;
}

int wl_tcpka_conn_del(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	int32 id = 0;
	int ret = 0;
	int s = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* init tcpka */
	id = 1;

	wl_iovar_mkbuf("tcpka_conn_del", (char *)&id, sizeof(int32),
		bufdata, WLC_IOCTL_MAXLEN, &ret);

	if (ret) {
		printf("Failed to build buffer, %s.\n", __FUNCTION__);
		return -1;
	}

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set tcpka_conn_del, %s\n", strerror(errno));
	}

	return ret;
}

int wl_wowl_pattern(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	const char *str;
	wl_wowl_pattern_t *wl_pattern;
	char *buf, *mask_and_pattern;
	char *mask, *pattern;
	int str_len = 0;
	uint buf_len = 0;
	int ret = 0;
	int s = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* wl wowl_pattern add 0 0x0f 0x77616b65 : Receive "wake" tcp data */
	str = "wowl_pattern";
	str_len = strlen(str);
	strncpy(bufdata, str, str_len);
	bufdata[str_len] = '\0';
	buf = bufdata + strlen(str) + 1;
	buf_len = str_len + 1;

	str = "add";
	strncpy(buf, str, strlen(str));
	buf_len += strlen(str) + 1;

	wl_pattern = (wl_wowl_pattern_t *)(buf + strlen(str) + 1);
	mask_and_pattern = (char*)wl_pattern + sizeof(wl_wowl_pattern_t);

	wl_pattern->offset = 66;

	/* Parse the mask */
	mask = "0x0f";
	wl_pattern_atoh(mask, mask_and_pattern);
	wl_pattern->masksize = 1;
	mask_and_pattern += wl_pattern->masksize;
	wl_pattern->patternoffset = sizeof(wl_wowl_pattern_t) +	wl_pattern->masksize;

	/* Parse the pattern */
	pattern = "0x77616b65";
	wl_pattern->patternsize = 4;
	wl_pattern_atoh(pattern, mask_and_pattern);

	buf_len += sizeof(wl_wowl_pattern_t) + wl_pattern->patternsize + wl_pattern->masksize;

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = buf_len;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set wowl_pattern add, %s\n", strerror(errno));
	}

	return ret;
}

int wl_wowl(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	int ret = 0;
	int s = 0;
	uint32 wowl = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* wl wowl 0x20016 */
	wowl = 0x00016;
	wl_iovar_mkbuf("wowl", (char *)&wowl, sizeof(uint32), bufdata, WLC_IOCTL_MAXLEN, &ret);

	if (ret) {
		printf("Failed to build buffer, %s.\n", __FUNCTION__);
		return -1;
	}

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set wowl, %s\n", strerror(errno));
	}

	return ret;
}

int wl_wowl_activate(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	int ret = 0;
	int s = 0;
	uint32 wowl_activate = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s", net_socket.iface, strerror(errno));
		return -1;
	}

	/* wl wowl_activate 1 */
	wowl_activate = 1;
	wl_iovar_mkbuf("wowl_activate", (char *)&wowl_activate, sizeof(uint32),
		bufdata, WLC_IOCTL_MAXLEN, &ret);

	if (ret) {
		printf("Failed to build buffer, %s.\n", __FUNCTION__);
		return -1;
	}

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set wowl_activate, %s\n", strerror(errno));
	}

	return ret;
}

/*  UDP Keep alive */
int wl_mkeep_alive(u32 length, const u_char *packet)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	const char *str;
	wl_mkeep_alive_pkt_t  mkeep_alive_pkt;
	wl_mkeep_alive_pkt_t  *mkeep_alive_pktp;
	int str_len = 0;
	uint buf_len = 0;
	int ret = 0;
	int s = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* init mkeep_alive data */
	str = "mkeep_alive";
	str_len = strlen(str);
	strncpy(bufdata, str, str_len);
	bufdata[ str_len ] = '\0';
	buf_len = str_len + 1;
	mkeep_alive_pktp = (wl_mkeep_alive_pkt_t *) (bufdata + str_len + 1);

	memset(&mkeep_alive_pkt, 0, sizeof(wl_mkeep_alive_pkt_t));
	mkeep_alive_pkt.period_msec = net_socket.interval * 1000; // milliseconds
	mkeep_alive_pkt.version = WL_MKEEP_ALIVE_VERSION;
	mkeep_alive_pkt.length = WL_MKEEP_ALIVE_FIXED_LEN;
	mkeep_alive_pkt.keep_alive_id = 1;
	mkeep_alive_pkt.len_bytes = length;
	memcpy(mkeep_alive_pktp->data, packet, length);
	buf_len += WL_MKEEP_ALIVE_FIXED_LEN + mkeep_alive_pkt.len_bytes;

	memcpy((char *)mkeep_alive_pktp, &mkeep_alive_pkt, WL_MKEEP_ALIVE_FIXED_LEN);

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = buf_len;
	ioc.set = 0;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail mkeep_alive, %s\n", strerror(errno));
		return ret;
	}

	if (net_socket.verbose) {
		printf("UDP Interval [%d] seconds\n", net_socket.interval);
	}

	return ret;
}

int wl_pkt_filter_add(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	const char *str;
	wl_pkt_filter_t	 pkt_filter;
	wl_pkt_filter_t	 *pkt_filterp;
	int str_len = 0;
	uint buf_len = 0;
	int ret = 0;
	int s = 0;
	uint32 mask_size = 0;
	uint32 pattern_size = 0;
	char *mask, *pattern;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* wl pkt_filter_add 200 0 0 36 0xffff 0x1EC5 : Receive UDP from port 7877 */
	str = "pkt_filter_add";
	str_len = strlen(str);
	strncpy(bufdata, str, str_len);
	bufdata[ str_len ] = '\0';
	buf_len = str_len + 1;

	pkt_filterp = (wl_pkt_filter_t *) (bufdata + str_len + 1);

	pkt_filter.id = 200;
	pkt_filter.negate_match = 0;
	pkt_filter.type = 0;
	pkt_filter.u.pattern.offset = 34;

	mask_size = 2;
	mask = "0xffff";
	wl_pattern_atoh(mask, (char*) pkt_filterp->u.pattern.mask_and_pattern);
	pattern = "0x1EC5";
	pattern_size = 2;
	wl_pattern_atoh(pattern, (char*) &pkt_filterp->u.pattern.mask_and_pattern[mask_size]);

	pkt_filter.u.pattern.size_bytes = pattern_size;
	buf_len += WL_PKT_FILTER_FIXED_LEN;
	buf_len += (WL_PKT_FILTER_PATTERN_FIXED_LEN + 2 * mask_size);

	memcpy((char *)pkt_filterp, &pkt_filter,
		WL_PKT_FILTER_FIXED_LEN + WL_PKT_FILTER_PATTERN_FIXED_LEN);

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = buf_len;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set pkt_filter_add, %s\n", strerror(errno));
	}

	return ret;
}

int pkt_filter_enable(void)
{
	char bufdata[WLC_IOCTL_MAXLEN];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	wl_pkt_filter_enable_t enable_parm;
	int ret = 0;
	int s = 0;

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", net_socket.iface, strerror(errno));
		return -1;
	}

	/* Init pkt_filter data */
	enable_parm.id = 200;
	enable_parm.enable = 1;

	wl_iovar_mkbuf("pkt_filter_enable", (char *)&enable_parm,
		sizeof(wl_pkt_filter_enable_t), bufdata, WLC_IOCTL_MAXLEN, &ret);

	if (ret) {
		printf("Failed to build buffer, %s.\n", __FUNCTION__);
		return -1;
	}

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 1;

	strncpy(ifr.ifr_name, net_socket.iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	//printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set pkt_filter_enable, %s\n", strerror(errno));
	}

	return ret;
}

#if 0
int wl_pkt_filter_stats()
{
	char iface[PROP_VALUE_MAX];
	struct ifreq ifr;
	wl_ioctl_t ioc;
	wl_pkt_filter_stats_t	*stats;
	uint32						id;
	char bufdata[WLC_IOCTL_MAXLEN];
	int ret = 0;
	int s;

	strncpy(iface, net_socket.iface, PROP_VALUE_MAX);

	/* open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("cannot open socket %s - %s\n", iface, strerror(errno));
		return -1;
	}

	//id =
	wl_iovar_mkbuf("pkt_filter_stats", (char *)&id, sizeof(id), bufdata, WLC_IOCTL_MAXLEN, &ret);
	if (ret) {
		return -1;
	}

	ioc.cmd = WLC_SET_VAR;
	ioc.buf = &bufdata;
	ioc.len = WLC_IOCTL_MAXLEN;
	ioc.set = 1;

	strncpy(ifr.ifr_name, iface, IFNAMSIZ);
	ifr.ifr_data = (caddr_t) &ioc;

	printf("--- cmd: %d ---\n", ioc.cmd);
	if ((ret = ioctl(s, SIOCDEVPRIVATE, &ifr)) < 0) {
		printf("fail set pkt_filter_stats, %s\n", strerror(errno));
	}
	printf("ret: %d", ret);

	stats = (wl_pkt_filter_stats_t *) bufdata;

	//stats->num_pkts_matched
	//stats->num_pkts_discarded
	//stats->num_pkts_forwarded

	return ret;
}
#endif

int net_socket_client_tcp(void)
{
	int fd_client = -1;
	int ret = -1;

	struct sockaddr_in addr_srv;

	if ((fd_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	bzero(&addr_srv, sizeof(addr_srv));
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(net_socket.port);

	do {
		if (inet_pton(AF_INET, net_socket.srv_ip, &addr_srv.sin_addr) < 0 ) {
			perror("inet_pton");
			break;
		}

		if (connect(fd_client, (struct sockaddr *)&addr_srv, sizeof(struct sockaddr)) < 0) {
			perror("connect");
			break;
		}
		printf("Socket [%d], Host: %s:%d, Connect OK\n",
			fd_client, net_socket.srv_ip, net_socket.port);

		//sleep(2);

		/* Send once */
		ret = writen(fd_client, net_socket.send_msg, strlen(net_socket.send_msg));
		if (ret == strlen(net_socket.send_msg)) {
			printf("Send Length[%d] OK\n", ret);
		}
	} while(0);

	close_fd(&fd_client);

	return ret;
}

int net_socket_client_udp(void)
{
	int fd_client = -1;
	int ret = -1;

	struct sockaddr_in addr_srv;

	if ((fd_client = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	bzero(&addr_srv, sizeof(addr_srv));
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(net_socket.port);

	do {
		if (inet_pton(AF_INET, net_socket.srv_ip, &addr_srv.sin_addr) < 0 ) {
			perror("inet_pton");
			break;
		}
		printf("Socket [%d], Port [%d] OK\n", fd_client, net_socket.port);

		/* Send */
		ret = sendto(fd_client, net_socket.send_msg, strlen(net_socket.send_msg),
			0, (struct sockaddr *)&addr_srv, sizeof(addr_srv));
		if (ret == strlen(net_socket.send_msg)) {
			printf("Send Length[%d] OK\n", ret);
		}
	} while(0);

	close_fd(&fd_client);

	return ret;
}

void processPacket(u_char *arg, const struct pcap_pkthdr* pkthdr, const u_char * packet)
{
	int i = 0;
	int *counter = (int *)arg;
	char dst_mac[DATA_MAXSIZE];

	struct ip *iphdr = NULL;		/* IPv4 Header */
	struct tcphdr *tcphdr = NULL;	/* TCP Header  */
	unsigned int *tsval;			/* Time Stamp (optional) */
	unsigned int *tsvalR;			/* Time Stamp Reply (optional) */

	tcpka_conn_t tcpka;

	printf("Packet Count: %d\n", ++(*counter));

	if (net_socket.verbose) {
		printf("Received Packet Size: %d\n", pkthdr->len);
		printf("Payload:\n");
		for(i = 0; i < pkthdr->len; ++i) {
			printf(" %02x", packet[i]);
			if( (i + 1) % 16 == 0 ) {
				printf("\n");
			}
		}
		printf("\n\n");
	}

	if (net_socket.is_tcp) {
		iphdr = (struct ip *)(packet + 14);
		tcphdr = (struct tcphdr *)(packet + 14 + 20);
		tsval = (unsigned int *)(packet + 58);
		tsvalR = tsval + 1;

		if (tcphdr->psh) {
			memset(dst_mac, 0, sizeof(dst_mac));
			sprintf(dst_mac, "%02X:%02X:%02X:%02X:%02X:%02X",
				packet[0], packet[1], packet[2], packet[3], packet[4], packet[5]);

			if (net_socket.verbose) {
				printf("	FLags: PSH [%d]\n", tcphdr->psh);

				printf("	DST MAC: %s\n", dst_mac);
				printf("	DST IP: %s\n", inet_ntoa(iphdr->ip_dst));
				printf("	SRC IP: %s\n", inet_ntoa(iphdr->ip_src));
				printf("	SRC PORT: %d\n", ntohs(tcphdr->th_sport));
				printf("	DST PORT: %d\n", ntohs(tcphdr->th_dport));
				printf("	ID: %d\n", ntohs(iphdr->ip_id));
				printf("	SEQ: %u\n", ntohl(tcphdr->th_seq));
				printf("	ACK: %u\n", ntohl(tcphdr->th_ack));
				printf("	Win: %d\n", ntohs(tcphdr->th_win));
				printf("	TS val: %u\n", ntohl(*tsval));
				printf("	TS valR: %u\n", ntohl(*tsvalR));
			}

			/* Fill WiFi FW to KeepAlive and Wakeup Pattern */
			tcpka.sess_id = 1;
			tcpka.dst_mac = (struct ether_addr)* ether_aton(dst_mac);
			tcpka.dst_ip = iphdr->ip_dst;
			tcpka.src_ip = iphdr->ip_src;
			tcpka.ipid = ntohs(iphdr->ip_id);
			tcpka.srcport = ntohs(tcphdr->th_sport);
			tcpka.dstport = ntohs(tcphdr->th_dport);
			tcpka.seq = ntohl(tcphdr->th_seq);
			tcpka.ack = ntohl(tcphdr->th_ack);
			tcpka.tcpwin = ntohs(tcphdr->th_win);
			tcpka.tsval = ntohl(*tsval);
			tcpka.tsecr = ntohl(*tsvalR);

			wl_wowl_pattern();
			wl_tcpka_conn_add(tcpka);
			wl_tcpka_conn_enable();

			wl_wowl();
			wl_wowl_activate();

			printf("Set TCP KeepAlive Done\n");
		}
	} else {
		wl_mkeep_alive(pkthdr->len, packet);
		wl_pkt_filter_add();
		pkt_filter_enable();

		printf("Set UDP KeepAlive Done\n");
	}
	return;
}

static void load_default_param(void)
{
	net_socket.is_session = 0;
	net_socket.is_close = 0;
	net_socket.is_tcp = 1;
	net_socket.is_sys_exec = 0;
	net_socket.verbose = 0;
	net_socket.port = DEFAULT_PORT;
	net_socket.interval = DEFAULT_INTERVAL;

	memset(net_socket.iface, 0, sizeof(net_socket.iface));
	memset(net_socket.srv_ip, 0, sizeof(net_socket.srv_ip));
	memset(net_socket.client_ip, 0, sizeof(net_socket.client_ip));

	strncpy(net_socket.srv_ip, DEFAULT_HOST, sizeof(net_socket.srv_ip));
	strncpy(net_socket.send_msg, DEFAULT_MSG, sizeof(net_socket.send_msg));
	strncpy(net_socket.net_filter, DEFAULT_NET_FILTER, sizeof(net_socket.net_filter));
}

int main(int argc, char *argv[])
{
	int ret = -1;
	int count = 0;
	pthread_t tid = 0;
	pcap_t *descr = NULL;
	char *device = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program filter;

	if (argc < 2) {
		usage();
		return -1;
	}

	load_default_param();
	memset(errbuf, 0, PCAP_ERRBUF_SIZE);

	if (init_param(argc, argv) < 0) {
		usage();
		return -1;
	}

	printf("Date Version : %s\n", __TIME__);

	if (net_socket.iface[0] != '\0') {
		device = net_socket.iface;
	} else {
		if ( (device = pcap_lookupdev(errbuf)) == NULL) {
			printf("ERROR: %s\n", errbuf);
			return -1;
		}
	}

	printf("Opening interface: %s\n", device);
	if ((descr = pcap_open_live(device, MAXBYTES2CAPTURE, 1, 512, errbuf)) == NULL) {
		printf("ERROR: %s\n", errbuf);
		return -1;
	}

	if (net_socket.net_filter[0] != '\0') {
		pcap_compile(descr, &filter, net_socket.net_filter, 1, 0);
		pcap_setfilter(descr, &filter);
		printf("Set Sniffer Network Filter: [%s]\n", net_socket.net_filter);
	}

	if (net_socket.is_session) {
		if (net_socket.is_tcp) {
			printf("Create TCP session\n");
			ret = pthread_create(&tid, NULL, (void *)net_socket_client_tcp, NULL);
			if (ret != 0) {
				printf("Create pthread error\n");
			}
		} else {
			printf("Create UDP session\n");
			ret = pthread_create(&tid, NULL, (void *)net_socket_client_udp, NULL);
			if (ret != 0) {
				printf("Create pthread error\n");
			}
		}
	}

	if (pcap_loop(descr, -1, processPacket, (u_char *)&count) == -1) {
		printf("ERROR: %s\n", pcap_geterr(descr) );
		return -1;
	}

	pcap_close(descr);

	return 0;
}

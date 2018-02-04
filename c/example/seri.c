#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int open_port(int fd, int comport)
{
	int fcntl_r;
	if (1 == comport) {
		fd = open("/dev/ttyS0", O_RDWR | O_CREAT | O_NDELAY);
		if(fd < 0) {
			perror("open");
			return -1;
		} else {
			printf("open %d seri ....\n", comport);
		}
	}

	if (fcntl(fd, F_SETFL, 0) < 0) {
		perror("fcntl");
		return -1;
	} else {
		printf("sucessed fcntl seri\n");
	}

	if (isatty(STDIN_FILENO) == 0) {
		perror("isatty");
		return -1;
	} else {
		printf("seri ok\n");
	}

	return fd;
}

int set_opt(int fd, int nSpeed, int nBit, int nEvent, int nStop)
{
	struct termios newtio,oldtio;

	if (tcgetattr(fd, &oldtio) != 0) {
		perror("tcgetattr");
		return -1;
	}

	bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = CLOCAL | CREAD;

	switch(nSpeed) {
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	newtio.c_cflag = ~CSIZE;
	switch(nBit) {
	case 7:
		newtio.c_cflag= CS7;
		break;
	case 8:
		newtio.c_cflag= CS8;
		break;
	default:
		newtio.c_cflag= CS8;
		break;
	}

	switch(nEvent) {
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E':
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':
		newtio.c_cflag &= ~PARENB;
		break;
	default:
		break;

	}
	if (nStop == 1) {
		newtio.c_cflag &= ~CSTOPB;
	} else if (nStop == 2) {
		newtio.c_cflag |= CSTOPB;
	}
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0;

	tcflush(fd,TCIFLUSH);
	if (tcsetattr(fd, TCSANOW, &newtio) !=0) {
		perror("tcsetattr");
		return -1;
	}

	printf("sucssed set seri\n");
	return 1;
}

int main(int argc, char *argv[])
{
	int fd, read_c;
	char buf_w[] = "hello\n";
	char buf[8] = {0};

	if (fd = open_port(fd, 0) < 0) {
		perror("open_port");
		return -1;
	}
	if (set_opt(fd, 9600, 8, 'N', 1) < 0) {
		perror("set_opt");
		return -1;
	}

	printf("fd: %d\n", fd);
	write(fd, buf_w, sizeof(buf_w));
	read_c = read(fd, buf, sizeof(buf));
	printf("read %d byte: %s\n", read_c, buf);

	return 0;
}

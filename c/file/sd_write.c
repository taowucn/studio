
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <memory.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>

#ifdef __UCLIBC__
#include <bits/getopt.h>
#else
#include <getopt.h>
#endif

#define NO_ARG		0
#define HAS_ARG		1

#define FRE_30		33333	//frequency = 30
#define FRE_60		16667	//frequcncy = 60

//static char filename[256]= "/tmp/mmcblk0p1/data.out";	//write to "/tmp/mmcblk0p1/data.out" by default
static char filename[256] = "data.out";
static int fd;					//open file
static char *buf;

//--------------default wirting speed = 1kB/s-----------------

static int  write_size = 1024;		//the default writing size is 1kB
static int write_time = 1;			//write once a period by default

static int init_sec = 1;			//the default writing frequency is 1Hz
static int init_usec = 0;

static int frequency = 1;			//only be used to show time
static unsigned int num = 1;
//----------------------------------------------------
struct timeval time_point_start, time_point_end;	//be used to record time

//static int timer = 1;

static int showTime = 0;			//whether show time

static struct option long_options[] = {
		{"Help", 					NO_ARG, 		NULL, 	'h'},
		{"The path to write",			HAS_ARG, 	NULL, 	'p'},
		{"Writing frequency", 		HAS_ARG, 	NULL, 	'f'},
		{"Write x size ", 			HAS_ARG, 	NULL, 	's'},
		{"Wirte x times in a period",	HAS_ARG, 	NULL, 	't'},
		{"Print actully writing speed",	NO_ARG, 		NULL, 	'm'},

		{0, 0, 0, 0},
};

struct hint_s{
	const char *arg;
	const char *str;
};

static const char *short_options = "hp:f:s:t:m";

static const struct hint_s hint[] = {
		{"","\t\t\tPrint command parm"},
		{"","\t\tThe data path"},
		{"","\t\tUnits: Hz"},
		{"","\t\tUnits: Byte"},
		{"","\tInteger"},
		{"","Unit: Bps"},
};

void usage(void)
{
	int i;
	printf("\nUsage: sd_write -p [PATHNAME] -f [FREQUENCY] -s [SIZE] -t [TIME]\n");

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
}

int init_opt(int argc, char **argv)
{
	int ch;
	int option_index = 0;
	float fvalue = 1;

	opterr = 0;

	while ( (ch = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1){
		switch(ch){
			case 'p':
				strcpy(filename, optarg);
				break;
			case 'f':
				fvalue = atof(optarg);
				frequency = (int) fvalue;
				if (fvalue == (float)30){
					init_sec = 0;
					init_usec = FRE_30;
				}else if (fvalue == (float)60){
					init_sec = 0;
					init_usec = FRE_60;
				}else if (fvalue == (float)1){
					init_sec = 1;
					init_usec = 0;
				}else {
					init_sec = (int)  ( 1 / fvalue );
					init_usec =(int) ( ((1 / fvalue) - init_sec) * 1000000 );
				}
				break;
			case 's':
				write_size = atoi(optarg);
				break;
			case 't':
				write_time = atoi(optarg);
				break;
			case 'm':
				showTime = 1;
				break;
			case 'h':
				usage();
				exit(0);
				break;
			default:
				printf("unknown option found %c or option requires an argument \n", ch);
				return -1;
				break;
		}
	}
	printf("Writing file: %s\n", filename);
	printf("Writing speed: write_size  x  write_time  x  frequency =\n");
	printf("\t\t%d x %d x %f = %.1f Byte/s\n", write_size, write_time , fvalue, ( (float)write_size ) * ( (float) write_time ) * fvalue);
	return 0;
}

//quit process when tpye 'q'.
int loop_parm(void)
{
	char buffer[256];
	 if (read (STDIN_FILENO, buffer, sizeof (buffer)) < 0) {
	 	return -1;
	 } else if ( buffer[0] == 'q' ) {
	 	printf("Quit!\n");
	 	return 0;
	 }

	 return 0;
}


int open_file(void)
{
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0666);
	if( fd < 0 ) {
		perror("write:");
		return -1;
	}

	buf = (char *)malloc(write_size);
	memset(buf, '0', write_size);
	return 0;
}

void write_sd(int signo)
{
	int i;
	unsigned int time_interval_us;
	struct timeval time_point1, time_point2;
	if (signo == SIGALRM){
		if ( fd ){
			num++;

			if ( showTime && (!(num % frequency)) ){
				gettimeofday(&time_point1, NULL);
				//printf("gettimeofday1 : tv_sec =%d, tv_usec =%d \n",  time_point1.tv_sec, time_point1.tv_usec);
			}

			for ( i = 0; i < write_time; i++){
				if(write( fd, buf, write_size) != write_size){
					printf("write error!\n");
					exit(1);
				}
			}

			if ( showTime && (!(num % frequency)) ){
				gettimeofday(&time_point2, NULL);
				//printf("gettimeofday2 : tv_sec =%d, tv_usec =%d \n",  time_point2.tv_sec, time_point2.tv_usec);
				time_interval_us = (time_point2.tv_sec - time_point1.tv_sec) * 1000000 +
								time_point2.tv_usec - time_point1.tv_usec;
				if ( time_interval_us != 0)
					printf("Actually writing speed is %.1f Bps\n",  ( (double) (write_size * write_time ) / (time_interval_us/1000000.f)));
				num = 1;
			}
		}
		else
			printf("File not found!\n");
	}
}

static void sigstop()
 {
	double time_interval_us;

	gettimeofday(&time_point_end, NULL);
	time_interval_us = (time_point_end.tv_sec - time_point_start.tv_sec) + (time_point_end.tv_usec - time_point_start.tv_usec) / 1000000.f;

	printf("All process spend about %f sec. \n", time_interval_us);

	if( close(fd) < 0 )
		exit(1);
	free(buf);
}
// This function not work when add "pause()" in main().
/*
void print_timer(int signo)
{
	if (signo == SIGVTALRM)
		printf("Timer: %d sec.\n",timer ++);
}
*/
int init_time(void)
{
	struct itimerval value, ovalue;

	signal(SIGALRM, write_sd);
	//signal(SIGVTALRM, print_timer);
	printf("init_sec  = %d, init_usec = %d\n", init_sec, init_usec);
	value.it_value.tv_sec = init_sec;
	value.it_value.tv_usec = init_usec;
	value.it_interval = value.it_value;
	setitimer(ITIMER_REAL, &value, &ovalue);
	// This function not work when add "pause()" in main().
	/*
        value2.it_value.tv_sec = 1;
        value2.it_value.tv_usec = 0;
        value2.it_interval  =  value2.it_value;
        setitimer(ITIMER_VIRTUAL, &value2, &ovalue);
	*/
	return 0;
}

int main(int argc , char **argv)
{
	 //register signal handler for Ctrl+C,  Ctrl+'\'  ,  and "kill" sys cmd
	signal(SIGINT, 	sigstop);
	signal(SIGQUIT,	sigstop);
	signal(SIGTERM,sigstop);

	if (argc < 2){
		usage();
		return -1;
	}
	//printf("Process id is %d\n",getpid());

	if (init_opt(argc, argv) != 0)
		return -1;

	if (open_file() != 0)
		return -1;

	if (init_time() != 0)
		return -1;

	gettimeofday(&time_point_start, NULL);

	while ( 1 ){
		if (!loop_parm()) {
			sigstop();
			return 0;
		}
	}

	if( close(fd) < 0 )
		return -1;

	free(buf);
	return 0;
}


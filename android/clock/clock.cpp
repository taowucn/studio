#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int main(){

	int pid;

	printf("start--------\n");
	sleep(3);
	struct timespec now, now1;
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now);
	printf("now0=%lld\n", now.tv_sec * 1000000000LL+now.tv_nsec);

	int i = 1000000000;
	while(i--);
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &now1);
	printf("now1=%lld\n", now1.tv_sec * 1000000000LL+now1.tv_nsec);

	sleep(3);
	struct timespec now2, now3;
	clock_gettime(CLOCK_REALTIME, &now2);
	printf("now2=%lld\n", now2.tv_sec * 1000000000LL+now2.tv_nsec);
	int m = 1000000000;

	while(m--);
	clock_gettime(CLOCK_MONOTONIC, &now3);
	printf("now3=%lld\n", now3.tv_sec * 1000000000LL+now3.tv_nsec);
	printf("end --------\n");

	return 0;
}



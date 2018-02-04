#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handle(void)
{
	system("date");
	printf("Alarm is here\n");
}

int main(int argc, char *argv[])
{
	int ret = 0, i = 0;

	signal(SIGALRM, sig_handle);
	system("date");
	ret=alarm(3);
	for (i=0; i < 5; i++) {
		printf("sleep %d ...\n", i);
		sleep(1);
	}
	printf("exit now, ret=%d\n", ret);

	return 0;
}

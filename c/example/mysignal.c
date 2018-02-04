#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void my_func(int sign_no)
{
	if (sign_no == SIGINT) {
		printf("I get a SIGINT message!");
	}
	if (sign_no == SIGQUIT) {
		printf("I get a SIGQUIT message!");
	}
}

int main(int argc, char *argv[])
{
	signal(SIGINT, my_func);
	signal(SIGQUIT, my_func);

	printf("waiting for signal\n");
	pause();

	return 0;
}

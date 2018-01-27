#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
void my_func(int sign_no)
{
	if(sign_no==SIGINT)
		printf("I get a SIGINT message!");
	if(sign_no==SIGQUIT)
		printf("I get a SIGQUIT message!");
}
int main()
{
	printf("wait for you message !");
	signal(SIGINT,my_func);
	signal(SIGQUIT,my_func);
	pause();
	exit(0);
}


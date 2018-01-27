#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
void my_func(int signum)
{
	printf("if you want quit ,please send QUIT!\n");
}
int main()
{
	sigset_t sig1,sig2;
	struct sigaction act1,act2;
	if(sigemptyset(&sig1)<0)
		perror("sigemptyset");
	if(sigaddset(&sig1,SIGINT)<0)
		perror("sigaddset");
	if(sigaddset(&sig1,SIGQUIT)<0)
		perror("sigaddset");
	if(sigprocmask(SIG_BLOCK,&sig1,NULL)<0)
		perror("sigprocmask");
	else
	{
		printf("sig blocked");
		sleep(5);
	}
	if(sigprocmask(SIG_UNBLOCK,&sig2,NULL)<0)
		perror("sigprocmask");
	else
		printf("sig unblocked");
	while(1)
	{
		if(sigismember(&sig1,SIGINT))
		{
			sigemptyset(&act1.sa_mask);
			act1.sa_handler=my_func;
			sigaction(SIGINT,&act1,NULL);
		}
		else if(sigismember(&sig1,SIGQUIT))
		{
			sigemptyset(&act2.sa_mask);
			act2.sa_handler=SIG_DFL;
			sigaction(SIGQUIT,&act2,NULL);
		}
	}



}

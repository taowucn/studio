#include <stdio.h>
#include <signal.h>
int nInterrupt;

void handle_sigint(int signo)
{
    nInterrupt = 1;
}

int main(int argc, char *argv[])
{
    sigset_t mask,omask;
    signal(SIGINT,handle_sigint);
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,&omask);//阻塞
    nInterrupt = 0;
    while(!nInterrupt)//循环调用sigsuspend等待信号，直到收到SIGINT，nInterrupt为1
        sigsuspend(&omask);//阻塞信号直到有信号到达
    printf("process return.\n");
}
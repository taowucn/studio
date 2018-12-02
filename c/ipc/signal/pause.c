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
    sigprocmask(SIG_BLOCK,&mask,&omask);//����
    nInterrupt = 0;
    while(!nInterrupt)//ѭ������sigsuspend�ȴ��źţ�ֱ���յ�SIGINT��nInterruptΪ1
        sigsuspend(&omask);//�����ź�ֱ�����źŵ���
    printf("process return.\n");
}
#include <stdio.h>
#include <signal.h>

void handle_sigint(int signo)
{
    printf("receive signal %d\n",signo);
}

main()
{
    sigset_t mask;
    sigset_t omask;
    sigset_t pmask;
    signal(SIGINT,handle_sigint);
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,&omask);
    sleep(3);
    if(sigpending(&pmask) < 0)//��ȡ��ǰδ�����źż�
    {
        perror("sigpending");
        exit(0);
    }
    if(sigismember(&pmask,SIGINT))//�ж�SIGINT�Ƿ���δ���źż���
    {
        printf("SIGINT signal is pending.\n");
    }
    printf("exit.\n");
}
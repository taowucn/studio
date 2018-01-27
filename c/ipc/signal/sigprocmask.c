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
    signal(SIGINT,handle_sigint);
    sigemptyset(&mask);//����źŴ�������
    sigaddset(&mask,SIGINT);//�������������ź�
    sigprocmask(SIG_BLOCK,&mask,&omask);//�������룬������ɺ�SIGINT�źű�����
    sleep(3);
    sigprocmask(SIG_SETMASK,&omask,NULL);//�ָ�ԭ�е��źŴ�������
}
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void HandleSigint(int signo,siginfo_t *info,void *none)
{
    printf("receive addtional data is %d\n",info->si_value.sival_int);
    exit(0);
}

main()
{
    int pid;
    struct sigaction act;
    union sigval sigvalPara;
    if((pid = fork()) == 0)
    {
        memset(&act,0x00,sizeof(struct sigaction));
        sigemptyset(&act.sa_mask);
        act.sa_sigaction = HandleSigint;
        act.sa_flags = SA_SIGINFO;
        if(sigaction(SIGINT,&act,NULL) == -1)
        {
            perror("sigaction");
            exit(0);
        }
        pause();//��ͣ�ӽ��̣��ȴ��ź�
    }
    else
    {
        sleep(3);
        sigvalPara.sival_int = 123;//���ø�������Ϊ123
        if(sigqueue(pid,SIGINT,sigvalPara) == -1)//���ӽ��̷����ź�SIGINT������������
        {
            perror("sigqueue");
            exit(0);
        }
    }
}
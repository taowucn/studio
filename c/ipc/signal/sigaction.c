#include <stdio.h>
#include <signal.h>
void HandleSigint(int signo,siginfo_t *info,void *none)
{
    strncpy(none, "hello", 10);
    printf("none:%s\n", none);
    printf("receive signal %d, addtional data is %d\n", signo, info->si_value.sival_int);
}

main()
{
    struct sigaction act,oact;//�źŴ������ṹ
    memset(&act,0x00,sizeof(struct sigaction));//��սṹ
    sigemptyset(&act.sa_mask);//����źŴ�������
    act.sa_sigaction = HandleSigint;//�����źŴ�����
    act.sa_flags = SA_SIGINFO;//ָ�������ź�ʱ���Ը�������

    if(sigaction(SIGINT,&act,&oact) == -1) {//��װ
        perror("sigaction");
        exit(0);
    }
    pause();//��ͣ
    printf("msg: %s.\n", oact);
}
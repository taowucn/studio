#include <stdio.h>
#include <signal.h>
void HandleSigint(int signo)//�źŴ�����
{
    printf("receive signal %d\n",signo);
}
main()
{
    if(signal(SIGINT,HandleSigint)  == SIG_ERR)//��װ�ź�
    {
        perror("signal");
        exit(0);
    }
    pause();//��ͣ���̵ȴ��ź�
}
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void HandleSigint(int signo)//�źŴ�����
{
    printf("receive signal %d\n",signo);
}

int main(int argc, char *argv[])
{
    if(signal(SIGINT,HandleSigint)  == SIG_ERR)//��װ�ź�
    {
        perror("signal");
        exit(0);
    }
    pause();//��ͣ���̵ȴ��ź�
}
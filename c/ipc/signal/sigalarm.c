#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int timeout;

void Handle_Alarm(int signo)
{
    timeout = 1;
    printf("SIGALRM received.\n");
}

int main(int argc, char *argv[])
{
    if(signal(SIGALRM,Handle_Alarm) ==SIG_ERR )//��װSIGALRM�ź�
    {
        perror("signal");
        exit(0);
    }
    timeout = 0;//���ó�ʱ��־Ϊ0
    alarm(5);//������ʱ��
    pause();//�������̣��ȴ��ź�
    if(timeout)//�����ʱ
    {
        printf("Pause time out.\n");
    }
}
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
    if(signal(SIGALRM,Handle_Alarm) ==SIG_ERR )//安装SIGALRM信号
    {
        perror("signal");
        exit(0);
    }
    timeout = 0;//设置超时标志为0
    alarm(5);//启动定时器
    pause();//阻塞进程，等待信号
    if(timeout)//如果超时
    {
        printf("Pause time out.\n");
    }
}
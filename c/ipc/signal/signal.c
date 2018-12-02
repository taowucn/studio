#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void HandleSigint(int signo)//信号处理函数
{
    printf("receive signal %d\n",signo);
}

int main(int argc, char *argv[])
{
    if(signal(SIGINT,HandleSigint)  == SIG_ERR)//安装信号
    {
        perror("signal");
        exit(0);
    }
    pause();//暂停进程等待信号
}
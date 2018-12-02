#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int signo)
{
    printf("receive signal %d\n",signo);
}

int main(int argc, char *argv[])
{
    sigset_t mask;
    sigset_t omask;
    signal(SIGINT,handle_sigint);
    sigemptyset(&mask);//清空信号处理掩码
    sigaddset(&mask,SIGINT);//向掩码中增加信号
    sigprocmask(SIG_BLOCK,&mask,&omask);//设置掩码，设置完成后SIGINT信号被阻塞
    sleep(3);
    sigprocmask(SIG_SETMASK,&omask,NULL);//恢复原有的信号处理掩码
}
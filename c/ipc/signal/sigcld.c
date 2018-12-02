#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

void Handle_Sigcld(int signo)
{
     int pid,status;
     pid = waitpid(-1,&status,0);
     printf("Child process %d exit with code %d\n",pid,status);
}

int main(int argc, char *argv[])
{
    int i,pid;
    signal(SIGCLD,Handle_Sigcld);
    for(i=0;i<5;i++)
    {
        if((pid = fork()) == 0)//子进程
        {
            srand(getpid());//产生随机数
            exit((int)i);//退出子进程，退出码为上步随机数
        }
        else
        {//父进程
            sleep(1);//休眠
            continue;//继续
        }
    }
}
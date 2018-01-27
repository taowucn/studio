#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
    int pfd[2];	//保存打开管道后的两个文件描述符
    pid_t cpid;//保存进程标识符
    char buf;
    if(argc != 2)//判断命令行参数是否符合
    {
        fprintf(stderr,"Usage: %s <string>\n",argv[0]);
        exit(0);
    }
    if (pipe(pfd) == -1)//建立管道
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)//子进程
    {
        close(pfd[1]);          //关闭管道写，引用计数-1
        printf("child process...\n");
        while (read(pfd[0], &buf, 1) > 0) {//从管道循环读取数据
            write(STDOUT_FILENO, &buf, 1);//输出读到的数据
        }
        write(STDOUT_FILENO, "\n", 1);//输出从管道读取的数据
        close(pfd[0]);//关闭管道读，引用计数-1
        exit(0);
    }
    else
    {//父进程
        close(pfd[0]);
        printf("father process...\n");
        write(pfd[1], argv[1], strlen(argv[1]));//向管道写入命令行参数1
        close(pfd[1]);
        wait(NULL);           //等待子进程退出
        exit(0);
    }
}
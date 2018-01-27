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
    struct sigaction act,oact;//信号处理函数结构
    memset(&act,0x00,sizeof(struct sigaction));//清空结构
    sigemptyset(&act.sa_mask);//清空信号处理掩码
    act.sa_sigaction = HandleSigint;//定义信号处理函数
    act.sa_flags = SA_SIGINFO;//指定发送信号时可以附加数据

    if(sigaction(SIGINT,&act,&oact) == -1) {//安装
        perror("sigaction");
        exit(0);
    }
    pause();//暂停
    printf("msg: %s.\n", oact);
}
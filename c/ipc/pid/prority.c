#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
main()
{
    int nPr;
    if (nice(3) == -1) { //进程的谦让值是3，优先级降低
        perror("nice");
        exit(0);
    }
    errno = 0;
    nPr = getpriority(PRIO_PROCESS,getpid());//获取当前进程的谦让值
    if (errno != 0)  {
        perror("getpriority");
        exit(0);
    }
    printf("priority is %d\n", nPr);
}
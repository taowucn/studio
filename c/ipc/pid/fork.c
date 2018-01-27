#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
main()
{
    fprintf(stderr,"fork...\n");
    if(fork() == 0 )
    {
        printf("1. process id=%d\n",getpid());
        printf("parent process id=%d\n",getppid());
        printf("process group id=%d\n",getpgrp());
        printf("wait...");
        wait();
        printf("wait end\n");
        exit(0);
    }
        printf("2. process id=%d\n",getpid());
        printf("parent process id=%d\n",getppid());
        printf("process group id=%d\n",getpgrp());
        printf("sleep...");
        sleep(10);
        printf("sleep end\n");
        exit(0);
}
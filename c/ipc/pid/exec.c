#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
main()
{
    pid_t pid;
    char *para[]={"ls","-a",NULL};
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(0);
    }
    if (pid == 0) {
        printf("1. son \n");
        if (execl("/bin/ls","ls","-l",(char *)0) == -1) {
            perror("execl");
            exit(0);
        }
    }
#if 0
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(0);
    }
    if (pid == 0) {
        printf("1. son\n");
        if(execv("/bin/ls",para) == -1) {
            perror("execl");
            exit(0);
        }
    }
#endif
    printf("Main Exit\n");
    return;
}
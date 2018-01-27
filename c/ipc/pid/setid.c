#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>
main()
{
    int n;
    __pid_t nPid;
    __pid_t nGroupId;
    if ((nPid = fork()) < 0) {
        perror("Error");
        exit(0);
    }
    if (nPid != 0) { //������
        printf("Parent: wait...\n");
	 //wait(0);
        printf("wait end\n");
        exit(0);//�������˳�
    }else {
        printf("Son: enter...\n");
        nGroupId = setsid(); //�»Ự
        if (nGroupId == -1) {
            perror("setsid");
            exit(0);
        }
        printf("sleep...\n");
        sleep(3);
        printf("sleep end\n");
        //exit(0);
    }
    printf("Main Exit\n");
}
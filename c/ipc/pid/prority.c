#include <unistd.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
main()
{
    int nPr;
    if (nice(3) == -1) { //���̵�ǫ��ֵ��3�����ȼ�����
        perror("nice");
        exit(0);
    }
    errno = 0;
    nPr = getpriority(PRIO_PROCESS,getpid());//��ȡ��ǰ���̵�ǫ��ֵ
    if (errno != 0)  {
        perror("getpriority");
        exit(0);
    }
    printf("priority is %d\n", nPr);
}
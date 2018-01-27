#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
main()
{
    if(mkfifo("p3",0644) < 0)
    {
        perror("mkfifo");
        exit(-1);
    }
    return;
}
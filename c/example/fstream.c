#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define BUFSIZE 1024

int main()
{
	FILE *fp;
	char *cmd="ps -ef";
	char buf[BUFSIZE];
	fp=popen(cmd,"r");
	if(fp==NULL)
	{
		perror("popen");
	}
	while((fgets(buf,BUFSIZE,fp))!=NULL)
	{
		printf("%s",buf);
	}
	pclose(fp);
	return 0;
}


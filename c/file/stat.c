#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

char*  getProtection(mode_t vProtection)
{
        char *cProtection="---------";

        if(S_IRUSR & vProtection)
                cProtection[0]='R';
        if(S_IWUSR & vProtection)
                cProtection[1]='W';
        if(S_IXUSR & vProtection)
                cProtection[2]='X';

        return cProtection;
}

int main(int argc, char **argv)
{
	struct stat s;
	//char *p;
	if (stat ("/cygdrive/v/taowu/study/exec/execve", &s) < 0){
		printf("get faild\n");
		return -1;
	}
	//p = getProtection(s.st_mode);
	printf("Msg: mode=%d\n", s.st_mode);
	return 0;
}

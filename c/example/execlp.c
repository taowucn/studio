#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	if(fork()==0)
	{
		if(execlp("ps","ps","-ef",NULL)<0)
			{
				perror("execlp error!");
			}

	}
	return 0;

}

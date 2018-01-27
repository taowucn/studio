#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int ret;
	ret=alarm(5);
	pause();
	printf("i have stop\n");
	exit(0);
}

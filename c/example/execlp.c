#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (fork() == 0) {
		if (execlp("ps", "ps", "-ef", NULL) < 0) {
			perror("execlp");
			return -1;
		}
	}

	return 0;
}

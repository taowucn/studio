#include <stdio.h>
#include <string.h>

static void overflow(char* arg)
{
	char buf[4];
	strcpy(buf, arg);
}

int main(int argc, char *argv[])
{
	if (argc == 2) {
		overflow(argv[1]);
	} else {
		printf("Usage: %s <string>\n", argv[0]);
    }

	return 0;
}

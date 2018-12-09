#include<string.h>

static void overflow(char* arg)
{
	char buf[4];
	strcpy(buf, arg);
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		overflow(argv[1]);
	}

	return 0;
}

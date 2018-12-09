#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

static int i = 0;

static void *test(void *s)
{
	int buffer[1024];

	printf("i = %d\n", i);
	i++;

	buffer[0] = i;
	test(s);
}

int main(void)
{
	pthread_t p;

	pthread_create(&p, NULL, &test, NULL);
	pthread_join(p, NULL);
	printf("done\n");

	return 0;
}

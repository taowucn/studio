#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

static int i = 0;

static void *test(void *s)
{
	int buffer[1024];

	i++;
	buffer[0] = i;
	printf("i: %d, buf: %d\n", i, buffer[0]);

	test(s);

	return NULL;
}

int main(void)
{
	pthread_t p;

	pthread_create(&p, NULL, &test, NULL);
	pthread_join(p, NULL);
	printf("done\n");

	return 0;
}

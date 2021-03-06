#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

static int myglobal = 0;
static pthread_mutex_t mymutex;

void *thread_function(void *arg)
{
	int i = 0;

	for (i = 0; i < 20; i++) {
		pthread_mutex_lock(&mymutex);
		myglobal++;
		printf(".");
		fflush(stdout);
		pthread_mutex_unlock(&mymutex);
	}
}

int main(int argc, char **argv)
{
	pthread_t mythread;
	int i = 0;

	pthread_mutex_init(&mymutex, NULL);
	if (pthread_create(&mythread, NULL, thread_function, NULL)) {
		printf("error creating thread.");
		return -1;
	}

	for (i = 0; i < 20; i++) {
		pthread_mutex_lock(&mymutex);
		myglobal++;
		pthread_mutex_unlock(&mymutex);
		printf("o");
		fflush(stdout);
	}

	pthread_join(mythread, NULL);

	printf("\nmyglobal equals %d\n", myglobal);
	return 0;
}

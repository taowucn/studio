#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int myglobal;

pthread_mutex_t mymutex;

void *thread_function(void *arg)
{
	int i,j;

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
	int i;

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
	if (pthread_join(mythread, NULL)) {
		printf("error joining thread.\n");
		return -1;
	}

	printf("\n myglobal equals %d\n", myglobal);
	return 0;
}

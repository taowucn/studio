#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static pthread_mutex_t mtx;
static pthread_cond_t cond;

static void *thread_func(void*arg)
{
	pthread_mutex_lock(&mtx);
	printf("Wait Start\n");
	pthread_cond_wait(&cond, &mtx);
	printf("Wait Done\n");
	pthread_mutex_unlock(&mtx);
}

int main(int argc, char **argv)
{
	pthread_t tid0;
	pthread_t tid1;
	int i = 0;
	int ret = 0;

	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);
	do {
		ret = pthread_create(&tid0, NULL, thread_func, NULL);
		if (ret) {
			perror("pthrea_create");
			break;
		}
		ret = pthread_create(&tid1, NULL, thread_func, NULL);
		if (ret) {
			perror("pthrea_create");
			break;
		}
		sleep(1);
	} while (0);

	for (i = 0; i < 2; i++) {
		pthread_mutex_lock(&mtx);
		pthread_cond_signal(&cond);
		//pthread_cond_broadcast(&cond);
		printf("Signal\n");
		pthread_mutex_unlock(&mtx);
		sleep(1);
	}

	pthread_join(tid1, NULL);
	pthread_join(tid0, NULL);
	printf("Main Quit\n");
	return 0;
}

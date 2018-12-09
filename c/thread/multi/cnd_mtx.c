#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static pthread_mutex_t mtx;
static pthread_cond_t cond;

static void *show_me(void*arg)
{
	printf("Show me\n");
}

static void *thread_func_recv(void*arg)
{
	pthread_t tid;
	int ret = 0;

	//sleep(1);

	pthread_mutex_lock(&mtx);
	printf("Wait Start\n");
	pthread_cond_wait(&cond, &mtx);
	printf("Wait Done\n");
	pthread_mutex_unlock(&mtx);

	ret = pthread_create(&tid, NULL, show_me, NULL);
	if (ret) {
		perror("pthrea_create");
	}
	pthread_join(tid, NULL);
}

static void *thread_func_send(void*arg)
{
	sleep(1);

	pthread_mutex_lock(&mtx);
	pthread_cond_signal(&cond);
	printf("Signal\n");
	pthread_mutex_unlock(&mtx);
}


int main(int argc, char **argv)
{
	pthread_t tid[4];
	int i = 0;
	int ret = 0;

	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);
	do {
		ret = pthread_create(&tid[0], NULL, thread_func_recv, NULL);
		if (ret) {
			perror("pthrea_create");
			break;
		}
		ret = pthread_create(&tid[1], NULL, thread_func_recv, NULL);
		if (ret) {
			perror("pthrea_create");
			break;
		}

		ret = pthread_create(&tid[2], NULL, thread_func_send, NULL);
		if (ret) {
			perror("pthrea_create");
			break;
		}
		ret = pthread_create(&tid[3], NULL, thread_func_send, NULL);
		if (ret) {
			perror("pthrea_create");
			break;
		}
	} while (0);

	for (i = 0; i < 4; ++i) {
		pthread_join(tid[i], NULL);
	}

	printf("Main Quit\n");
	return 0;
}

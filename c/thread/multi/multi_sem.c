#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t bin_sem;

void *thread_function1(void *arg)
{
	printf("T1: sem_wait ...\n");
	sem_wait(&bin_sem);
	printf("T1: sem_wait done\n");
}

void *thread_function2(void *arg)
{
	printf("T2: sem_post ...\n");
	sem_post(&bin_sem);
	printf("T2: sem_post done\n");
}

int main(int argc, char **argv)
{
	void *thread_result = NULL;
	pthread_t a_thread, b_thread;
	int res = 0;

	res = sem_init(&bin_sem, 0, 0);
	if (res != 0) {
		perror("Semaphore initialization err\n");
	}

	res = pthread_create(&a_thread, NULL, thread_function1, NULL);
	if (res != 0) {
		perror("pthread_create err");
	}

	printf("sleep ...\n");
	sleep(2);
	printf("sleep done\n");

	res = pthread_create(&b_thread, NULL, thread_function2, NULL);
	if (res != 0) {
		perror("pthread_create err");
	}

	pthread_join(b_thread, thread_result);
	pthread_join(a_thread, thread_result);

	sem_destroy(&bin_sem);

	return 0;
}

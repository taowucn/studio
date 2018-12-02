#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t bin_sem;

void *thread_function1(void *arg)
{
	printf("thread_1, sem_wait\n");
	sem_wait(&bin_sem);
	printf("sem_wait\n");
}

void *thread_function2(void *arg)
{
	printf("thread_2, sem_post\n");
	sem_post(&bin_sem);
	printf("sem_post\n");
}

int main(int argc, char **argv)
{
	int res;
	pthread_t a_thread, b_thread;
	void *thread_result;

	res = sem_init(&bin_sem, 0, 0);
	if (res != 0){
		perror("Semaphore initialization failed");
	}
	printf("sem_init\n");

	res = pthread_create(&a_thread, NULL, thread_function1, NULL);
	if (res != 0){
		perror("Thread creation failure");
	}
	printf("sleep -start\n");
	sleep(2);
	printf("sleep -end\n");

	res = pthread_create(&b_thread, NULL, thread_function2, NULL);
	if (res != 0){
		perror("Thread creation failure");
	}

	pthread_join(b_thread, thread_result);
	pthread_join(a_thread, thread_result);

	sem_destroy(&bin_sem);

	return 0;
}


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threadpool.h"

#define DEFAULT_TASK_CNT    10
#define DEFAULT_THREAD_CNT  3

void *task_routine(void* arg)
{
	pthread_t tid = pthread_self();
	int task_id = *(int*)arg;

	free(arg);
	printf("Thread %#lx working on task %d\n", (size_t)tid, task_id);
	sleep(1);

	return NULL;
}

int main(int argc, char** argv)
{
	int taskcnt, threadcnt, ch;
	taskcnt = DEFAULT_TASK_CNT;
	threadcnt = DEFAULT_THREAD_CNT;
	char *prog = argv[0];

	while ((ch = getopt(argc, argv, "t:k:")) != -1) {
		switch (ch) {
		case 't':
			taskcnt = atoi(optarg);
			break;
		case 'k':
			threadcnt = atoi(optarg);
			break;
		case '?':
		default:
			printf("Usage: %s [-k threadcnt] [-t taskcnt]\n"
				"\tdefault threadcnt=3, taskcnt=10\n", prog);
			exit(EXIT_FAILURE);
		}
	}

	threadpool_t pool;
	threadpool_init(&pool, threadcnt);

	int i;
	for (i = 0; i < taskcnt; ++i) {
		void *arg = malloc(sizeof(int));
		memcpy(arg, &i, sizeof(int));
		threadpool_add_task(&pool, task_routine, arg);
	}

	threadpool_destroy(&pool);

	return 0;
}

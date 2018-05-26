
#include <unistd.h>
#include <errno.h>

#include "threadpool.h"
#include "pool_util.h"

void* thread_routine(void* arg)
{
	pthread_t tid = pthread_self();
	threadpool_t *pool = (threadpool_t*)arg;
	int timedout;

	printf("Thread %#lx starting\n", (size_t)tid);

	while (1) {
		timedout = 0;
		cond_lock(&pool->ready);

		pool->idle++;
		//waiting for new task or the destroy of thread pool
		while ((NULL == pool->first) && (0 == pool->quit)) {
			//while((NULL==pool->first)) {
			printf("Thread %#lx waiting\n", (size_t)tid);
			//blocked wait
			//cond_wait(&pool->ready);

			//impletement timedout wait
			struct timeval tv;
			struct timespec ts;
			gettimeofday(&tv, NULL);
			ts.tv_sec = tv.tv_sec + 2;
			ts.tv_nsec = 0;
			int ret = cond_timedwait(&pool->ready, &ts);
			if (ETIMEDOUT == ret) {
				printf("Thread %#lx waiting timedout\n", (size_t)tid);
				timedout = 1;
				break;
			}
		}
		pool->idle--;

		// new task
		if (pool->first) {
			// extract a task from the head of the queue
			task_t *tk = pool->first;
			pool->first = tk->next;

			//It takes some time to excute task, unlock first to permit
			//other producers to add task, and other consumers to enter the loop
			cond_unlock(&pool->ready);
			//execute task
			tk->run(tk->arg);
			free(tk);
			cond_lock(&pool->ready);
		}

		// the destroy of thread pool
		if (pool->quit && NULL==pool->first) {
			pool->threadcnt--;
			if(0 == pool->threadcnt)
			cond_signal(&pool->ready);
			cond_unlock(&pool->ready);//do not forget unlock when breaking out the loop
			break;
		}

		// wait timedout
		if (timedout && NULL==pool->first) {
			pool->threadcnt--;
			cond_unlock(&pool->ready);//do not forget unlock when breaking out the loop
			break;
		}
		cond_unlock(&pool->ready);
	}
	printf("Thread %#lx exiting\n", (size_t)tid);

	return NULL;
}

//initialize thread pool
void threadpool_init(threadpool_t* pool, int max_threads)
{
	cond_init(&pool->ready);
	pool->first = pool->last = NULL;
	pool->threadcnt = pool->idle = 0;
	pool->max_threads = max_threads;
	pool->quit = 0;
}

//deallocate thread pool
void threadpool_destroy(threadpool_t *pool)
{
	if (pool->quit) {
		return;
	}

	cond_lock(&pool->ready);

	pool->quit = 1;
	if (pool->threadcnt) {
		//the working thread cannot receive the broadcast notification
		if (pool->idle)
			cond_broadcast(&pool->ready);

		while (pool->threadcnt) {
			//printf("Waiting thread(s) to exit\n");
			cond_wait(&pool->ready);
		}
	}

	cond_unlock(&pool->ready);
	cond_destroy(&pool->ready);
}

// add a task to thread pool
void threadpool_add_task(threadpool_t *pool, TASK_ROUTINE mytask, TASK_PARA_TYPE arg)
{
	task_t* newtask = (task_t*)malloc(sizeof(task_t));
	newtask->run = mytask;
	newtask->arg = arg;
	newtask->next = NULL;

	cond_lock(&pool->ready);

	// insert newtask at the end of the queue
	if (pool->first) {
		pool->last->next = newtask;
	} else {
		pool->first = newtask;
	}
	pool->last = newtask;

	// notify waiting threads
	if (pool->idle > 0) {
		cond_signal(&pool->ready);
	} else if (pool->threadcnt < pool->max_threads) { //add new thread if not reaching limit
		pthread_t tid;
		int ret;
		if ((ret = pthread_create(&tid, NULL, thread_routine, (void*)pool))) {
			ERROR("pthread_create", ret);
		}
		pool->threadcnt++;
	}

	cond_unlock(&pool->ready);
}


#ifndef __PTHREAD_POOL_H__
#define __PTHREAD_POOL_H__
//#pragma once

#include "condition.h"

typedef void* (*TASK_ROUTINE) (void*);
typedef void* TASK_PARA_TYPE;

typedef struct task {
	TASK_ROUTINE run; // task handler
	TASK_PARA_TYPE arg; //para for handler "run"
	struct task* next; // pointer to the next task
}task_t;

typedef struct threadpool {
	cond_t ready; // mutex and condition variable for thread pool
	task_t *first;  // pointer to the first task in the thread pool
	task_t *last; // point to the last past one task in the thread pool
	int threadcnt; // thread count at the present
	int idle; //idle thread count at the present
	int max_threads; // max threads for thread pool
	int quit; // set 1 when destroying thread pool
}threadpool_t;

//initialize thread pool
void threadpool_init(threadpool_t* pool, int max_threads);

//deallocate thread pool
void threadpool_destroy(threadpool_t *pool);

// add a task to thread pool
void threadpool_add_task(threadpool_t *poo,
	TASK_ROUTINE mytask, TASK_PARA_TYPE arg);

#endif

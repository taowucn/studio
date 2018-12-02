
#ifndef __CONDITION_H__
#define __CONDITION_H__
//#pragma once

#include <sys/time.h>
#include <pthread.h>

typedef struct condition {
	pthread_mutex_t p_mutex; //mutex
	pthread_cond_t p_cond; //condition variable
}cond_t;

int cond_init(cond_t* cond); //initial Mutex and Condition variable
int cond_destroy(cond_t* cond); // deallocate Mutex and Condition variable
int cond_lock(cond_t* cond); // acquire Mutex lock
int cond_unlock(cond_t* cond); //// release Mutex lock
int cond_wait(cond_t* cond); // wait for a condition
int cond_timedwait(cond_t* cond, const struct timespec *tv); // wait for a condition for a specified time
int cond_signal(cond_t* cond); // send  signal to a waiting thread
int cond_broadcast(cond_t* cond); // send  signal to all waiting thread

#endif

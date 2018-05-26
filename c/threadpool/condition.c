
#include "condition.h"
#include "pool_util.h"

int cond_init(cond_t* cond)
{
	int ret;

	ret = pthread_mutex_init(&cond->p_mutex, NULL);
	if (ret) {
		ERROR("pthread_mutex_init", ret);
	}

	ret = pthread_cond_init(&cond->p_cond, NULL);
	if (ret) {
		ERROR("pthread_cond_init", ret);
	}

	return 0;
}

int cond_destroy(cond_t* cond)
{
	int ret;

	ret = pthread_cond_destroy(&cond->p_cond);
	if (ret) {
		ERROR("pthread_cond_destroy", ret);
	}

	ret = pthread_mutex_destroy(&cond->p_mutex);
	if (ret) {
		ERROR("pthread_mutex_destroy", ret);
	}

	return 0;
}

int cond_lock(cond_t* cond)
{
	return pthread_mutex_lock(&cond->p_mutex);
}

int cond_unlock(cond_t* cond)
{
	return pthread_mutex_unlock(&cond->p_mutex);
}

int cond_wait(cond_t* cond)
{
	return pthread_cond_wait(&cond->p_cond, &cond->p_mutex);
}

int cond_timedwait(cond_t* cond, const struct timespec *ts)
{
	return pthread_cond_timedwait(&cond->p_cond, &cond->p_mutex, ts);
}

int cond_signal(cond_t* cond)
{
	return pthread_cond_signal(&cond->p_cond);
}

int cond_broadcast(cond_t* cond)
{
	return pthread_cond_broadcast(&cond->p_cond);
}


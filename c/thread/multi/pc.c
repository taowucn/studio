#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 8

struct Products
{
	int buffer[BUFFER_SIZE];
	pthread_mutex_t locker;
	pthread_cond_t notEmpty;
	pthread_cond_t notFull;
	int posReadFrom;
	int posWriteTo;
};

int BufferIsFull(struct Products* products)
{
	if((products->posWriteTo + 1) % BUFFER_SIZE == products->posReadFrom) {
		return 1;
	}
	return 0;
}

int BufferIsEmpty(struct Products *products)
{
	if (products->posWriteTo == products->posReadFrom) {
		return 1;
	}
	return 0;
}

void Produce(struct Products *products, int item)
{
	pthread_mutex_lock(&products->locker);

	while (BufferIsFull(products)) {
		pthread_cond_wait(&products->notFull, &products->locker);
	}

	products->buffer[products->posWriteTo] = item;
	products->posWriteTo++;

	if (products->posWriteTo >= BUFFER_SIZE)
		products->posWriteTo = 0;

	pthread_cond_signal(&products->notEmpty);
	pthread_mutex_unlock(&products->locker);
}

int Consume(struct Products *products)
{
	int item;

	pthread_mutex_lock(&products->locker);

	while (BufferIsEmpty(products)) {
		pthread_cond_wait(&products->notEmpty, &products->locker);
	}

	item = products->buffer[products->posReadFrom];
	products->posReadFrom++;

	if (products->posReadFrom >= BUFFER_SIZE)
		products->posReadFrom = 0;

	pthread_cond_signal(&products->notFull);
	pthread_mutex_unlock(&products->locker);

	return item;
}

#define END_FLAG (-1)
struct Products products;

void *ProducerThread(void *data)
{
	int i;
	for (i = 0; i < 16; ++i) {
		printf("producer: %d\n", i);
		Produce(&products, i);
	}
	Produce(&products, END_FLAG);
}

void *ConsumerThread(void *data)
{
	int item;

	while (1) {
		item = Consume(&products);
		if (END_FLAG == item) {
			printf("End Flag\n");
			break;
		}
		printf("consumer: %d\n", item);
	}
}

int main(int argc, char *argv[])
{
	pthread_t producer;
	pthread_t consumer;

	pthread_create(&producer, NULL, &ProducerThread, NULL);
	pthread_create(&consumer, NULL, &ConsumerThread, NULL);

	pthread_join(consumer, NULL);
	pthread_join(producer, NULL);

	return 0;
}

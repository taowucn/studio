#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
void thread1()
{
	int i;
	for(i=0;i<6;i++)
	{
		printf("thread1 is running\n");
		if(i==2)
			pthread_exit(0);
		sleep(1);
	}
}
void thread2()
{
	int i;
	for(i=0;i<3;i++)
		printf("thread2 is running\n");
	pthread_exit(0);
}

int main()
{
	pthread_t id1,id2;
	if(pthread_create(&id1,NULL,(void *)thread1,NULL)!=0)
	{
		perror("pthread1_create");
		exit(1);
	}
	if(pthread_create(&id2,NULL,(void *)thread2,NULL)!=0)
	{
		perror("pthread2_create");
		exit(1);
	}
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	exit(0);

}


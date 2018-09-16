#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>

#define PAGE_SIZE (4096)

void read_mem(void *virt)
{
	char *p = NULL;
	int i = 0;

	printf("Pid: %u, read mem virt: %p\n", getpid(), virt);
	p = (char *)virt;
	for (i = 0; i < 8; i++) {
		printf("%d, ", *p++);
	}
	printf("\n");
}

void *thread_handle(void *arg)
{
	int i = 0;

	for (i = 0; i < 2; i++) {
		read_mem(arg);
		sleep(1);
	}
}

int main(int argc , char *argv[])
{
	void *virt = NULL;
	int fd = 0, rval = 0;

	fd = open("/dev/mymap", O_RDWR);
	if (fd < 0) {
		perror("open fail");
		return -1;
	}

	do {
		virt = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (virt == MAP_FAILED) {
			printf("mmap fail");
			rval = -1;
			break;
		} else {
			printf("Pid: %u, mmap virt: %p\n", getpid(), virt);
			read_mem(virt);
		}
	} while (0);

#if 0
	pid_t pid;

	pid = fork();
	if (pid < 0) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		printf("Process children: %d\n", getpid());
		read_mem(virt);
	} else {
		printf("Process parent: %d\n", getpid());
		read_mem(virt);
	}
#else
	pthread_t tid;
	if (pthread_create(&tid, NULL, thread_handle, virt)) {
		perror("pthread_create");
		return -1;
	}
	if (pthread_join(tid, NULL)) {
		perror("pthread_join");
		return -1;
	}
#endif

	printf("Process end pid: %d\n", getpid());
#if 0
	if (virt != NULL) {
		munmap(virt, PAGE_SIZE);
		virt = NULL;
	}

	if (fd >= 0) {
		close(fd);
		fd = -1;
	}
#endif

	return rval;
}

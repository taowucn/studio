#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>

int main(int argc, char *argv[])
{
    int semid = 0;
    key_t semkey;

    semkey = ftok("/etc/profile", 1);
    if (semkey < 0) {
        perror("ftok");
        return -1;
    }

    semid = semget(semkey, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget");
        return -1;
    }
    printf("semid = %d\n", semid);

    return 0;
}

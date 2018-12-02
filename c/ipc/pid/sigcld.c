#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void handle_sigcld(int signo)
{
    pid_t pid;
    int status;
    if((pid = wait(&status)) != -1) {
        printf("children %d exit\n",pid);
    }
    if(WIFEXITED(status)) {
        printf("children exit with %d\n", WEXITSTATUS(status));
    }
    if(WIFSIGNALED(status)) {
        printf("children was killed by %d\n", WTERMSIG(status));
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    signal(SIGCLD,handle_sigcld);
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(0);
    }
    if (pid == 0) {
       exit(123);
    }
    sleep(5);
}
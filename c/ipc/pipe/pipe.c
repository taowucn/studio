#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
    int pfd[2];	//����򿪹ܵ���������ļ�������
    pid_t cpid;//������̱�ʶ��
    char buf;
    if(argc != 2)//�ж������в����Ƿ����
    {
        fprintf(stderr,"Usage: %s <string>\n",argv[0]);
        exit(0);
    }
    if (pipe(pfd) == -1)//�����ܵ�
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)//�ӽ���
    {
        close(pfd[1]);          //�رչܵ�д�����ü���-1
        printf("child process...\n");
        while (read(pfd[0], &buf, 1) > 0) {//�ӹܵ�ѭ����ȡ����
            write(STDOUT_FILENO, &buf, 1);//�������������
        }
        write(STDOUT_FILENO, "\n", 1);//����ӹܵ���ȡ������
        close(pfd[0]);//�رչܵ��������ü���-1
        exit(0);
    }
    else
    {//������
        close(pfd[0]);
        printf("father process...\n");
        write(pfd[1], argv[1], strlen(argv[1]));//��ܵ�д�������в���1
        close(pfd[1]);
        wait(NULL);           //�ȴ��ӽ����˳�
        exit(0);
    }
}
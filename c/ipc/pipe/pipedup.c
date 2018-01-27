#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
int pfds[2];
if ( pipe(pfds) == 0 )
{
    if ( fork() == 0 )//�ӽ���
    {
        printf("child...\n");
        close(1);//�رձ�׼���
        dup2( pfds[1], 1 );//�ܵ���д�ļ����������Ƶ����̵����
        close( pfds[0] );//�رչܵ���
        printf("child...\n");
        execlp( "ls", "ls","-l", NULL );//ִ��ls -l ���д��ܵ�
    }
    else
    {
        close(0);
        dup2( pfds[0], 0 );//�ܵ��Ķ��ļ����������Ƶ����̵�����
        close( pfds[1] );
        printf("parent...\n");
        execlp( "wc", "wc", "-l", NULL );//ִ��wc -l ���ܵ���ȡ������Ϊwc���������
     }
}
return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main( void )
{
    FILE   *stream;//�ļ���
    char   buf[1024];//��д������
    memset( buf, '\0', sizeof(buf) );//���
    stream = popen( "wc -l", "w" );
    for(;;)
    {
        memset(buf,0x00,sizeof(buf));
        scanf("%s",buf);//��������
        if(strcmp(buf,"q") == 0)//�����q���˳�
        {
            break;
        }
        fprintf(stream,"%s\n",buf);//д��
    }
    printf("exit\n");
    pclose( stream );//�ر�
    return 0;
}
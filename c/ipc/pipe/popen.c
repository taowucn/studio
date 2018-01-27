#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main( void )
{
    FILE   *stream;//文件流
    char   buf[1024];//读写缓冲区
    memset( buf, '\0', sizeof(buf) );//清空
    stream = popen( "wc -l", "w" );
    for(;;)
    {
        memset(buf,0x00,sizeof(buf));
        scanf("%s",buf);//接受输入
        if(strcmp(buf,"q") == 0)//如果是q就退出
        {
            break;
        }
        fprintf(stream,"%s\n",buf);//写入
    }
    printf("exit\n");
    pclose( stream );//关闭
    return 0;
}
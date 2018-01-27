#include <stdio.h>
#include <malloc.h>
#define MAX 10000
void malloc_array(void)
{
    int *a[MAX];
    int i;
    for( i=0; i<MAX; i++ ) {
        a[i] = (int *)malloc( MAX );
	if (a[i] == NULL) {
	  printf("null\n");
	}
    }
}

void malloc_char(void)
{
    char *a; //声明一个指向char的指针
    a=(char *)malloc(100*sizeof(char)); //使用malloc分配内存的首地址，然后赋值给a
    gets(a); //从用户那里得到数据，把数据放入a中
    printf("%s\n",a); //输出用户输入的数据
    free(a); //释放掉使用的内存地址，就是因为可以释放内存地址才被称为动态内存分配
}

int main(int argc, char ** argv)
{
  malloc_array();
  malloc_char();
  return 0;
}

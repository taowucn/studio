#include <stdlib.h>
#include <stdio.h>

using namespace std;
int fun(void){
	static int count = 10; //��ʵ�ϴ˸�ֵ������û��ִ�й�
	//int count = 5; //��ʵ�ϴ˸�ֵ������û��ִ�й�
	return count--;
}
int count = 1;
int main(void)
{
	printf("global\t\tlocal static\n");
	for(; count <= 10; ++count)
	printf("%d\t\t%d\n", count, fun());
	return 0;
}
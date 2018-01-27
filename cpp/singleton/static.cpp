#include <stdlib.h>
#include <stdio.h>

using namespace std;
int fun(void){
	static int count = 10; //事实上此赋值语句从来没有执行过
	//int count = 5; //事实上此赋值语句从来没有执行过
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
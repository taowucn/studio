#include <stdio.h>

int main(int argc, char **argv)
{
	int pass=123;
	int enter;

	printf("please enter the password:\n");
	scanf("%d",&enter);
	if (enter==pass){
		printf("right\n");
	} else {
		printf("wrong\n");
	}

	return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
	char *ptr = "test";

	printf("Before ...\n");
	strcpy(ptr, "TEST");
	printf("After ...\n");

	return 0;
}

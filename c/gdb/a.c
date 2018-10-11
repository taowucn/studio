#include <stdio.h>

static int func(int n)
{
	int sum = 0, i = 0;

	for (i = 0; i < n; i++) {
		sum += i;
	}

	return sum;
}

int main(int argc, char **argv)
{
	int i = 0;
	int result = 0;
	int *p = (int *)0x1;

	for (i = 1; i <= 100; i++) {
		result += i;
	}

	printf("result[1-100] = %d \n", result);
	printf("result[1-250] = %d \n", func(250));
	printf("p = %d\n", *p);

	return 0;
}

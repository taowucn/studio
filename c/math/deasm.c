#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
	double a, b = 3.14f;
	a = exp(b);

	printf("exp=%lf\n", a);
	return 0;
}

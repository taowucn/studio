#include <stdio.h>
#include <math.h>

int main (int argc, char **argv)
{
	double x = 1.0;
	double y = 1.1;
	double m = 0.0;
	double n = 0.0;
	int i = 1;

	if (x > y) {
		printf("x is bigger\n");
	} else if (x < y) {
		printf("y is bigger\n");
	} else {
		printf("x,y is equal\n");
	}
	if (i == x ) {
		printf("i, x is equal\n");
	}
	m = y + 1e-6;
	n = y - 1e-6;
	printf("m = %f, n = %f\n", m, n);

	y = atan(x);
	printf("y = %f\n", y);

	return 0;
}

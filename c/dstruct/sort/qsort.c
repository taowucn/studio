#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int cmp(const void *p1, const void *p2)
{
	return (*(int *)p1 - * (int*)p2);
}

static int cmp_r(const void *p1, const void *p2, void *arg)
{
	int l_idx = *(const int *)p1;
	int r_idx = *(const int *)p2;
	int *d = (int *)arg;

	int l = (int)d[l_idx];
	int r = (int)d[r_idx];

	return (l - r);
}

int main(int argc, char *argv[])
{
	int j = 0, len = 0;
	int values[8] = {1, 9, 3, 4, 8, 7, 7, 2};
	int values_arg[8];
	int values_idx[8];

	len = sizeof(values) / sizeof(values[0]);
	printf("Len: %d\n", len);

	printf("sort before: ");
	for (j = 0; j < len; j++) {
		printf("%d, ", values[j]);
	}
	printf("\n");

	/* qsort */
	qsort(values, len, sizeof(int), cmp);

	printf("qort after: ");
	for (j = 0; j < len; j++) {
		printf("%d, ", values[j]);
	}
	printf("\n");

	/* qsrot_r */
	memcpy(values_arg, values, sizeof(values_arg));
	for (j = 0; j < len; j++) {
		values_idx[j] = j;
	}
	qsort_r(values_idx, len, sizeof(int), cmp_r, values_arg);
	printf("qort_r after: ");
	for (j = 0; j < len; j++) {
		printf("%d, ", values_arg[j]);
	}
	printf("\n");

	return 0;
}

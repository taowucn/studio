#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

void bubble_sort(uint32_t *list, uint32_t num)
{
	uint32_t i = 0, j = 0, t = 0;
	uint32_t n = num - 1;

	for (i = 0; i < n; i++) {
		for (j = 0; j < (n - i); j++) {
			if (list[j] > list[j + 1]) {
				t = list[j + 1];
				list[j + 1] = list[j];
				list[j] = t;
			}
		}
	}
}

void print_list(uint32_t *list, uint32_t num)
{
	uint32_t i = 0;

	for (i = 0; i < num; i++) {
		printf("%08u: %u\n", i, list[i]);
	}
}

int main(int argc, char **argv)
{
#define MAX (100)

	uint32_t list[MAX] = {0};
	uint32_t i = 0;

	for (i = 0; i < MAX; i++) {
		srand((unsigned int)time(NULL));
		list[i] = rand();
	}

	bubble_sort(list, MAX);
	print_list(list, MAX);

	return 0;
}

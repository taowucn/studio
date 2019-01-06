#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int dichotomy(uint32_t *list, uint32_t key, uint32_t num)
{
	uint32_t low = 0;
	uint32_t high = num - 1;
	uint32_t mid = (low + high)/2;

	printf("low:%d, mid:%d, high:%d\n", low, mid, high);
	while (high >= low) {
		if (key > list[mid]) {
			low = mid + 1;
			mid = (low + high) / 2;
		} else if (key < list[mid]) {
			high = mid - 1;
			mid = (low + high) / 2;
		} else if (key == list[mid]) {
			return (mid + 1);
		}
		//printf(".");
	}

	return -1;
}

int main(int argc, char **argv)
{
#define MAX 10

	uint32_t list[MAX] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	uint32_t key = 0, pos = 0;

	key = 3;
	printf("Finding number: %u\n", key);

	pos = dichotomy(list , key, MAX);
	if (pos < 0) {
		printf("Can't find number.\n");
	} else {
		printf("Found key postion: %u\n", pos);
	}

	return 0;
}

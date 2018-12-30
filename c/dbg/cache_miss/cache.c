#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000
#define CACHE_GOOD (0)

int main(int argc, char **argv)
{
    int array[SIZE][SIZE] = {0};
    int i,j;

#if CACHE_GOOD
    for (i = 0; i < SIZE; ++i) {
        for (j = 0; j < SIZE; ++j) {
            array[i][j] = i + j;
        }
    }
#else
    for (j = 0; j < SIZE; ++j) {
        for (i = 0; i < SIZE; ++i) {
            array[i][j] = i + j;
        }
    }
#endif
	printf("array[0]: %d\n", array[0][0]);

    return 0;
}

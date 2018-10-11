#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX 10000

void malloc_array(void)
{
    int *a[MAX];
    int i = 0;

    for (i = 0; i < MAX; i++) {
        a[i] = (int *)malloc(MAX);
		if (a[i] == NULL) {
		  printf("null\n");
		}
    }
}

void malloc_char(void)
{
	char *a = NULL;
	char *ret = NULL;

	a = (char *)malloc(32 * sizeof(char));
	ret = gets(a);
	if (ret == NULL) {
		perror("get");
	}

	printf("%s\n", a);
	free(a);
}

int main(int argc, char **argv)
{
	malloc_array();
	malloc_char();

	return 0;
}

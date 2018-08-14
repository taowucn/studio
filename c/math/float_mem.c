#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char **argv)
{
	float f0 = 3.75f;
	float f1 = 12345.0f;
	FILE *file;
	uint8_t *c = (uint8_t *) &f0;
	int i = 0;
	
	file = fopen("/tmp/f_mem.bin", "wb");
	fwrite(&f0, sizeof(f0), 1, file);
	fwrite(&f1, sizeof(f1), 1, file);
	fclose(file);
	printf("Write f0=%f, f1=%f\n", f0, f1);

	printf("f0: ");
	for (i = 3; i >= 0; i--) {
		printf("%02x ", c[i]);
	}
	printf("\n");

	c = (uint8_t *)&f1;
	printf("f1: ");
	for (i = 3; i >= 0; i--) {
		printf("%02x ", c[i]);
	}
	printf("\n");
	
	return 0;
}

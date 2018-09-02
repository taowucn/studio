#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include "md5.h"

static uint32_t get_file_size(const char *path)
{
	struct stat statbuff;

	if (stat(path, &statbuff) < 0) {
		return 0;
	} else {
		return statbuff.st_size;
	}
}

static void md5sum(uint8_t *in, uint8_t *out, uint32_t size)
{
	MD5_CTX md5;

	MD5Init(&md5);
	MD5Update(&md5, in, size);
	MD5Final(&md5, out);
}

int main(int argc, char *argv[])
{
	//unsigned char encrypt[] = "admin"; //21232f297a57a5a743894a0e4a801fc3
	uint8_t *in = NULL;
	uint8_t decrypt[16] = {0};
	char *filename = NULL;
	FILE *fp = NULL;
	uint32_t in_sz = 0;
	int rval = 0, i = 0;

	if (argc < 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}
	filename = argv[1];
	printf("filename: %s\n", filename);

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		perror("fopen");
		return -1;
	}

	do {
		in_sz = get_file_size(filename);
		in = malloc(in_sz);
		if (in == NULL) {
			perror("malloc");
			return -1;
		}
		if (fread(in, 1, in_sz, fp) != in_sz) {
			perror("fread");
			rval = -1;
			break;
		}

		md5sum(in, decrypt, 5);
		printf("md5: ");
		for (i = 0; i < 16; i++) {
			printf("%02x", decrypt[i]);
		}
		printf("\n");
	} while (0);

	if (in) {
		free(in);
		in = NULL;
	}
	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	return 0;
}

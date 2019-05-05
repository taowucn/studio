#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include "half.hpp"

using namespace half_float;

static uint32_t get_file_size(const char *path)
{
	struct stat statbuff;

	if (stat(path, &statbuff) < 0) {
		return 0;
	} else {
		return statbuff.st_size;
	}
}

int main(int argc, char *argv[])
{
	FILE *in_fp = NULL;
	char *in_file = NULL;
	char *fmt = NULL;

	uint32_t in_size = 0, is_fp16 = 0;
	uint32_t i = 0, j = 0, row_max = 0;
	int rval = 0;

	if (argc != 4) {
		printf("Usage: %s <input filename> <fp32|fp16> <Show Num in Row>\n", argv[0]);
		printf("\t View data with fp32 or fp16 format\n");
		return -1;
	}
	in_file = argv[1];
	fmt = argv[2];
	row_max = atoi(argv[3]);

	in_size = get_file_size(in_file);
	if (in_size == 0) {
		perror("state");
		return -1;
	}

	if (strcmp(fmt, "fp16") == 0) {
		//printf("View with fp16\n");
		is_fp16 = 1;
	} else {
		//printf("View with fp32\n");
		is_fp16 = 0;
	}

	do {
		in_fp = fopen(in_file, "rb");
		if (in_fp == NULL) {
			perror("fopen");
			rval = -1;
			break;
		}

		if (is_fp16) {
			half in_data;
			while (in_size > 0) {
				if (fread(&in_data, 1, sizeof(in_data), in_fp) != sizeof(in_data)) {
					perror("fread");
					rval = -1;
					break;
				}
				in_size -= sizeof(in_data);
				if (j == 0) {
					printf("[%08d]: ", i++);
				}
				printf("%f, ", (float)in_data);
				if ((++j) == row_max) {
					printf("\n");
					j = 0;
				}
			}
		} else {
			float in_data;
			while (in_size > 0) {
				if (fread(&in_data, 1, sizeof(in_data), in_fp) != sizeof(in_data)) {
					perror("fread");
					rval = -1;
					break;
				}
				in_size -= sizeof(in_data);
				if (j == 0) {
					printf("[%08d]: ", i++);
				}
				printf("%f, ", (float)in_data);
				if ((++j) == row_max) {
					printf("\n");
					j = 0;
				}
			}
		}
	} while (0);
	printf("\n");

	if (in_fp != NULL) {
		fclose(in_fp);
		in_fp = NULL;
	}

	return rval;
}

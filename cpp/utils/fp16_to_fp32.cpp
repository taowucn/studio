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

#if 0
static void test_fp16_to_fp32()
{
	float out = 0.2f;
	half in;

	in = (half) out;
	printf("test_1: in: %f, %f\n", (float) in, out);

	out = (float) in;
	printf("test_2: in: %f, %f\n", (float) in, out);
}
#endif

int main(int argc, char *argv[])
{
	FILE *in_fp = NULL;
	FILE *out_fp = NULL;
	char *in_file = NULL;
	char *out_file = NULL;

	float out_data;
	half in_data;
	uint32_t in_size = 0;
	int rval = 0;

	//test_fp16_to_fp32();

	if (argc != 3) {
		printf("Usage: %s <Input filename> <Output filename> \n", argv[0]);
		return -1;
	}

	in_file = argv[1];
	out_file = argv[2];
	in_size = get_file_size(in_file);
	if (in_size == 0) {
		perror("state");
		return -1;
	}

	do {
		in_fp = fopen(in_file, "rb");
		if (in_fp == NULL) {
			perror("fopen");
			rval = -1;
			break;
		}

		out_fp = fopen(out_file, "w+b");
		if (out_fp == NULL) {
			perror("fopen");
			rval = -1;
			break;
		}
		while (in_size > 0) {
			if (fread(&in_data, 1, sizeof(half), in_fp) != sizeof(half)) {
				perror("fread");
				rval = -1;
				break;
			}

			/* Convert fp16 to fp32 */
			out_data = (float) in_data;

			if (fwrite(&out_data, 1, sizeof(float), out_fp) != sizeof(float)) {
				perror("fwrite");
				rval = -1;
				break;
			}
			//printf("in: %f, out:%f\n", (float)in_data, out_data);

			in_size -= sizeof(half);
		}
	}while (0);

	if (out_fp != NULL) {
		fclose(out_fp);
		out_fp = NULL;
	}
	if (in_fp != NULL) {
		fclose(in_fp);
		in_fp = NULL;
	}

	if (rval == 0) {
		printf("Write %s done\n", out_file);
	}

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/stat.h>

static uint32_t get_file_size(const char *path)
{
	struct stat statbuff;

	if (stat(path, &statbuff) < 0) {
		return 0;
	} else {
		return statbuff.st_size;
	}
}

static int check_file_size(uint32_t in_size, uint32_t algin_sz)
{
	if ((in_size % algin_sz) == 0) {
		return 0;
	} else {
		return -1;
	}
}

int main(int argc, char *argv[])
{
	struct stat in_state;
	FILE *in_fp = NULL;
	FILE *out_fp = NULL;
	char *in_file = NULL;
	char *out_file = NULL;
	char *data = NULL;
	uint32_t in_size = 0;
	uint32_t out_size = 0;
	uint32_t read_sz = 0;
	uint32_t skip_sz = 0;
	uint32_t algin_sz = 0;
	int rval = 0;

	if (argc != 5) {
		printf("Usage: %s <Input filename> <Output filename> <read size> <skip size>\n", argv[0]);
		return -1;
	}

	in_file = argv[1];
	out_file = argv[2];
	read_sz = atoi(argv[3]);
	skip_sz = atoi(argv[4]);
	algin_sz = read_sz + skip_sz;

	in_size = get_file_size(in_file);
	if (in_size == 0) {
		perror("state");
		return -1;
	}
	if (check_file_size(in_size, algin_sz) < 0) {
		printf("Check file size err: total size: %u, algin size: %u\n", in_size, algin_sz);
		return -1;
	}

	data = (char *)malloc(read_sz);
	if (data == NULL) {
		perror("malloc");
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
			if (fread(data, 1, read_sz, in_fp) != read_sz) {
				perror("fread");
				rval = -1;
				break;
			}
			if (fwrite(data, 1, read_sz, out_fp) != read_sz) {
				perror("fwrite");
				rval = -1;
				break;
			}
			if (fseek(in_fp, skip_sz, SEEK_CUR) < 0) {
				perror("fseek");
				rval = -1;
				break;
			}
			in_size -= algin_sz;
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
	if (data != NULL) {
		free(data);
		data = NULL;
	}

	if (rval == 0) {
		printf("Write %s done\n", out_file);
	}

	return 0;
}

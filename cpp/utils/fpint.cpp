#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "half.hpp"

using namespace half_float;

int main(int argc, char *argv[])
{
	char *fmt = NULL;
	char *data = NULL;
	int rval = 0;

	//test_fp16_to_fp32();
	if (argc != 3) {
		printf("Usage: %s <Input format> <Input data> \n", argv[0]);
		printf("Format: fp32, fp16, fx32, fx16\n");
		return -1;
	}
	fmt = argv[1];
	data = argv[2];

	if (!strcmp(fmt, "fp32")) {
		float in = atof(data);
		uint32_t *out = (uint32_t *)&in;
		printf("IN fp32: %f, OUT fx32: 0x%x \n", in, *out);
	} else if (!strcmp(fmt, "fp16")) {
		float in_tmp = atof(data);
		half in = (half)in_tmp;
		uint16_t *out = (uint16_t *)&in;
		printf("IN fp16: %f, OUT fx16: 0x%x \n", (float)in, *out);
	} else if (!strcmp(fmt, "fx32")) {
		uint32_t in = strtoul(data, 0, 0);
		float *out = (float *)&in;
		printf("IN fx32: 0x%x, OUT fp32: %f \n", in, *out);
	} else if (!strcmp(fmt, "fx16")) {
		uint32_t in_tmp = strtoul(data, 0, 0);
		uint16_t in = (uint16_t) in_tmp;
		half *out =  (half *)&in;
		printf("IN fx16: 0x%x, OUT fp16: %f\n", in, (float)*out);
	} else {
		printf("Invalid format: %s\n", fmt);
		rval = -1;
	}

	return rval;
}

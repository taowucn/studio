#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "half.hpp"

#define DEBUG_TIME (1)

#if DEBUG_TIME
#define MEASURE_TIME_VAR() \
	struct timeval tv1, tv2; \
	unsigned long tv_diff

#define MEASURE_TIME_START(s) do { \
	gettimeofday(&tv1, NULL); \
} while (0)

#define MEASURE_TIME_END(s) do { \
	gettimeofday(&tv2, NULL); \
	tv_diff = (unsigned long) 1000000 * (unsigned long) (tv2.tv_sec - tv1.tv_sec) + \
		(unsigned long) (tv2.tv_usec - tv1.tv_usec); \
	tv1 = tv2; \
	printf("%s : %lu us\n", s, tv_diff); \
} while (0)
#else
#define MEASURE_TIME_VAR()
#define MEASURE_TIME_START(s)
#define MEASURE_TIME_END(s)
#endif

#define HMAP_H (1 << 16)
#define HMAP_W (32)
#define HMAP_W_IDX_NUM (HMAP_W)

using namespace half_float;

struct hmap_row {
	//uint16_t num;
	uint16_t index[HMAP_W_IDX_NUM]; /* index rang: [0, 30] */
	//void *next;
};

static int cmp(const void *p1, const void *p2)
{
	half diff = (*(half *)p1 - *(half *)p2);
	int ret = 0;

	if (diff > 0) {
		ret = -1;
	} else if (diff == 0) {
		ret = 0;
	} else {
		ret = 1;
	}

	return ret;
}

static void init_hist_sort(uint16_t *hist_sort, uint32_t size)
{
	uint32_t j = 0;
	MEASURE_TIME_VAR();

	for (j = 0; j < size; j++) {
		hist_sort[j] = j;
	}

	MEASURE_TIME_START();
	qsort(hist_sort, size, sizeof(uint16_t), cmp);
	MEASURE_TIME_END("init_hist_sort");
}

static void build_hist_map(struct hmap_row *hist_map, uint16_t *hist_num,
	half *data, uint32_t data_num)
{
	uint16_t *p_u16 = NULL;
	uint32_t i = 0, j = 0, n = 0;
	MEASURE_TIME_VAR();

	MEASURE_TIME_START();
	memset((void *)hist_num, 0, HMAP_H );

	p_u16 = (uint16_t *)data;
	for (i = 0; i < data_num; i++) {
		j = p_u16[i];
		n = hist_num[j];

		if (n < HMAP_W_IDX_NUM) {
			hist_map[j].index[n] = i;
			hist_num[j]++;
			//printf("%u: num: %u, data_idx: %u, data: %f\n", j,
			//	hist_num[j], hist_map[j].index[n], (float)data[i]);
		} else {
			printf("hist_map column [%u] is full, data[%u]: %f\n", j, i, (float)data[i]);
			break;
		}
	}
	MEASURE_TIME_END("build_hist_map");
}

int main(int argc, char **argv)
{
#define NUM (4)

	float val_full[NUM] = {0.2, 2.5, 1.1, 1.1,};
	half data[NUM];

	uint16_t *p_u16 = NULL;
	uint32_t i = 0, j = 0, n = 0;
	struct hmap_row hist_map[HMAP_H] = {0};
	uint16_t hist_num[HMAP_H] = {0};
	uint16_t hist_sort[HMAP_H] = {0};

	int rval = 0;
	uint32_t data_num = sizeof(val_full)/sizeof(val_full[0]);

	printf("Bit16 max: %u\n", HMAP_H);
	printf("Load half ...\n");
	for (i = 0; i < data_num; i++) {
		data[i] = (half)val_full[i];
	}
	printf("half data: ");
	for (i = 0; i < data_num; i++) {
		printf("%f, ", (float)data[i]);
	}
	printf("\n");
	printf("uint16_t data: ");
	p_u16 = (uint16_t *)data;
	for (i = 0; i < data_num; i++) {
		printf("%u, ", p_u16[i]);
	}
	printf("\n");

	/* ----------*/

	init_hist_sort(hist_sort, HMAP_H);

	#if 0
	half *p_hf = (half *)hist_sort;
	for (j = 8000; j < 8003; j++) {
		printf("hist_sort %u: %u, %f\n", j, hist_sort[j], (float)p_hf[j]);
	}
	#endif

	build_hist_map(hist_map, hist_num, data, data_num);

	printf("\nshow histgram:\n");
	for (j = 0; j < HMAP_H; j++) {
		for (n = 0; n < hist_num[j]; n++) {
			printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, n, hist_num[j],
				hist_map[j].index[n], (float)data[hist_map[j].index[n]]);
		}
	}

	printf("\nshow order (decsend):\n");
	for (i = 0; i < HMAP_H; i++) {
		j = hist_sort[i];
		for (n = 0; n < hist_num[j]; n++) {
			printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, n, hist_num[j],
				hist_map[j].index[n], (float)data[hist_map[j].index[n]]);
		}
	}

	return rval;
}

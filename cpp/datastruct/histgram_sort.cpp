#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

//#include "list.h"

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

#define HMAP_H (1 << 16) /* element number */
#define HMAP_W (4) /* element number */
#define HMAP_W_INFO_NUM (0)
#define HMAP_W_NUM (HMAP_W - HMAP_W_INFO_NUM)

#define HMAP_W_EXT (12)
#define HMAP_W_EXT_INFO_NUM ((uint32_t)sizeof(void *))
#define HMAP_W_EXT_NUM (HMAP_W_EXT - HMAP_W_EXT_INFO_NUM)

using namespace half_float;

struct hmap_row {
	uint16_t index[HMAP_W_NUM];  /* index rang: [0, 8] */
};

struct hmap_row_ext {
	/* ext next size : 8; NUM: 8/2 = 4 */
	struct hmap_row_ext *next;

	uint16_t index[HMAP_W_EXT_NUM]; /* index rang: [0, 512] */
};

struct hmap_list_hdr {
	/* list header size :  8 x 2 + 2 + 2 + reserve(4) = 24 */
	struct hmap_row_ext *head;
	struct hmap_row_ext *curr;
	uint16_t row_idx;
	uint16_t column_num;
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

static void build_hist_map(struct hmap_row *hist_map,
	struct hmap_list_hdr *hist_list, uint16_t *hist_num,
	half *data, uint32_t data_num)
{
	uint16_t *p_u16 = NULL;
	uint32_t i = 0, j = 0, n = 0;
	MEASURE_TIME_VAR();

	MEASURE_TIME_START();
	memset((void *)hist_num, 0, HMAP_H * sizeof(uint16_t));
	for (i = 0; i < HMAP_H; i++) {
		if (hist_list[i].head) {
			hist_list[i].curr = hist_list[i].head;
			hist_list[i].row_idx = 0;
			hist_list[i].column_num = 0;
		}
	}

	p_u16 = (uint16_t *)data;
	for (i = 0; i < data_num; i++) {
		j = p_u16[i];
		n = hist_num[j];

		if (n < HMAP_W_NUM) {
			hist_map[j].index[n] = i;
			hist_num[j]++;
			//printf("%u: num: %u, data_idx: %u, data: %f\n", j,
			//	hist_num[j], hist_map[j].index[n], (float)data[i]);
		} else {
			/* this first column */
			if (hist_list[j].head == NULL) {
				void *p = malloc(sizeof(struct hmap_row_ext));
				if (!p) {
					perror("malloc err");
					break;
				}
				hist_list[j].head = (struct hmap_row_ext *)p;
				hist_list[j].curr = (struct hmap_row_ext *)p;
				hist_list[j].column_num++;
			}
			/* column is full */
			if (hist_list[j].row_idx == HMAP_W_EXT_NUM) {
				if (hist_list[j].curr->next == NULL) {
					/* end column is full */
					void *p = malloc(sizeof(struct hmap_row_ext));
					if (!p) {
						perror("malloc err");
						break;
					}
					hist_list[j].curr->next = (struct hmap_row_ext *)p;
				}
				/* use the next column */
				hist_list[j].curr = hist_list[j].curr->next;
				hist_list[j].column_num++;
				hist_list[j].row_idx = 0;
			}
			/* add data index */
			hist_list[j].curr->index[hist_list[j].row_idx] = i;
			hist_list[j].row_idx++;
			hist_num[j]++;
			//printf("Ext map list, %u: num: %u, data_idx: %u, data: %f\n", j,
			//	hist_num[j], i, (float)data[i]);
		}
	}
	MEASURE_TIME_END("\nbuild_hist_map");
}

int main(int argc, char **argv)
{
	float val_full[] = {0.8, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 2.5,};
	struct hmap_row hist_map[HMAP_H] = {0};
	struct hmap_list_hdr hist_list[HMAP_H] = {0};
	struct hmap_row_ext *node = NULL;
	struct hmap_row_ext *free_head = NULL;
	uint16_t hist_num[HMAP_H] = {0};
	uint16_t hist_sort[HMAP_H] = {0};

	uint16_t *p_u16 = NULL;
	uint32_t i = 0, j = 0, n = 0, k = 0;
	uint32_t remain_num = 0, iteration = 1;

	int rval = 0;

	uint32_t data_num = sizeof(val_full)/sizeof(val_full[0]);
	half data[data_num];

	if (argc == 2) {
		iteration = atoi(argv[1]);
		printf("Iteraction Num: %u\n", iteration);
	}
	printf("HMAP_H: %u, HMAP_W_NUM: %u, HMAP_W_EXT_NUM: %u, "
		"row num: %lu, data_num: %u\n",
		HMAP_H, HMAP_W_NUM, HMAP_W_EXT_NUM,
		sizeof(struct hmap_row)/sizeof(uint16_t), data_num);

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

	for (uint32_t cnt = 0; cnt < iteration; cnt++) {
		build_hist_map(hist_map, hist_list, hist_num, data, data_num);

		#if 0
		printf("\nshow histgram:\n");
		for (j = 0; j < HMAP_H; j++) {
			if (hist_num[j] <= HMAP_W_NUM) {
				for (n = 0; n < hist_num[j]; n++) {
					printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, n, hist_num[j],
						hist_map[j].index[n], (float)data[hist_map[j].index[n]]);
				}
			} else {
				for (n = 0; n < HMAP_W_NUM; n++) {
					printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, n, hist_num[j],
						hist_map[j].index[n], (float)data[hist_map[j].index[n]]);
				}
				node = hist_list[j].head;
				if (node == NULL) {
					printf("hist_list [%u] head is NULL\n", j);
					break;
				}

				remain_num = hist_num[j] - HMAP_W_NUM;
				for (n = 0, k = 0; n < remain_num; n++, k++) {
					if (k == HMAP_W_EXT_NUM) {
						node = node->next;
						k = 0;
					}
					if (node == NULL) {
						printf("node is NULL\n");
						break;
					}
					printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, k, hist_num[j],
						node->index[k], (float)data[node->index[k]]);
				}
			}
		}
		#endif

		printf("\nshow order (decsend):\n");
		for (i = 0; i < HMAP_H; i++) {
			j = hist_sort[i];
			if (hist_num[j] <= HMAP_W_NUM) {
				for (n = 0; n < hist_num[j]; n++) {
					printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, n, hist_num[j],
						hist_map[j].index[n], (float)data[hist_map[j].index[n]]);
				}
			} else {
				/* hist map */
				for (n = 0; n < HMAP_W_NUM; n++) {
					printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, n, hist_num[j],
						hist_map[j].index[n], (float)data[hist_map[j].index[n]]);
				}

				/* hist list */
				node = hist_list[j].head;
				if (node == NULL) {
					printf("hist_list [%u] head is NULL\n", j);
					break;
				}
				remain_num = hist_num[j] - HMAP_W_NUM;
				for (n = 0, k = 0; n < remain_num; n++, k++) {
					if (k == HMAP_W_EXT_NUM) {
						node = node->next;
						k = 0;
					}
					if (node == NULL) {
						printf("node is NULL\n");
						break;
					}
					printf("%u: num: %u/%u, data_idx: %u, data: %f\n", j, k, hist_num[j],
						node->index[k], (float)data[node->index[k]]);
				}
			}
		}
	}

	return rval;
}

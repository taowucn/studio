#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"

using namespace cv;
using namespace std;

#define USE_OPENCV_FUNC (1)
#define DEBUG_TIME (1)

#if DEBUG_TIME
#define MEASURE_TIME_VARIABLES()	\
	struct timeval tv1, tv2;	\
	unsigned long	us

#define MEASURE_TIME_START() gettimeofday(&tv1, NULL)

#define MEASURE_TIME_END(s)	\
	gettimeofday(&tv2, NULL);	\
	us = 1000000 * (tv2.tv_sec - tv1.tv_sec) + (tv2.tv_usec - tv1.tv_usec);	\
	tv1 = tv2; \
	printf("%s: %10lu us\n", s, us)
#else
#define MEASURE_TIME_VARIABLES()
#define MEASURE_TIME_START()
#define MEASURE_TIME_END(s)
#endif

typedef int8_t in_dt;
typedef int8_t diff_dt;

void do_left_sub_right(in_dt *left, in_dt *right, diff_dt *out, int width, int height)
{
	int size = width * height;
	int i = 0;

	for (i = 0; i < size; i++) {
		out[i] = left[i] - right[i];
	}
}

void do_integral_image(diff_dt *pixel, int *sum, int width, int height)
{
	diff_dt *in;
	int *out, *prev_out;
	int x, y;
	int dx, dy;
	int sat_1, sat_2, sat_3, dot;
	//int y_offset, dy_offset;

	in = pixel;
	out = sum;
	*sum = *pixel; //(0, 0)

	in = pixel + 1;
	out = sum + 1;
	prev_out = sum; // first row
	for (x = 1; x < width; x++, in++, out++, prev_out++) {
		*out = *prev_out + *in;
		//printf("[%d] %p, %p, %p, %p\n", x, pixel, sum, out, in);
	}

	in = pixel + width;
	out = sum + width;
	prev_out = sum; // first column
	for (y = 1; y < height; y++, in += width, out += width, prev_out += width) {
		*out = *prev_out + *in;
	}

	#if 1
	in = pixel;
	out = sum;
	for (y = 0, dy = 1; dy < height; y++, dy++) {
		//dy_offset = dy * width;
		//y_offset = y * width;
		for (x = 0, dx = 1; dx < width; x++, dx++) {
			sat_1 = out[dy * width + x]; //SAT(x-1, y)
			sat_2 = out[y * width + dx]; //SAT(x, y-1)
			sat_3 = out[y * width + x]; //SAT(x-1, y-1)
			dot = in[y * width + x]; //I(x, y)
			out[dy * width + dx] = sat_1 + sat_2 - sat_3 + dot;
		}
	}
	#endif
}

int store_file(const char *file_name, uint8_t *virt_addr, uint32_t size)
{
	FILE *out = NULL;
	int rval = 0;

	do {
		if ((out = fopen(file_name, "w+b")) == NULL) {
			perror(file_name);
			rval = -1;
			break;
		}
		if (fwrite(virt_addr, 1, size, out) != size) {
			perror(file_name);
			rval = -1;
			break;
		}
	} while (0);

	if (out) {
		fclose(out);
		out = NULL;
	}

	return rval;
}

void print_int32_hexdump(int *in, int width, int height)
{
	int x, y;

	printf("\n");
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			printf("%08d ", *in++);
		}
		printf("\n");
	}
	printf("\n");
}

#define MAXN (32)

int main(int argc, char *argv[])
{
	int width = 0, height = 0, size = 0;
	in_dt *left = NULL, *right = NULL;
	diff_dt *diff = NULL;
	int *sum = NULL;
	int verbose = 0;
	MEASURE_TIME_VARIABLES();

	if (argc != 3 && argc != 4) {
		printf("Usage: %s <width> <height> <is_verbose>\n", argv[0]);
		return -1;
	}
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	if (argc == 4) {
		verbose = atoi(argv[3]);
	}
	size = width * height;
	printf("[INFO] width: %d, height: %d, size: %d\n", width, height, size);

	left = (int8_t *)malloc(size * sizeof(in_dt));
	right = (int8_t *)malloc(size * sizeof(in_dt));
	diff = (int8_t *)malloc(size * sizeof(diff_dt));
	sum = (int *)malloc(size * sizeof(int));

	#if 0
	memset(left, 2, size);
	memset(right, 1, size);
	memset(diff, 0, size);
	memset(sum, 0, size * sizeof(int));
	#else
	for (int i = 0; i < size; i++) {
		left[i] = 2;
		right[i] = 1;
		diff[i] = 0;
		sum[i] = 0;
	}
	#endif

	MEASURE_TIME_START();
	for (int i = 0; i < MAXN; i++) {
		do_left_sub_right(left, right, diff, width, height);
	}
	MEASURE_TIME_END("left_sub_right");

	#if USE_OPENCV_FUNC
	Mat src(height, width, CV_8UC1, diff);
	Mat sum_mat;
	#endif

	for (int i = 0; i < MAXN; i++) {
		#if USE_OPENCV_FUNC
		integral(src, sum_mat, -1);
		#else
		do_integral_image(diff, sum, width, height);
		#endif
	}
	MEASURE_TIME_END("integral_image");

	store_file("out_diff.bin", (uint8_t *)diff, size * sizeof(diff_dt));
	#if USE_OPENCV_FUNC
	store_file("out_sum.bin", (uint8_t *)sum_mat.data, size * sizeof(int));
	#else
	store_file("out_sum.bin", (uint8_t *)sum, size * sizeof(int));
	#endif

	Mat result_img;
	#if USE_OPENCV_FUNC
	normalize(sum_mat, result_img, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
	#else
	Mat sum_mat(height, width, CV_32SC1, sum);
	normalize(sum_mat, result_img, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
	#endif
	imwrite("out_sum.jpg", result_img);

	if (verbose) {
		print_int32_hexdump(sum, width, height);
	}

	if (left) {
		free(left);
		left = NULL;
	}
	if (right) {
		free(right);
		right = NULL;
	}
	if (diff) {
		free(diff);
		diff = NULL;
	}
	if (sum) {
		free(sum);
		sum = NULL;
	}

	printf("Done\n");
	return 0;
}

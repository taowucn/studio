#include <opencv2/opencv.hpp>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

using namespace cv;
using namespace std;

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

void print_float32_hexdump(int *in, int width, int height)
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

int main(int argc, char *argv[])
{
	uchar a[5][5] = {
		{17 ,   24 ,    1  ,   8,    15},
		{23 ,    5  ,   7 ,   14  ,  16},
		{4 ,    6  ,  13 ,   20 ,   22},
		{10,    12 ,   19  ,  21,     3},
		{11 ,   18 ,   25 ,    2 ,    9},
	};

	Mat src(5, 5, CV_8UC1, &a );
	Mat dst;
	integral(src, dst, -1);

	cout << src << endl;
	cout << "====" << endl;

	cout <<"Height: " << dst.rows << ", Width: " << dst.cols << endl;
	cout << dst(Range(0, dst.rows), Range(0, dst.cols)) << endl;

	return 0;
}

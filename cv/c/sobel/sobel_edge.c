#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#define MASK_N 2
#define MASK_X 3
#define MASK_Y 3
#define WHITE 255
#define BLACK 0

struct bmp_file_header
{
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
} __attribute__((packed));

struct bmp_info_header
{
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPelsPerMerer;
	unsigned int biYPelsPerMerer;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
}__attribute__((packed));

// bitmap header
/*
unsigned char header[54] = {
	0x42,        // identity : B
	0x4d,        // identity : M
	0, 0, 0, 0,  // file size
	0, 0,        // reserved1
	0, 0,        // reserved2
	54, 0, 0, 0, // RGB data offset
	40, 0, 0, 0, // struct BITMAPINFOHEADER size
	0, 0, 0, 0,  // bmp width
	0, 0, 0, 0,  // bmp height
	1, 0,        // planes
	24, 0,       // bit per pixel
	0, 0, 0, 0,  // compression
	0, 0, 0, 0,  // data size
	0, 0, 0, 0,  // h resolution
	0, 0, 0, 0,  // v resolution
	0, 0, 0, 0,  // used colors
	0, 0, 0, 0   // important colors
};
*/

// sobel mask
static int mask[MASK_N][MASK_X][MASK_Y] = {
	{{-1, 0, 1},
	{-2, 0, 2},
	{-1, 0, 1}},

	{{-1,-2,-1},
	{0 , 0, 0},
	{1 , 2, 1}}
};

enum direction {
	DIR_X = 0,
	DIR_Y = 1,
	DIR_XY = 2,
};

void show_bmp_head(struct bmp_file_header *file, struct bmp_info_header *info)
{
	printf("Bmp File Header: Type=0x%x, Size=%d, OffBits=%d\n",
		file->bfType, file->bfSize, file->bfOffBits);
	printf("Bmp Info Header: Size=%d, WxH=%dx%d, Planes=%d, Bit=%d, Compr=%d, "
		"SizeImage=%d, X=%d, Y=%d, Clrused=%d, ClrImp=%d\n",
		info->biSize, info->biWidth, info->biHeight,
		info->biPlanes, info->biBitCount, info->biCompression,
		info->biSizeImage, info->biXPelsPerMerer, info->biYPelsPerMerer,
		info->biClrUsed, info->biClrImportant);
}

void read_bmp_head(FILE *fp, struct bmp_file_header *file, struct bmp_info_header *info)
{
	fseek(fp, 0, SEEK_SET);
	fread(file, sizeof(struct bmp_file_header), 1, fp);
	fread(info, sizeof(struct bmp_info_header), 1, fp);
}

void write_bmp_head(FILE *fp, struct bmp_file_header *file,
	struct bmp_info_header *info, unsigned char *in)
{
	unsigned int gap_size = 0;
	unsigned int offset = 0;

	fseek(fp, 0, SEEK_SET);
	fwrite(file, sizeof(struct bmp_file_header), 1, fp);
	fwrite(info, sizeof(struct bmp_info_header), 1, fp);

	/* 8Bit clut, not eixt in 24bit */
	offset = sizeof(struct bmp_file_header) + sizeof(struct bmp_info_header);
	gap_size = file->bfOffBits - offset;
	if (gap_size > 0) {
		fwrite(in + offset, gap_size, 1, fp);
	}
}

int read_bmp_raw(FILE *fp, unsigned char *in, unsigned int size)
{
	int ret = 0;

	ret = fread(in, size, 1, fp);
	if (ret != 1) {
		printf("Read bmp failed\n");
	}

	return 0;
}

int write_bmp_raw(FILE *fp, unsigned char *out, unsigned int size)
{
	int ret = 0;

	ret = fwrite(out, size, 1, fp);
	if (ret != 1) {
		printf("Write bmp raw data err\n");
	}

	return 0;
}

//convert RGB to gray level int
unsigned char color_to_int(unsigned char r, unsigned char g, unsigned char b)
{
	return (r + g + b) / 3;
}

int sobel_rgb(unsigned char *in, unsigned char *out, unsigned int width, unsigned int height,
	unsigned int byte_per_pixel, double threshold)
{
	unsigned int  x, y, i, v, u; // for loop counter
	unsigned char R, G, B; // color of R, G, B
	double val[MASK_N] = {0.0};
	int adjustX, adjustY, xBound, yBound;
	double total;
	unsigned char *base;

	adjustX = (MASK_X % 2) ? 1 : 0;
	adjustY = (MASK_Y % 2) ? 1 : 0;
	xBound = MASK_X / 2;
	yBound = MASK_Y / 2;
	//printf("Adjust: x=%u, y=%u; Bound: x=%u, y=%u\n", adjustX, adjustY, xBound, yBound);

	for (y = 0; y != height; ++y) {
		for (x = 0; x != width; ++x) {
			for (i = 0; i != MASK_N; ++i) {
				val[i] = 0.0;
				for (v = -yBound; v != yBound + adjustY; ++v) {
					for (u = -xBound; u != xBound + adjustX; ++u) {
						if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
							base = in + byte_per_pixel * (width * (y+v) + (x+u));
							R = *(base + 2);
							G = *(base + 1);
							B = *(base + 0);
							val[i] += color_to_int(R, G, B) * mask[i][u + xBound][v + yBound];
						}
					}
				}
			}

			total = 0.0;
			for (i = 0; i != MASK_N; ++i) {
				total += val[i] * val[i];
			}
			total = sqrt(total);

			base = out + byte_per_pixel * (width * y + x);
			if (total - threshold >= 0) {
				// black
				*(base + 2) = BLACK;
				*(base + 1) = BLACK;
				*(base + 0) = BLACK;
			} else {
				// white
				*(base + 2) = WHITE;
				*(base + 1) = WHITE;
				*(base + 0) = WHITE;
			}
		}
	}

	return 0;
}

int sobel_yuv(unsigned char *in, unsigned char *out, unsigned int width, unsigned int height,
	unsigned int byte_per_pixel, enum direction dir)
{
	unsigned int  x, y, i, v, u; // for loop counter
	unsigned char luma; // color of R, G, B
	unsigned char val[MASK_N] = {0};
	int adjustX, adjustY, xBound, yBound;
	unsigned int total;
	unsigned char *base;

	adjustX = (MASK_X % 2) ? 1 : 0;
	adjustY = (MASK_Y % 2) ? 1 : 0;
	xBound = MASK_X / 2;
	yBound = MASK_Y / 2;
	//printf("Adjust: x=%u, y=%u; Bound: x=%u, y=%u\n", adjustX, adjustY, xBound, yBound);

	for (y = 0; y != height; ++y) {
		for (x = 0; x != width; ++x) {
			for (i = 0; i != MASK_N; ++i) {
				val[i] = 0.0;
				for (v = -yBound; v != yBound + adjustY; ++v) {
					for (u = -xBound; u != xBound + adjustX; ++u) {
						if (x + u >= 0 && x + u < width && y + v >= 0 && y + v < height) {
							base = in + byte_per_pixel * (width * (y+v) + (x+u));
							luma = *(base);
							val[i] += luma * mask[i][u + xBound][v + yBound];
						}
					}
				}
			}

			total = 0;
			if (dir == DIR_XY) {
				for (i = 0; i != MASK_N; ++i) {
					total += val[i] * val[i];
				}
				total = sqrt(total);
			} else if (dir == DIR_X) {
				total = val[0];
			} else if (dir == DIR_Y) {
				total = val[1];
			} else {
				printf("Unsupport direction\n");
				return -1;
			}

			base = out + byte_per_pixel * (width * y + x);
			*(base) = (unsigned char)total;
		}
	}

	return 0;
}

int main(int argc, char *argv[])
{
	double threshold = 60.00f;
	char *inname = NULL;
	char *outname = NULL;
	FILE *fp_s = NULL;			// source file handler
	FILE *fp_t = NULL;			// target file handler
	unsigned char *image_s = NULL; // source image array
	unsigned char *image_t = NULL; // target image array
	unsigned int raw_size = 0;
	struct bmp_file_header file;
	struct bmp_info_header info;
	int is_bmp = 0;
	unsigned int width = 0;
	unsigned int height = 0;
	enum direction dir = DIR_XY;

	if (argc == 3) {
		inname = argv[1];
		threshold = atof(argv[2]);
		outname = "out_edge.bmp";
	} else if (argc == 5) {
		inname = argv[1];
		dir = atoi(argv[2]);
		width = atoi(argv[3]);
		height = atoi(argv[4]);
		outname = "out_edge.yuv";
	} else {
		printf("Usage: %s <Input bmp file> <Threshold>\n", argv[0]);
		printf("Usage: %s <Input yuv file> <0|1|2> <Width> <Height> \n"
				"\t Direction: 0=X; 1=Y; 2=XY\n", argv[0]);
		return -1;
	}

	printf("Input File: %s, Threshold: %lf\n", inname, threshold);
	printf("Output File: %s\n", outname);

	do {
		fp_s = fopen(inname, "rb");
		if (fp_s == NULL) {
			printf("fopen fp_s error\n");
			break;
		}
		fp_t = fopen(outname, "wb");
		if (fp_t == NULL) {
			printf("fopen fname_t error\n");
			break;
		}
		read_bmp_head(fp_s, &file, &info);
		if (file.bfType == 0x4d42) {
			is_bmp = 1;
		}

		if (is_bmp) {
			show_bmp_head(&file, &info);
			if (info.biBitCount != 24) {
				printf("Only support 24bit bmp, exit\n");
				break;
			}
			raw_size = info.biWidth * info.biHeight * (info.biBitCount >> 3);
		} else {
			raw_size = width * height;
			fseek(fp_s, 0, SEEK_SET);
		}
		printf("raw_size=%d\n", raw_size);
		image_s = (unsigned char *)malloc(raw_size);
		if (image_s == NULL) {
			printf("malloc images_s error\n");
			break;
		}
		image_t = (unsigned char *)malloc(raw_size);
		if (image_t == NULL) {
			printf("malloc image_t error\n");
			break;
		}

		read_bmp_raw(fp_s, image_s, raw_size); //24 bit gray level image

		if (is_bmp) {
			sobel_rgb(image_s, image_t, info.biWidth, info.biHeight,
				info.biBitCount >> 3, threshold);
			write_bmp_head(fp_t, &file, &info, image_s);
		} else {
			sobel_yuv(image_s, image_t, width, height, 1, dir);
		}

		write_bmp_raw(fp_t, image_t, raw_size);
	} while (0);

	if (image_t != NULL) {
		free(image_t);
		image_t = NULL;
	}
	if (image_s != NULL) {
		free(image_s);
		image_s = NULL;
	}
	if (fp_t != NULL) {
		fclose(fp_t);
	}
	if (fp_s != NULL) {
		fclose(fp_s);
	}

	return 0;
}

/*
 * cjpeg.c
 *
 * This file was part of the Independent JPEG Group's software:
 * Copyright (C) 1991-1998, Thomas G. Lane.
 * Modified 2003-2011 by Guido Vollbeding.
 * libjpeg-turbo Modifications:
 * Copyright (C) 2010, 2013, D. R. Commander.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains a command-line user interface for the JPEG compressor.
 * It should work on any system with Unix- or MS-DOS-style command lines.
 *
 * Two different command line styles are permitted, depending on the
 * compile-time switch TWO_FILE_COMMANDLINE:
 *	cjpeg [options]  inputfile outputfile
 *	cjpeg [options]  [inputfile]
 * In the second style, output is always to standard output, which you'd
 * normally redirect to a file or pipe to some other program.  Input is
 * either from a named file or from standard input (typically redirected).
 * The second style is convenient on Unix but is unhelpful on systems that
 * don't support pipes.  Also, you MUST use the first style if your system
 * doesn't do binary I/O to stdin/stdout.
 * To simplify script writing, the "-outfile" switch is provided.  The syntax
 *	cjpeg [options]  -outfile outputfile  inputfile
 * works regardless of which command line style is used.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>

#include <jpeglib.h>
#include <setjmp.h>
#include <turbojpeg.h>

typedef unsigned char  u8;


static const char * progname;	/* program name for error messages */
static char * outfilename;	/* for -outfile switch */
boolean memdst;  /* for -memdst switch */

#define JPEG_QUALITY_MIN 	(0)
#define JPEG_QUALITY_MAX 	(100)
#define COLOR_COMPONENTS 	(4)
const char *default_filename_jpeg= "/tmp/out_turbo.jpeg";
static int jpeg_quality = 60;
typedef struct rgb_data_s
{
	u8 *image_buf;
	int image_width;	/* Number of columns in image */
	int image_height;	/* Number of rows in image */
}rgb_data_t;
static u8* jpeg_buf = NULL;
static rgb_data_t rgb_data= {NULL, 0, 0};


static int init_rgb_mem(int width, int height)
{
	static int rgb_mem_mapped = 0;

	if (rgb_mem_mapped)
		return 0;

	rgb_data.image_width = width;
	rgb_data.image_height = height;
	rgb_data.image_buf = (u8*) malloc(width * height * COLOR_COMPONENTS);
	if (!rgb_data.image_buf) {
		printf("Not enough memory for rgb buffer.\n");
		return -1;
	}
	memset(rgb_data.image_buf, 0, width * height * COLOR_COMPONENTS);
	rgb_mem_mapped = 1;

	return 0;
}

static int init_jpeg_mem(int width, int height)
{
	static int jpeg_mem_mapped = 0;

	if (jpeg_mem_mapped)
		return 0;

	jpeg_buf = tjAlloc(width * height * COLOR_COMPONENTS);
	if (jpeg_buf == NULL) {
		printf("Not enough memory for jpeg buffer.\n");
		return -1;
	}
	memset(jpeg_buf, 0, width * height * COLOR_COMPONENTS);
	jpeg_mem_mapped = 1;

	return 0;
}
struct BITMAP_FILE_HEADER
{
    //bitmap file header  (14 bytes in total)
    char bm[2];     //"BM"
    unsigned char file_size[4];   //size of bitmap file in little-endian, (bytes)
    char reserved[4]; //should be 0
    unsigned char pixel_array_offset[4]; //from start of file, in bytes.
    //bitmap info header
    unsigned char info_header_size[4]; //in bytes
    unsigned char bitmap_width[4];
    unsigned char bitmap_height[4];
    unsigned char num_color_planes[2];  //must be 1
    unsigned char num_bits_per_pixel[2];  //set to 24
    unsigned char compressed_format[4];   //set to 0
    unsigned char reserved2[4];         //set to 0
    unsigned char h_resolution[4];    //set to 2835  which means 72DPI
    unsigned char v_resolution[4];    //set to 2835  which means 72DPI
    unsigned char num_colors_in_palette[4]; //set to 0
    unsigned char reserved3[4];     //set to 0
};


/*
 * The main program.
 */

int
main (int argc, char **argv)
{
	int ret = -1;
	int width=1920;
	int height=1080;

	int pf=TJPF_RGBA;
	int ps=tjPixelSize[pf];
//	int pitch=width * ps;

	tjhandle handle=NULL;
	FILE *filejpeg=NULL;
	int fd_in = -1;

	unsigned long jpegsize=0;

	struct timeval lasttime, curtime;

	init_rgb_mem(width, height);
	init_jpeg_mem(width, height);

	gettimeofday( &lasttime, NULL);

	handle=tjInitCompress();
	if (handle==NULL) {
		printf("tjInitCompress failed\n");
	}

	fd_in = open(argv[1], O_RDWR);
	if (fd_in < 0) {
		printf("open failed.\n");
		return -1;
	}

	//lseek(fd_in, sizeof(struct BITMAP_FILE_HEADER), SEEK_SET);
	ret = read(fd_in, rgb_data.image_buf, width * height * COLOR_COMPONENTS);
	if (ret != width * height * COLOR_COMPONENTS) {
		printf("read failed.\n");
		return -1;
	}

	/* Execute once to preload cache */
	if(tjCompress2(handle, rgb_data.image_buf, rgb_data.image_width, 0,
		rgb_data.image_height, pf, &jpeg_buf,
		&jpegsize, TJSAMP_444, jpeg_quality, TJFLAG_FASTDCT)==-1) {
		printf("tjCompress2 failed. msg=%s. jpegsize=%d.\n", tjGetErrorStr(), jpegsize);
	}
	printf("tjCompress2 ok. jpegsize=%d.\n", tjGetErrorStr(), jpegsize);

	if((filejpeg=fopen(default_filename_jpeg, "wb"))==NULL) {
		printf("fopen failed.\n");
	}

	if(fwrite(jpeg_buf, jpegsize, 1, filejpeg)!=1) {
		printf("fwrite failed.\n");
	}
	printf("Writer JPEG file [%s]\n", default_filename_jpeg);

	if(tjDestroy(handle)==-1) {
		printf("tjDestroy failed\n");
	}
	handle=NULL;

	if (filejpeg) {fclose(filejpeg);  filejpeg=NULL;}

	gettimeofday( &curtime, NULL);
	printf("JPEG Elapsed Time  (ms): [%05ld]\n", (curtime.tv_sec - lasttime.tv_sec) * 1000
		  + (curtime.tv_usec - lasttime.tv_usec)/1000);
	lasttime = curtime;

	return 0;			/* suppress no-return-value warnings */
}

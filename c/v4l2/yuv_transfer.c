#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef  unsigned char u8;


//refer to http://en.wikipedia.org/wiki/BMP_file_format
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

void usage(char* execname)
{
	printf("%s fin fout w h.\n",execname);
}

void nv12_to_rgba32(u8 *in, u8* out, int width, int height)
{
	int r, g, b;
	int w, h;

	u8* py = in;
	u8* pu = in + width*height;
	u8* pv = pu + 1;

	u8* pout=out;

	for (h=0; h<height; h++) {
		for (w=0; w<width; w++) {
			r = (*py) + (int)(1.402f * (*pv));
			g = (*py) + (int)(0.344f * (*pu) + 0.714 * (*pv));
			b = (*py) - (int)(1.772f * (*pu));

			r = r>255? 255 : r<0 ? 0 : r;
			g = g>255? 255 : g<0 ? 0 : g;
			b = b>255? 255 : b<0 ? 0 : b;

			*(pout++) = r;
			*(pout++) = g;
			*(pout++) = b;
			//*(pout++) = 0xff;

			py++;
			if ((w & 3) == 1) {
				pu+=2;
				pv+=2;
			}
		}
	}

}

int main(int argc, char* argv[])
{
	FILE* fin = NULL;
	FILE* fout = NULL;

	int w = 640;
	int h = 480;

	int frmsize = 0;
	int ret = 0;

	u8* bufin = NULL;
	u8* bufout = NULL;
	u8* bufbmp = NULL;


	int file_length = 0;
	int pixel_array_offset = 0;
	int info_header_size = 0;
	int num_color_planes = 1;
	int num_bits_per_pixel = 24;
	int pixel_resolution = 2835;


	if(argc<5){
		usage(argv[0]);
		return -1;
	}

	fin = fopen(argv[1], "rb");
	if(NULL==fin){
		printf("open in file %s failed\n", argv[1]);
		goto errout;
	}

	fout = fopen(argv[2], "wb+");
	if(NULL==fout){
		printf("open out file %s failed\n", argv[2]);
		goto errout;
	}

	w = atoi(argv[3]);
	h = atoi(argv[4]);


	if(w<0||h<0){
		printf("invalid wxh %dx%d\n", w,h);
		goto errout;
	}
	frmsize = w*h;

	bufin = (u8*)malloc(frmsize * 3/2);
	if(NULL==bufin){
		printf("malloc bufin failed\n");
		goto errout;
	}
	memset(bufin, 0, frmsize * 3/2);

	bufout = (u8*)malloc(frmsize * 3);
	if(NULL==bufout){
		printf("malloc bufout failed\n");
		goto errout;
	}
	memset(bufout, 0, frmsize*3);

	bufbmp = (u8*)malloc(sizeof(struct BITMAP_FILE_HEADER));
	if(NULL==bufbmp){
		printf("malloc bufbmp failed\n");
		goto errout;
	}

	printf("#### w=%d, h=%d, infile=%s, outfile=%s\n", w, h, argv[1], argv[2]);

	ret = fread(bufin, 1, frmsize *3/2, fin);

	if (ret != frmsize *3/2) {
		printf("read failed, ret=%d.\n", ret);
		goto errout;
	}

	nv12_to_rgba32(bufin, bufout, w, h);


	//write the bitmap header
	struct BITMAP_FILE_HEADER *bitmap_file_header =
		(struct BITMAP_FILE_HEADER *) bufbmp;
	memset(bitmap_file_header, 0, sizeof(*bitmap_file_header));
	bitmap_file_header->bm[0] = 'B';
	bitmap_file_header->bm[1] = 'M';
	file_length = sizeof(struct BITMAP_FILE_HEADER) + w * h * 3;
	memcpy(bitmap_file_header->file_size, &file_length, 4);
	pixel_array_offset = sizeof(struct BITMAP_FILE_HEADER);
	memcpy(bitmap_file_header->pixel_array_offset, &pixel_array_offset, 4);
	info_header_size = 40; //always 40
	memcpy(bitmap_file_header->info_header_size, &info_header_size, 4);
	memcpy(bitmap_file_header->bitmap_width, &w, 4);
	memcpy(bitmap_file_header->bitmap_height, &h, 4);
	memcpy(bitmap_file_header->num_color_planes, &num_color_planes, 2);
	memcpy(bitmap_file_header->num_bits_per_pixel, &num_bits_per_pixel, 2);
	memcpy(bitmap_file_header->h_resolution, &pixel_resolution, 4);
	memcpy(bitmap_file_header->v_resolution, &pixel_resolution, 4);

	printf("info_header_size = %lu.\n", sizeof(*bitmap_file_header));
	fwrite(bufbmp, 1, sizeof(*bitmap_file_header), fout);
	fwrite(bufout, 1, frmsize*3, fout);

	fflush(fin);
	fclose(fin);
	fflush(fout);
	fclose(fout);

	free(bufin);
	free(bufout);
	return 0;
errout:
	if(NULL!=bufin)
		free(bufin);
	if(NULL!=bufout)
		free(bufout);

	if(NULL!=fin)
		fclose(fin);
	if(NULL!=fout)
		fclose(fout);
	return -1;
}


#include "cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jversion.h"		/* for version message */

#ifdef USE_CCOMMAND		/* command-line reader for Macintosh */
#ifdef __MWERKS__
#include <SIOUX.h>              /* Metrowerks needs this */
#include <console.h>		/* ... and this */
#endif
#ifdef THINK_C
#include <console.h>		/* Think declares it here */
#endif
#endif

#include <fcntl.h>
typedef unsigned char  	    u8;	/**< UNSIGNED 8-bit data type */
typedef unsigned short 	    u16;/**< UNSIGNED 16-bit data type */


JSAMPLE * image_buffer;	/* Points to large array of R,G,B-order data */
int image_width = 640;		/* Number of columns in image */
int image_height =480;	/* Number of rows in image */

void deinterleave(unsigned char* vu_planar, unsigned char* u_row,
	unsigned char* v_row, int row_index, int width, int height)
{
	int num_rows = (height - row_index) / 2;
	if (num_rows > 8) num_rows = 8;
	for (int row = 0; row < num_rows; ++row) {
		int offset = ((row_index >> 1) + row) * width;
		unsigned char* vu = vu_planar + offset;
		for (int i = 0; i < (width >> 1); ++i) {
			int index = row * (width >> 1) + i;
			u_row[index] = vu[1];
			v_row[index] = vu[0];
			vu += 2;
		}
	}
}

void config_sampling_factors(struct jpeg_compress_struct *cinfo)
{
	cinfo->comp_info[0].h_samp_factor = 2;
	cinfo->comp_info[0].v_samp_factor = 2;
	cinfo->comp_info[1].h_samp_factor = 1;
	cinfo->comp_info[1].v_samp_factor = 1;
	cinfo->comp_info[2].h_samp_factor = 1;
	cinfo->comp_info[2].v_samp_factor = 1;
}

void write_JPEG_file (char * filename, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	FILE * outfile;		/* target file */
//	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
//	int row_stride;		/* physical row width in image buffer */

	JSAMPROW y[16];
	JSAMPROW cb[8];
	JSAMPROW cr[8];
	JSAMPARRAY planes[3];
	planes[0] = y;
	planes[1] = cb;
	planes[2] = cr;

	int width = image_width;
	int height = image_height;

	u8* y_planar = image_buffer;
	u8* vu_planar = image_buffer + width * height;
	u8* u_rows_ = vu_planar;
	u8* v_rows_ = vu_planar + 1;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = image_width; 	/* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_YCbCr; 	/* colorspace of input image */

	jpeg_set_defaults(&cinfo);
//	jpeg_set_colorspace(&cinfo, JCS_YCbCr);

	cinfo.raw_data_in = TRUE;
	cinfo.dct_method = JDCT_FASTEST;
	cinfo.write_JFIF_header = FALSE;
	cinfo.restart_interval = 10;

	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
	jpeg_start_compress(&cinfo, TRUE);

//	row_stride = image_width * 1;	/* JSAMPLEs per row in image_buffer */
	config_sampling_factors(&cinfo);

	printf ("[USERSP]  %s, Line:%d. cinfo.next_scanline=%u. width=%d, height=%d.\n",
		__FUNCTION__, __LINE__, cinfo.next_scanline, width, height);

	while (cinfo.next_scanline < cinfo.image_height) {
		//deitnerleave u and v
		deinterleave(vu_planar, u_rows_, v_rows_, cinfo.next_scanline, width, height);
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
		// Jpeg library ignores the rows whose indices are greater than height.
		for (int i = 0; i < 16; i++) {
			// y row
			y[i] = y_planar + (cinfo.next_scanline + i) * width;

			// construct u row and v row
			if ((i & 1) == 0) {
				// height and width are both halved because of downsampling
				int offset = (i >> 1) * (width >> 1);
				cb[i/2] = u_rows_ + offset;
				cr[i/2] = v_rows_ + offset;
			}
		}
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
		jpeg_write_raw_data(&cinfo, planes, 16);
		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
	}

	printf ("[USERSP]  %s, Line:%d. cinfo.next_scanline=%u.\n", __FUNCTION__, __LINE__,
		cinfo.next_scanline);

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);

}

int
main (int argc, char **argv)
{
	int len = -1;
	int size_input = -1;
	int fd_input = -1;

	char file_out[32]="1a_out.jpeg";

	do {
		size_input = (image_width* image_height * 3/2);
		printf("[%s] size is [%u].\n", argv[1], size_input);
		if ((fd_input = open(argv[1], O_RDONLY )) < 0) {
			printf("Failed to open file [%s].\n", argv[1]);
			break;
		}

		image_buffer = (u8 *) malloc(size_input);
		if (!image_buffer) {
			printf("Not enough memory for image buffer.\n");
			break;
		}
		memset(image_buffer, 0, size_input);
		len = read(fd_input, image_buffer, size_input);
		if (len != size_input) {
			printf("Read %s error\n", argv[1]);
			break;
		}

		printf ("[USERSP]  %s, Line:%d.\n", __FUNCTION__, __LINE__);
		write_JPEG_file(file_out, 60);
		printf("Write JPEG file [%s] OK.\n", file_out);
	} while (0);

	if (fd_input >0) {
		close(fd_input);
		fd_input = -1;
	}
	if (image_buffer) {
		free(image_buffer);
	}

  return 0;			/* suppress no-return-value warnings */
}

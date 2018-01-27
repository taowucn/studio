
/* Author : Jiejing.zhang<kzjeef(at)gmail.com>
 *
 * JUST reference Linux-UVC streaming input-plugin for MJPG-streamer
 *
 * same License as , GPL2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>          /* for videodev2.h */
#include <jpeglib.h>

int             _still_width            = 2048;
int             _still_height           = 1536;

/* default size, This just the size of sample yuv  */

#define OUTPUT_BUF_SIZE 4096

typedef struct {
  struct jpeg_destination_mgr pub; /* public fields */

  JOCTET * buffer;    /* start of buffer */

  unsigned char *outbuffer;
  int outbuffer_size;
  unsigned char *outbuffer_cursor;
  int *written;
}my_destmgr;

METHODDEF(void) init_destination(j_compress_ptr cinfo) {
  my_destmgr * dest = (my_destmgr*) cinfo->dest;

  /* Allocate the output buffer --- it will be released when done with
   * image */
  dest->buffer = (JOCTET *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE, OUTPUT_BUF_SIZE * sizeof(JOCTET));

  *(dest->written) = 0;

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;
}

/******************************************************************************
Description.: called whenever local jpeg buffer fills up
Input Value.:
Return Value:
******************************************************************************/
METHODDEF(boolean) empty_output_buffer(j_compress_ptr cinfo)
{
  my_destmgr *dest = (my_destmgr *) cinfo->dest;

  memcpy(dest->outbuffer_cursor, dest->buffer, OUTPUT_BUF_SIZE);
  dest->outbuffer_cursor += OUTPUT_BUF_SIZE;
  *(dest->written) += OUTPUT_BUF_SIZE;

  dest->pub.next_output_byte = dest->buffer;
  dest->pub.free_in_buffer = OUTPUT_BUF_SIZE;

  return TRUE;
}


METHODDEF(void) term_destination(j_compress_ptr cinfo)
{
  my_destmgr * dest = (my_destmgr *) cinfo->dest;
  size_t datacount = OUTPUT_BUF_SIZE - dest->pub.free_in_buffer;

  /* Write any data remaining in the buffer */
  memcpy(dest->outbuffer_cursor, dest->buffer, datacount);
  dest->outbuffer_cursor += datacount;
  *(dest->written) += datacount;
}

void dest_buffer(j_compress_ptr cinfo, unsigned char *buffer, int size, int *written)
{
  my_destmgr * dest;

  if (cinfo->dest == NULL) {
    cinfo->dest = (struct jpeg_destination_mgr *)(*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_destmgr));
  }

  dest = (my_destmgr*) cinfo->dest;
  dest->pub.init_destination = init_destination;
  dest->pub.empty_output_buffer = empty_output_buffer;
  dest->pub.term_destination = term_destination;
  dest->outbuffer = buffer;
  dest->outbuffer_size = size;
  dest->outbuffer_cursor = buffer;
  dest->written = written;
}



/* Encodes a YUV planar frame of width "width and height "height" at "src" straight
 * into a JPEG frame at "dst" (must be allocated y caller). "len" is set to the
 * length of the compressed JPEG frame. "j" contains the JPEG compressor and
 * must be initialised correctly by the caller
 */
/* YUV420P */
static int jpeg_encode_yuv(void *src, int src_len,  void *dst, int pwidth, int pheight) {
    //Code for this function is taken from Motion
    //Credit to them !!!
    //Also Credit mjpg-streamer

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    my_destmgr dest;

    memset(&cinfo , 0, sizeof(cinfo));

    JSAMPROW y[16],cb[16],cr[16];
    JSAMPARRAY data[3];
    int i, line, width = pwidth, height = pheight;
    int written;                /* for count file size */

    printf("Encoding a %dx%d frame\n", width, height);

    data[0] = y;
    data[1] = cb;
    data[2] = cr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    //init JPEG dest mgr
    dest_buffer(&cinfo, dst, src_len, &written);

    jpeg_set_quality(&cinfo, 100, TRUE);

    cinfo.input_components = 3;
    cinfo.image_width = width;
    cinfo.image_height = height;

    cinfo.in_color_space = JCS_YCbCr;
    jpeg_set_defaults(&cinfo);
    jpeg_set_colorspace(&cinfo, JCS_YCbCr);
    cinfo.raw_data_in = TRUE; // supply downsampled data
    /* set_defaults will set YUV 420 format */
    /* cinfo.comp_info[0].h_samp_factor = 2; */
    /* cinfo.comp_info[0].v_samp_factor = 2; */
    /* cinfo.comp_info[1].h_samp_factor = 1; */
    /* cinfo.comp_info[1].v_samp_factor = 1; */
    /* cinfo.comp_info[2].h_samp_factor = 1; */
    /* cinfo.comp_info[2].v_samp_factor = 1; */

    cinfo.dct_method = JDCT_FLOAT;
    /* This method will faster in faster HW */

    jpeg_start_compress( &cinfo, TRUE );
    for (line=0; line<height; line+=16) {
        for (i=0; i<16; i++) {
            y[i] = src + width*(i+line);
            if (i%2 == 0) {
                cb[i/2] = src + width*height + width/2*((i+line)/2);
                cr[i/2] = src + width*height + width*height/4 + width/2*((i+line)/2);
            }
        }
        jpeg_write_raw_data(&cinfo, data, 16);
    }
    jpeg_finish_compress(&cinfo);
    return written;
}


void convert_to_jpg (unsigned char *yuv_buf, size_t length)
{
    char *buf = malloc(length);
    assert(buf != NULL);
    long size = jpeg_encode_yuv(yuv_buf, length, buf,_still_width, _still_height);
    const char *name = "/tmp/still.jpg";
    unlink(name);
    int tfd = creat (name, 0777);
    if (write (tfd, buf, size) != size) {
        perror("write on jpg");
        exit(1);
    }
    close(tfd);
}

void usage (void)
{
    printf("usage: \n"
           "This yuv2jpg only support yuv420P format to a jpg image\n"
           "Just a show for how to use libjpeg convert yuv to jpg\n"
           "./yuv2jpg [yuv_file_path]\n"
           "the Jpg image will put in /tmp/still.jpg\n");
};

int main(int argc, char **argv) {
    char *buf;
    int maxsize;
    int size;
    struct stat s;

    if (stat(argv[1], &s) < 0) {
        perror("source file open error");
        usage();
        exit(1);
    }

    maxsize = s.st_size;
    /* We use YUV source file size,since after compress file size
     * always smaller then raw souce file
     * */

    FILE* f = fopen(argv[1], "rb");
    if (!f) {
        perror ("file open error");
        exit(1);
    }

    buf = malloc(maxsize);
    assert(buf);
    size =  fread(buf, 1, maxsize, f);
    printf(" fread return %d : %s\n ", size, strerror(errno));
    assert(size >= 0);
    convert_to_jpg(buf, size);
}


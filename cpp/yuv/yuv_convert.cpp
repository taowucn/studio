#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>


 enum PIXEL_COLOR_FORMAT{
    //packed format
    FMT_UYVY = 0,   //YUV 4:2:2 (Y sample at every pixel, U and V sampled at every second pixel horizontally on each line). A macropixel contains 2 pixels in 1 u_int32.
    FMT_YVYU = 1,   //422
    FMT_YUYV = 2,   //YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel.
    FMT_YUY2 = 2,  //same as YUYV
    FMT_V210 = 5,   //10-bit 4:2:2 YCrCb equivalent to the Quicktime format of the same name (or 422p10le)

    //planar format
    FMT_I420 = 10,     //8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes.
    FMT_IYUV = 10,     //identical to I420
    FMT_NV12 = 11,      //8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling (4:2:0)
    FMT_NV21 = 12,      //As NV12 with U and V reversed in the interleaved plane  (4:2:0)
    FMT_YV16 = 13,      //8 bit Y plane followed by 8 bit 2x1 subsampled V and U planes
    FMT_YV12 = 14,      //8 bit Y plane followed by 8 bit 2x2 subsampled V and U planes. (similar to I420 but with reversed U/V orrder
    FMT_Y800 =  20,     //Simple, single Y plane for monochrome images.
    FMT_Y8   =  20,         //same as Y800
    FMT_YU16 =  21,      //8 bit Y plane followed by 8 bit 2x1 subsampled U and V planes  ( only U/V order is reversed, compare to YV16 ) ( Compatible to Geeviewer "YUV" )
    FMT_YUV444 = 22,    //8 bit Y plane followed by 8-bit U and 8-b it V planes for each pixel ( no subsample U/V).  (Compatible to Geeviewer "YUV444" )
    FMT_Y8_ME = 23,     //format same as Y8, however, width is 1/4, and height is 1/4 of original.
    FMT_RGB_BITMAP = 24,  //this is not a YUV format, but a RGB format
};


struct CYUVPixel
{
    unsigned char Y;
    unsigned char U;
    unsigned char V;
    unsigned char reserved;
};




class CYUVConverter{
    protected:
            char * m_buffer;      //for input and output file buffer
            CYUVPixel * m_yuv_array;
            int get_frame_size(int yuv_format);
            int input_to_yuv_array(FILE *fp, int frame_size);
            int yuv_array_to_output(FILE *fp);
            int load_v210();
            int load_i420();
            int load_yv12();
            int load_nv12();
            int load_nv21();
            int load_yv16();
            int load_yu16();
            int load_uyvy();
            int load_yuyv();
            int load_yvyu();
            int load_yuv444();
            int prepare_v210();
            int prepare_i420();
            int prepare_yv12();
            int prepare_nv12();
            int prepare_nv21();
            int prepare_yv16();
            int prepare_yu16();
            int prepare_uyvy();
            int prepare_yuyv();
            int prepare_yvyu();
            int prepare_yuv444();
            int prepare_y8me();
            int prepare_rgb_bitmap();
            int m_width;
            int m_height;

            int m_input_format;
            int m_output_format;
            int m_input_format_set;
            int m_output_format_set;

            char * m_input_filename;
            char * m_output_filename;


    public:
        CYUVConverter();
        virtual ~CYUVConverter();
        int init();
        int set_input_format(int yuv_format, int width, int height);
        int set_output_format(int yuv_format);
        int set_input_filename(char * pathname);
        int set_output_filename(char * pathname);
        int convert(int frame_count);

};


CYUVConverter::CYUVConverter()
{
    m_buffer = NULL;
    m_yuv_array = NULL;
    m_width = 0;
    m_height = 0;

    m_input_format_set = 0;
    m_output_format_set = 0;

    m_input_filename = NULL;
    m_output_filename = NULL;

}

CYUVConverter::~CYUVConverter()
{
    delete[] m_buffer;
    delete[] m_yuv_array;

}

int CYUVConverter::init()
{



    return 0;
}

int CYUVConverter::set_input_format(int yuv_format, int width, int height)
{
        int ret = 0;

         if (m_input_format_set) {
                printf("already set input format \n");
                return -1;
        }

        do {
                //check value
                if  ((width & 3) || ( width <= 0)) {
                    printf("invalid input width \n");
                    ret = -1;
                    break;
                }

                if  ((height & 3) || ( height <= 0)) {
                    printf("invalid input height \n");
                    ret = -2;
                    break;
                }

              //allocate mem, allocate the max possible memory, it's even enough for 10-bit YUV 422
                m_buffer = new char[width * height * 3];
                if (!m_buffer)  {
                    printf("insufficient mem for input buffer \n");
                    ret = -3;
                    break;
               }

                memset(m_buffer, 0, width * height * 3);

                m_yuv_array = new CYUVPixel[width * height];
                if (!m_yuv_array)  {
                    printf("insufficient mem for yuv buffer \n");
                    ret = -4;
                    break;
               }

                memset(m_yuv_array, 0,  width * height * sizeof(CYUVPixel));


       }while(0);


        if (ret != 0)  {
            if (m_buffer)
                delete[] m_buffer;
            if (m_yuv_array)
                delete[] m_yuv_array;
        } else {

            m_input_format = yuv_format;
            m_width = width;
            m_height = height;
            m_input_format_set = 1;
       }

        return ret;
}

int CYUVConverter::set_output_format(int yuv_format)
{
    if (m_output_format_set) {
            printf("output format already set \n");
            return -1;
     }
    m_output_format = yuv_format;
    m_output_format_set = 1;

    if (yuv_format == FMT_Y8_ME) {
        if ((m_width & 3) || ( m_height & 3)) {
            printf("input width and height must be multiple of 4 to do Y8_ME output \n");
            return -1;
        }
    }

    return 0;
}

int CYUVConverter::set_input_filename(char * pathname)
{
    if (m_input_filename != NULL) {
            printf("input file name already set \n");
            return -1;
     }

    m_input_filename = strdup (pathname);

    return 0;

}

int CYUVConverter::set_output_filename(char * pathname)
{
    if (m_output_filename != NULL) {
            printf("output file name already set \n");
            return -1;
     }

    m_output_filename = strdup (pathname);

    return 0;
}


int CYUVConverter::load_v210()
{
    int i, j;
    unsigned int    *  data_block = (unsigned int *) m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;
    unsigned int       tmp;


    //refer to here about YUV v210 format:   http://wiki.multimedia.cx/index.php?title=V210
    //each block is 4 bytes,  every 4 blocks are 6 pixels of yuv 210,  so every 16 bytes are 6 pixels of yuv210

    //so the width of YUV210 must be multiple of 6, otherwise, report error
    if (m_width%6) {
        printf("width must be multiple of 6 for YUV210 format, otherwise, need detail format definition \n");
        return -1;
    }

    for (j=0; j < m_height; j++) {
            for (i= 0 ; i < m_width/6; i++)   {
                    //truncate 10-bit data into 8-bit for YUV 4:2:2 8-bit storage in YUV array
                    //pixel 1
                    tmp = *data_block;
                    update_pixel->U = (tmp >> 2) & 0xFF;
                    update_pixel->Y = (tmp >> 12) & 0xFF;
                    update_pixel->V = (tmp >> 22) & 0xFF;

                    //pixel 2 (shares U/V with pixel 1)
                    (update_pixel+1)->U = update_pixel->U;
                    (update_pixel+1)->V = update_pixel->V;
                    update_pixel++;
                    data_block++;
                    tmp = *data_block;
                    update_pixel->Y = (tmp >> 2) & 0xFF;
                    //pixel 3
                    update_pixel++;
                    update_pixel->U = (tmp >> 12) & 0xFF;
                    update_pixel->Y = (tmp >> 22) & 0xFF;
                    data_block++;
                    tmp = *data_block;
                    update_pixel->V = (tmp >> 2) & 0xFF;
                    //pixel 4 (shares U/V with pixel 3)
                    (update_pixel+1)->U = update_pixel->U;
                    (update_pixel+1)->V = update_pixel->V;
                    update_pixel++;
                    update_pixel->Y = (tmp >>12) & 0xFF;

                    //pixel 5
                    update_pixel++;
                    update_pixel->U  = (tmp>>22)& 0xFF;
                    data_block++;
                    tmp = *data_block;
                    update_pixel->Y  = (tmp >> 2) & 0xFF;
                    update_pixel->V  = (tmp >> 12) & 0xFF;

                    //pixel 6 (shares U/V with pixel 5)
                    (update_pixel+1)->U = update_pixel->U;
                    (update_pixel+1)->V = update_pixel->V;
                    update_pixel++;
                    update_pixel->Y = (tmp>>22) & 0xFF;

                    //move to next pixel and next block
                    update_pixel++;
                    data_block++;
            }
        }

    return 0;
}

int CYUVConverter::load_i420()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load U, 2x2 subsampled
    for (j=0;j < m_height; j+=2)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                update_pixel[i + (j + 1)* m_width].U = *input_data;
                update_pixel[i +1 + (j+1)* m_width].U = *input_data;
                input_data++;
             }
    }

    //load V, 2x2 subsampled
    for (j=0;j < m_height; j+=2)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                update_pixel[i + (j + 1)* m_width].V = *input_data;
                update_pixel[i +1 + (j+1)* m_width].V = *input_data;
                input_data++;
             }
    }


    return 0;
}



int CYUVConverter::load_yv12()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load V, 2x2 subsampled
    for (j=0;j < m_height; j+=2)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                update_pixel[i + (j + 1)* m_width].V = *input_data;
                update_pixel[i +1 + (j+1)* m_width].V = *input_data;
                input_data++;
             }
    }

    //load U, 2x2 subsampled
    for (j=0;j < m_height; j+=2)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                update_pixel[i + (j + 1)* m_width].U = *input_data;
                update_pixel[i +1 + (j+1)* m_width].U = *input_data;
                input_data++;
             }
    }
    return 0;
}


int CYUVConverter::load_yv16()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load V, 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                input_data++;
             }
    }

    //load U, 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                input_data++;
             }
    }
    return 0;
}


int CYUVConverter::load_yu16()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load U, 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                input_data++;
             }
    }


    //load V, 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                input_data++;
             }
    }

    return 0;
}


int CYUVConverter::load_nv12()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load V/U, 2x2 subsampled
    for (j=0;j < m_height; j+=2)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                update_pixel[i + (j+1)* m_width].U = *input_data;
                update_pixel[i +1 + (j+1)* m_width].U = *input_data;
                input_data++;
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                update_pixel[i + (j+1)* m_width].V = *input_data;
                update_pixel[i +1 + (j+1)* m_width].V = *input_data;
                input_data++;
             }
    }

    return 0;
}


int CYUVConverter::load_nv21()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load V/U, 2x2 subsampled
    for (j=0;j < m_height; j+=2)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                update_pixel[i + (j+1)* m_width].V = *input_data;
                update_pixel[i +1 + (j+1)* m_width].V = *input_data;
                input_data++;
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                update_pixel[i + (j+1)* m_width].U = *input_data;
                update_pixel[i +1 + (j+1)* m_width].U = *input_data;
                input_data++;

             }
    }

    return 0;
}

int CYUVConverter::load_uyvy()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;



    //load UYVY    , U/V is 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                input_data++;
                update_pixel[i + j* m_width].Y = *input_data;
                input_data++;
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                input_data++;
                update_pixel[i +1 + j* m_width].Y = *input_data;
                input_data++;
             }
    }

    return 0;
}

int CYUVConverter::load_yuyv()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;


    //load YUYV    , U/V is 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].Y = *input_data;
                input_data++;
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                input_data++;
                update_pixel[i +1 + j* m_width].Y = *input_data;
                input_data++;
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                input_data++;
             }
    }

    return 0;
}

int CYUVConverter::load_yvyu()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;


    //load YVYU    , V/U is 2x1 subsampled
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i+=2) {
                update_pixel[i + j* m_width].Y = *input_data;
                input_data++;
                update_pixel[i + j* m_width].V = *input_data;
                update_pixel[i +1 + j* m_width].V = *input_data;
                input_data++;
                update_pixel[i +1 + j* m_width].Y = *input_data;
                input_data++;
                update_pixel[i + j* m_width].U = *input_data;
                update_pixel[i +1 + j* m_width].U = *input_data;
                input_data++;
             }
    }

    return 0;
}



int CYUVConverter::load_yuv444()
{
    int i, j;
    unsigned char * input_data   =  (unsigned char *)m_buffer;
    CYUVPixel       *  update_pixel    = m_yuv_array;

    //load Y
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].Y = *input_data;
                 input_data++;
             }
    }

    //load U
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].U = *input_data;
                input_data++;
             }
    }

    //load V
    for (j=0;j < m_height; j++)   {
            for (i=0;i<m_width; i++) {
                update_pixel[i + j* m_width].V = *input_data;
                input_data++;
             }
    }


    return 0;
}


int CYUVConverter::prepare_v210()
{

    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
    unsigned int    *  data_block = (unsigned int *) m_buffer;
    unsigned int tmp;
    //refer to here about YUV v210 format:   http://wiki.multimedia.cx/index.php?title=V210
    //each block is 4 bytes,  every 4 blocks are 6 pixels of yuv 210,  so every 16 bytes are 6 pixels of yuv210
    //so the width of YUV210 must be multiple of 6, otherwise, report error
    if (m_width%6) {
        printf("width must be multiple of 6 for YUV210 format, otherwise, need detail format definition \n");
        return -1;
    }

    for (j=0; j < m_height; j++) {
            for (i= 0 ; i < m_width; i+=6)   {
                tmp = 0;
                tmp |= (yuv_pixel[i + j* m_width].U <<2);
                tmp |= (yuv_pixel[i + j* m_width].Y <<(10+2));
                tmp |= (yuv_pixel[i + j* m_width].V <<(20+2));
                *data_block = tmp;
                data_block++;
                tmp = 0;
                tmp |= (yuv_pixel[i + 1 + j* m_width].Y <<2);
                tmp |= (yuv_pixel[i + 2 + j* m_width].U <<(10+2));
                tmp |= (yuv_pixel[i + 2 + j* m_width].Y <<(20+2));
                *data_block = tmp;
                data_block++;
                tmp = 0;
                tmp |= (yuv_pixel[i + 2 + j* m_width].V <<2);
                tmp |= (yuv_pixel[i + 3 + j* m_width].Y <<(10+2));
                tmp |= (yuv_pixel[i + 4 + j* m_width].U <<(20+2));
                *data_block = tmp;
                data_block++;
                tmp = 0;
                tmp |= (yuv_pixel[i + 4 + j* m_width].Y <<2);
                tmp |= (yuv_pixel[i + 4 + j* m_width].V <<(10+2));
                tmp |= (yuv_pixel[i + 5 + j* m_width].Y <<(20+2));
                *data_block = tmp;
                data_block++;

     }
   }


     file_length = m_width * m_height * 16/6;
    return file_length;
}

//return value is number of bytes to write
int CYUVConverter::prepare_yv16()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }


   //write V plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }


    //write U plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }



     file_length = m_width * m_height * 2;
    return file_length;

}


//return value is number of bytes to write
int CYUVConverter::prepare_yu16()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }


    //write U plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }


   //write V plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }

     file_length = m_width * m_height * 2;
    return file_length;

}


//return value is number of bytes to write
int CYUVConverter::prepare_nv12()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }

   //write U/V interleaved, (4:2:0)
    for (j =0;  j < m_height; j+=2) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }

     file_length = m_width * m_height * 3/2;
    return file_length;

}


//return value is number of bytes to write
int CYUVConverter::prepare_nv21()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }

   //write V/U interleaved, (4:2:0)
    for (j =0;  j < m_height; j+=2) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }

     file_length = m_width * m_height * 3/2;
    return file_length;


}

//return value is number of bytes to write
int CYUVConverter::prepare_i420()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }


    //write U plane,2x2 subsampled
    for (j =0;  j < m_height; j+=2) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }


   //write V plane, 2x2 subsampled
    for (j =0;  j < m_height; j+=2) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }
     file_length = m_width * m_height * 3/2;
    return file_length;
}



//return value is number of bytes to write
int CYUVConverter::prepare_yv12()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }

   //write V plane, 2x2 subsampled
    for (j =0;  j < m_height; j+=2) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }

    //write U plane,2x2 subsampled
    for (j =0;  j < m_height; j+=2) {
        for (i=0;i< m_width; i+=2) {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }

     file_length = m_width * m_height * 3/2;
    return file_length;

}






//return value is number of bytes to write
int CYUVConverter::prepare_yuv444()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    //write Y plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
          }
    }


    //write U plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }

   //write V plane
    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i++) {
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }

     file_length = m_width * m_height * 3;
    return file_length;

}



//return value is number of bytes to write
int CYUVConverter::prepare_uyvy()
{

    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;



    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2)
          {
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width +1].Y;
                write_pointer++;
          }
    }


     file_length = m_width * m_height * 2;
    return file_length;


    return 0;
}



int CYUVConverter::prepare_yuyv()
{

    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;



    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2)
          {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width +1].Y;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
          }
    }


     file_length = m_width * m_height * 2;
    return file_length;


    return 0;
}

int CYUVConverter::prepare_yvyu()
{

    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;


    for (j =0;  j < m_height; j++) {
        for (i=0;i< m_width; i+=2)
          {
                *write_pointer= yuv_pixel[i + j * m_width].Y;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].V;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width +1].Y;
                write_pointer++;
                *write_pointer= yuv_pixel[i + j * m_width].U;
                write_pointer++;
          }
    }

     file_length = m_width * m_height * 2;
    return file_length;


    return 0;
}



//return value is number of bytes to write
int CYUVConverter::prepare_y8me()
{
    int i, j;
    int file_length;
   CYUVPixel       *  yuv_pixel    = m_yuv_array;
   unsigned char * write_pointer  =  (unsigned char *) m_buffer;
    int tmp;
    int out_width, out_height;

    out_width  = m_width/4;
    out_height = m_height/4;
    for (j =0;  j < out_height; j++) {
        for (i=0;i < out_width; i++) {
               tmp = 0;
               tmp += yuv_pixel[i*4 + (j*4)* m_width].Y;
               tmp += yuv_pixel[i*4 + (j*4)* m_width + 1].Y;
               tmp += yuv_pixel[i*4 + (j*4)* m_width + 2].Y;
               tmp += yuv_pixel[i*4 + (j*4)* m_width + 3].Y;

               tmp += yuv_pixel[i*4 + (j*4 + 1)* m_width].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 1)* m_width + 1].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 1)* m_width + 2].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 1)* m_width + 3].Y;

               tmp += yuv_pixel[i*4 + (j*4 + 2)* m_width].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 2)* m_width + 1].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 2)* m_width + 2].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 2)* m_width + 3].Y;

               tmp += yuv_pixel[i*4 + (j*4 + 3)* m_width].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 3)* m_width + 1].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 3)* m_width + 2].Y;
               tmp += yuv_pixel[i*4 + (j*4 + 3)* m_width + 3].Y;
               *write_pointer = tmp /16;
               write_pointer++;
          }
    }

    file_length = out_width * out_height;
    return file_length;

}



static inline void yuv_2_rgb(CYUVPixel *yuv_pixel,
                      unsigned char *r,
                      unsigned char *g,
                      unsigned char *b)
{
  int red, green, blue;
  int Y = yuv_pixel->Y;
  int Cr = yuv_pixel->V;
  int Cb = yuv_pixel->U;

  //http://www.equasys.de/colorconversion.html
  //Full Range YCbCr to RGB color conversion
  red = Y + 1.4 * (Cr - 128);
  green = Y - 0.343 * (Cb - 128) - 0.711 * (Cr - 128);
  blue = Y + 1.765 * (Cb - 128);

  *r = (red < 0) ? 0 : (red >= 255) ? 255 : red;
  *g = (green < 0) ? 0 : (green >= 255) ? 255 : green;
  *b = (blue < 0) ? 0 : (blue >= 255) ? 255 : blue;
}


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

int CYUVConverter::prepare_rgb_bitmap()
{
  int i, j;
  int file_length;
  CYUVPixel * yuv_pixel = m_yuv_array;
  unsigned char * write_pointer = (unsigned char *) m_buffer;
  unsigned char r, g,  b;
  int pixel_array_offset;
  int info_header_size;
  int out_width, out_height;
  int num_color_planes = 1;
  int num_bits_per_pixel = 24;
  int pixel_resolution = 2835;

  //RGB bitmap in 24-color mode is 1:1
  out_width = m_width;
  out_height = m_height;

  //write the bitmap header
  struct BITMAP_FILE_HEADER *bitmap_file_header =
      (struct BITMAP_FILE_HEADER *) write_pointer;
  memset(bitmap_file_header, 0, sizeof(*bitmap_file_header));
  bitmap_file_header->bm[0] = 'B';
  bitmap_file_header->bm[1] = 'M';
  file_length = sizeof(BITMAP_FILE_HEADER) + out_width * out_height * 3;
  memcpy(bitmap_file_header->file_size, &file_length, 4);
  pixel_array_offset = sizeof(BITMAP_FILE_HEADER);
  memcpy(bitmap_file_header->pixel_array_offset, &pixel_array_offset, 4);
  info_header_size = 40; //always 40
  memcpy(bitmap_file_header->info_header_size, &info_header_size, 4);
  memcpy(bitmap_file_header->bitmap_width, &out_width, 4);
  memcpy(bitmap_file_header->bitmap_height, &out_height, 4);
  memcpy(bitmap_file_header->num_color_planes, &num_color_planes, 2);
  memcpy(bitmap_file_header->num_bits_per_pixel, &num_bits_per_pixel, 2);
  memcpy(bitmap_file_header->h_resolution, &pixel_resolution, 4);
  memcpy(bitmap_file_header->v_resolution, &pixel_resolution, 4);

  write_pointer += sizeof(BITMAP_FILE_HEADER);
  //write bitmap pixel array
  for (j = m_height-1; j >=0; j --) {
    for (i = 0; i < m_width; i ++) {
      yuv_2_rgb(&yuv_pixel[i + j * m_width], &r, &g, &b);
      //b, g, r is the order of 24-bit bitmap pixel array
      *write_pointer ++ = b;
      *write_pointer ++ = g;
      *write_pointer ++ = r;
    }
  }

  return file_length;
}

int CYUVConverter::get_frame_size(int yuv_format)
{
    int frame_size;

    switch(m_input_format) {
           case FMT_UYVY:
           case FMT_YVYU:
           case FMT_YUYV: //  FMT_YUY2
           case FMT_YV16:
           case FMT_YU16:
                frame_size = m_width* m_height *2;
                break;

           case FMT_I420: //FMT_IYUV
           case FMT_NV12:
           case FMT_NV21:
           case FMT_YV12:
                frame_size = m_width * m_height *3/2;
                break;

            case FMT_V210:
                frame_size = m_width *m_height * 16/6;
                break;

            case FMT_Y800:  //FMT_Y8
                frame_size = m_width * m_height;
                break;

            case FMT_YUV444:
                frame_size = m_width * m_height *3;
                break;

            case FMT_Y8_ME:
                frame_size = (m_width/4) * (m_height/4);
                break;


           default:
                printf("Error: YUV input format %d not supported \n", m_input_format);
                frame_size = -1;
                break;
        }

       return frame_size;
}


int CYUVConverter::input_to_yuv_array(FILE * fp,  int frame_size)
{
    //load file to buffer
    int ret;

    ret = fread(m_buffer, 1,  frame_size, fp);
    if (ret != frame_size) {
        printf("input yuv file %s read count %d , different to frame size %d \n",
             m_input_filename, ret, frame_size);
        return -1;
    }

    //now input yuv file is already in buffer, load from buffer into yuv array
    switch(m_input_format) {
        case FMT_V210:
            load_v210();
            break;
        case FMT_I420:  //same as IYUV
            load_i420();
            break;
        case FMT_YV12:
            load_yv12();
            break;
        case FMT_YV16:
            load_yv16();
            break;
        case FMT_YU16:
            load_yu16();
            break;
        case FMT_NV12:
            load_nv12();
            break;
        case FMT_NV21:
            load_nv21();
            break;
       case FMT_UYVY:
            load_uyvy();
            break;
       case FMT_YUYV:   //same as YUY2
            load_yuyv();
            break;
       case FMT_YVYU:
            load_yvyu();
            break;
       case FMT_YUV444:
            load_yuv444();
            break;
        default:
            printf("input YUV format %d not supported yet\n", m_input_format);
            return -1;
    }

    return 0;
}


int CYUVConverter::yuv_array_to_output(FILE * fp)
{
    int frame_length;
    int ret;

    //clear output buffer to zero , and reuse the mem buffer for output YUV file
    memset(m_buffer, 0,  m_width * m_height * 3);


    switch(m_output_format) {
        case FMT_YV16:
            frame_length = prepare_yv16();
            break;
        case FMT_UYVY:
            frame_length = prepare_uyvy();
            break;
        case FMT_YUYV:      //same as YUY2
            frame_length = prepare_yuyv();
            break;
        case FMT_YVYU:
            frame_length = prepare_yvyu();
            break;
        case FMT_YU16:
            frame_length = prepare_yu16();
            break;
        case FMT_NV12:
            frame_length = prepare_nv12();
            break;
        case FMT_NV21:
            frame_length = prepare_nv21();
            break;
        case FMT_I420:      //same as IYUV
            frame_length = prepare_i420();
            break;
        case FMT_YV12:
            frame_length = prepare_yv12();
            break;
        case FMT_YUV444:
            frame_length = prepare_yuv444();
            break;
        case FMT_V210:
            frame_length = prepare_v210();
            break;
        case FMT_Y8_ME:
            frame_length = prepare_y8me();
            break;
        case FMT_RGB_BITMAP:
            frame_length = prepare_rgb_bitmap();
            break;
        default:
            printf("output YUV format %d not supported yet \n", m_output_format);
            return -1;
    }


    ret = fwrite(m_buffer, 1, frame_length, fp);
    if (ret != frame_length) {
        printf("write yuv output file byte count %d , does not match frame length %d \n", ret, frame_length);
        return -1;
   } else {
        return 0;
    }
}



int CYUVConverter::convert(int frame_count)
{
    int i;
    FILE * fp_in;
    FILE * fp_out;
    int frame_size;

    if ((!m_input_filename)||(!m_output_filename)||(!m_input_format_set) ||(!m_output_format_set)) {
            printf("yuv converter setup not complete, cannot convert  \n");
            return -1;
    }

    if (frame_count < 1) {
            return -1;
    }


    fp_in = fopen(m_input_filename, "r");
    if (!fp_in) {
        printf("input yuv file %s cannot open \n", m_input_filename);
        return -1;
    }

    fp_out = fopen(m_output_filename, "w");
    if (!fp_out) {
        printf("output yuv file %s cannot open \n", m_output_filename);
        return -1;
    }


    frame_size = get_frame_size(m_input_format);
    if (frame_size < 0) {
            goto error_catch;
    }

    for (i =0; i< frame_count; i++) {
            //convert everything into YV16 first, and then convert from YV16 format into that format
           if (input_to_yuv_array(fp_in, frame_size) < 0) {
                printf("load input yuv error \n");
                goto error_catch;
            }

            if (yuv_array_to_output(fp_out) < 0) {
                printf("convert to output yuv error \n");
                goto error_catch;
           }
   }


    fclose(fp_in);
    fclose(fp_out);
    return 0;


error_catch:
    fclose(fp_in);
    fclose(fp_out);
    return -1;
}




#define NO_ARG		0
#define HAS_ARG		1

struct hint_s {
	const char *arg;
	const char *str;
};

static const char *short_options = "i:o:w:h:c:f:p:t";

static int input_format = 0;
static int input_format_flag = 0;
static int output_format = 0;
static int output_format_flag = 0;
static int width = 0;
static int width_flag = 0;
static int height = 0;
static int height_flag = 0;
static int repeat_count = 1;
static int repeat_count_flag = 0;
static char input_file[256];
static int input_file_flag = 0;
static char output_file[256];
static int output_file_flag = 0;
static int tips_flag = 0;


static struct option long_options[] = {
	{"input_format",  HAS_ARG, 0, 'i'},
	{"output_format",  HAS_ARG, 0, 'o'},
       {"width",  HAS_ARG, 0, 'w'},
       {"height",  HAS_ARG, 0, 'h'},
	{"count",  HAS_ARG, 0, 'c'},
	{"input_file",  HAS_ARG, 0, 'f'},
	{"output_file",  HAS_ARG, 0, 'p'},
	{"tips",  NO_ARG, 0, 't'},
	{0, 0, 0, 0}
};

static const struct hint_s hint[] = {
	{"", "  uyvy yvyu yuyv yuy2 v210 i420 iyuv nv12 nv21 yv16 \n\t\t\t    yv12 yu16 yuv444"},
       {"", "  uyvy yvyu yuyv yuy2 v210 i420 iyuv nv12 nv21 yv16 y8me\n\t\t\t    yv12 yu16 yuv444 bmp"},
       {"2~n",   "  picture width.  should be multiple of 2, v210 format\n\t\t\t    width be multiple of 6"},
       {"2~n",   "  picture height.  should be multiple of 2"},
       {"1~n",   "  how many frames to process, if not specified, use 1 \n\t\t\t    as default"},
       {"",   "\t  input yuv file name"},
       {"",   "  output yuv file name"},
       {"",   "\t  show tips about yuv format"},
};

static void usage(void)
{
	int i;
	printf("\nyuv_convert 1.1");
	printf("\nUsage:\n$yuv_convert  -i [yuv_format] -o [yuv_format]  -w [width] -h [height]  -f [input.yuv] -p [output.yuv]\n\n" );

	for (i = 0; i < sizeof(long_options) / sizeof(long_options[0]) - 1; i++) {
		if (isalpha(long_options[i].val))
			printf("-%c ", long_options[i].val);
		else
			printf("   ");
		printf("--%s", long_options[i].name);
		if (hint[i].arg[0] != 0)
			printf(" [%s]", hint[i].arg);
		printf("\t%s\n", hint[i].str);
	}



}

static void show_knowledge()
{
    printf("\n\nPACKED YUV FORMATS \n\n");

    printf("YUY2(or YUYV):\n"
              "\tInterleaved YUYV format, each U/V is 2x1 pixels subsample, \n"
              "\tFormat is like: \n"
              "\tYUYVYUYVYUYVYUYVYUYVYUYV\n"
              "\tYUYVYUYVYUYVYUYVYUYVYUYV\n"
              "\tYUYVYUYVYUYVYUYVYUYVYUYV\n"
              "\tYUYVYUYVYUYVYUYVYUYVYUYV\n");

    printf("YVYU:\n"
              "\tInterleaved YVYU format, each U/V is 2x1 pixels subsample, \n"
              "\tFormat is like: (U/V order different to YUY2) \n"
              "\tYVYUYVYUYVYUYVYUYVYUYVYU\n"
              "\tYVYUYVYUYVYUYVYUYVYUYVYU\n"
              "\tYVYUYVYUYVYUYVYUYVYUYVYU\n"
              "\tYVYUYVYUYVYUYVYUYVYUYVYU\n");


    printf("UYVY:\n"
              "\tInterleaved UYVY format, each U/V is 2x1 pixels subsample, \n"
              "\tFormat is like: \n"
              "\tUYVYUYVYUYVYUYVYUYVYUYVY\n"
              "\tUYVYUYVYUYVYUYVYUYVYUYVY\n"
              "\tUYVYUYVYUYVYUYVYUYVYUYVY\n"
              "\tUYVYUYVYUYVYUYVYUYVYUYVY\n");

    printf("V210:\n"
            "\tAJA uses this format for QuickTime on windows for uncompressed \n"
            "\tYUV in 10-bit YCbCr 4:2:2 format. Every 6 pixels are packed into\n"
            "\t4 blocks, each block is 32-bit in little endian.\n"
            "\tblock 1, bits  0 -  9: U0+1\n"
            "\tblock 1, bits 10 - 19: Y0\n"
            "\tblock 1, bits 20 - 29: V0+1\n"
            "\tblock 2, bits  0 -  9: Y1\n"
            "\tblock 2, bits 10 - 19: U2+3\n"
            "\tblock 2, bits 20 - 29: Y2\n"
            "\tblock 3, bits  0 -  9: V2+3\n"
            "\tblock 3, bits 10 - 19: Y3\n"
            "\tblock 3, bits 20 - 29: U4+5\n"
            "\tblock 4, bits  0 -  9: Y4\n"
            "\tblock 4, bits 10 - 19: V4+5\n"
            "\tblock 4, bits 20 - 29: Y5\n");


    printf("\n\nPLANAR YUV FORMATS \n\n");


    printf("IYUV(or I420):\n"
              "\tA Y plane, followed by a U plane, and a V plane, each U/V is 2x2\n"
              "\tpixels subsample, Format is like:\n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tUUUUUU\n"
                "\tUUUUUU\n"
                "\tVVVVVV\n"
                "\tVVVVVV\n");

    printf("YV12:\n"
              "\tA Y plane, followed by a V plane, and a U plane, each U/V is 2x2\n"
              "\tpixels subsample, (U/V order different to IYUV), Format is like:\n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tVVVVVV\n"
                "\tVVVVVV\n"
                "\tUUUUUU\n"
                "\tUUUUUU\n");


    printf("YV16:\n"
              "\tA Y plane, followed by a V plane, and a U plane, each U/V is 2x1\n"
              "\tpixels subsample, Format is like:\n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tVVVVVV\n"
                "\tVVVVVV\n"
                "\tVVVVVV\n"
                "\tVVVVVV\n"
                "\tUUUUUU\n"
                "\tUUUUUU\n"
                "\tUUUUUU\n"
                "\tUUUUUU\n");

    printf("YU16:\n"
              "\tA Y plane, followed by a U plane, and a V plane, each U/V is 2x1\n"
              "\tpixels subsample (U/V order different to YV16), Format is like: \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tUUUUUU\n"
                "\tUUUUUU\n"
                "\tUUUUUU\n"
                "\tUUUUUU\n"
                "\tVVVVVV\n"
                "\tVVVVVV\n"
                "\tVVVVVV\n"
                "\tVVVVVV\n");

printf("YUV444:\n"
              "\tA Y plane, followed by a U plane, and a V plane, each U/V is each\n"
              "\tpixel's U/V without subsample, Format is like: \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tUUUUUUUUUUUU\n"
                "\tUUUUUUUUUUUU\n"
                "\tUUUUUUUUUUUU\n"
                "\tUUUUUUUUUUUU\n"
                "\tVVVVVVVVVVVV\n"
                "\tVVVVVVVVVVVV\n"
                "\tVVVVVVVVVVVV\n"
                "\tVVVVVVVVVVVV\n");

    printf("NV12:\n"
              "\tA Y plane, followed by U/V interleaved, each U/V is 2x2 pixels\n"
              "\tsubsample, Format is like: \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tUVUVUVUVUVUV\n"
                "\tUVUVUVUVUVUV\n");

    printf("NV21:\n"
              "\tA Y plane, followed by V/U interleaved, each U/V is 2x2 pixels\n"
              "\tsubsample, Format is like: \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
               "\tYYYYYYYYYYYY \n"
                "\tVUVUVUVUVUVU\n"
                "\tVUVUVUVUVUVU\n");



    printf("\n\nAll YUV formats except V210 format use 8-bit for each Y/U/V channel\n"
            "V210 format uses 10-bit for Y/U/V\n");



}



int find_yuv_format(char * format_string)
{
    int yuv_format;
    if  ((!format_string)||(format_string[0]=='\0')) {
        printf("empty yuv format string \n");
        return -1;
    }

    if (!strcmp(format_string, "uyvy"))
        yuv_format=FMT_UYVY;
    else if (!strcmp(format_string, "yvyu"))
        yuv_format=FMT_YVYU;
    else if (!strcmp(format_string, "yuyv"))
        yuv_format=FMT_YUYV;
    else if (!strcmp(format_string, "yuy2"))
        yuv_format=FMT_YUY2;
    else if (!strcmp(format_string, "v210"))
        yuv_format=FMT_V210;
    else if (!strcmp(format_string, "i420"))
        yuv_format=FMT_I420;
    else if (!strcmp(format_string, "iyuv"))
        yuv_format=FMT_IYUV;
    else if (!strcmp(format_string, "nv12"))
        yuv_format=FMT_NV12;
    else if (!strcmp(format_string, "nv21"))
        yuv_format=FMT_NV21;
    else if (!strcmp(format_string, "yv16"))
        yuv_format=FMT_YV16;
    else if (!strcmp(format_string, "yv12"))
        yuv_format=FMT_YV12;
    else if (!strcmp(format_string, "y8"))
        yuv_format=FMT_Y8;
    else if (!strcmp(format_string, "y800"))
        yuv_format=FMT_Y800;
    else if (!strcmp(format_string, "yu16"))
        yuv_format=FMT_YU16;
    else if (!strcmp(format_string, "yuv444"))
        yuv_format=FMT_YUV444;
    else if (!strcmp(format_string, "y8me"))
        yuv_format=FMT_Y8_ME;
    else if (!strcmp(format_string, "bmp"))
      yuv_format = FMT_RGB_BITMAP;
    else
        yuv_format = -1;

    if (yuv_format < 0) {
         printf("yuv format %s unrecognized\n", format_string);
         return -1;
    } else {
        return yuv_format;
   }
}

static int init_param(int argc, char **argv)
{
  int ch;
  int option_index = 0;

  opterr = 0;
  while ((ch = getopt_long(argc,
                           argv,
                           short_options,
                           long_options,
                           &option_index)) != -1) {
    switch (ch) {
      case 'i':
        input_format_flag = 1;
        input_format = find_yuv_format(optarg);
        break;
      case 'o':
        output_format_flag = 1;
        output_format = find_yuv_format(optarg);
        break;
      case 'w':
        width_flag = 1;
        width = atoi(optarg);
        break;
      case 'h':
        height_flag = 1;
        height = atoi(optarg);
        break;
      case 'c':
        repeat_count_flag = 1;
        repeat_count = atoi(optarg);
        break;
      case 'f':
        input_file_flag = 1;
        strcpy(input_file, optarg);
        break;
      case 'p':
        output_file_flag = 1;
        strcpy(output_file, optarg);
        break;
      case 't':
        tips_flag = 1;
        break;
      default:
        printf("unknown option found: %c\n", ch);
        return -1;
    }
  }

  if (tips_flag)
    return 0;

  //validate the input params
  if ((!input_format_flag) || (!output_format_flag) || (input_format < 0)
      || (output_format < 0)) {
    printf("yuv format not specified correctly \n");
    return -1;
  }

  if (input_format == output_format) {
    printf("input and output format is same, no need to do conversion \n");
    return -1;
  }

  if ((!input_file_flag) || (!output_file_flag) || (input_file[0] == '\0')
      || (output_file[0] == '\0')) {
    printf("input or output file not specified \n");
    return -1;
  }

  if ((!width_flag) || (width < 2) || (width & 1) || (!height_flag)
      || (height < 2) || (height & 1)) {
    printf("image width or height setup error, w=%d, h=%d \n", width, height);
    return -1;
  }

  return 0;
}



int main(int argc, char * argv[])
{
    //get param parse

        CYUVConverter  converter;
        int ret= 0;

	if (init_param(argc, argv) < 0) {
		usage();
		return -1;
	}

        if (tips_flag) {
            show_knowledge();
            return 0;
        }



    do {
        if (converter.init() < 0) {
            printf("init failed \n");
            ret = -1;
            break;
        }

        converter.set_input_format(input_format, width, height);     //source is 10-bit YUV 4:2:2
        converter.set_output_format(output_format);    //output is 8-bit YUV, all in plane

        if (converter.set_input_filename(input_file) < 0) {
            printf("input file cannot open\n");
            ret = -4;
            break;
        }
        if (converter.set_output_filename(output_file) < 0) {
            printf("output file cannot open\n");
            ret = -5;
            break;
        }

        if (converter.convert(repeat_count) < 0) {
            printf("conversion failed\n");
            ret = -2;
            break;
        }


   } while(0);


    return ret;
}




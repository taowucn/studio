
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3;
int g_nBoxFilterValue=3;
int g_nMeanBlurValue=3;
int g_nGaussianBlurValue=3;

static void on_BoxFilter(int, void *);
static void on_MeanBlur(int, void *);
static void on_GaussianBlur(int, void *);

#define LOGO_IMAGE "../../img/1.jpg"

int main()
{
       g_srcImage= imread(LOGO_IMAGE, 1);
       if(!g_srcImage.data ) { cerr << "Read Image error" << endl; return false; }

       g_dstImage1= g_srcImage.clone();
       g_dstImage2= g_srcImage.clone();
       g_dstImage3= g_srcImage.clone();

       namedWindow("Original Image", 1);
       imshow("Original Image",g_srcImage);

       namedWindow("BoxFilter Image", 1);
       createTrackbar("Core Value:", "BoxFilter Image", &g_nBoxFilterValue, 40, on_BoxFilter);
       on_MeanBlur(g_nBoxFilterValue, 0);
       imshow("BoxFilter Image", g_dstImage1);

       namedWindow("MeanFilter Image", 1);
       createTrackbar("Core Value", "MeanFilter Image", &g_nMeanBlurValue, 40, on_MeanBlur);
       on_MeanBlur(g_nMeanBlurValue, 0);

       namedWindow("Gaussian Filter", 1);
       createTrackbar("Core Value", "Gaussian Filter", &g_nGaussianBlurValue, 40, on_GaussianBlur);
       on_GaussianBlur(g_nGaussianBlurValue,0);

       cout << "Done" << endl;
       while (char(waitKey(1))!= 'q') {}
       return 0;
}

static void on_BoxFilter(int, void *)
{
	boxFilter(g_srcImage, g_dstImage1, -1, Size(g_nBoxFilterValue+1, g_nBoxFilterValue+1),
			Point(-1,-1), false, BORDER_DEFAULT);
	imshow("BoxFilter Image", g_dstImage1);
}

static void on_MeanBlur(int, void *)
{
	blur(g_srcImage, g_dstImage2, Size( g_nMeanBlurValue+1, g_nMeanBlurValue+1), Point(-1,-1));
	imshow("MeanFilter Image", g_dstImage2);
}

static void on_GaussianBlur(int, void *)
{
	GaussianBlur(g_srcImage, g_dstImage3, Size(g_nGaussianBlurValue*2+1, g_nGaussianBlurValue*2+1 ), 0, 0);
	imshow("Gaussian Filter", g_dstImage3);
}

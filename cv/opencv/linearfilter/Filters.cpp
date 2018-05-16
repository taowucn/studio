
#include <opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5;
int g_nBoxFilterValue=6;  //方框滤波内核值
int g_nMeanBlurValue=10;  //均值滤波内核值
int g_nGaussianBlurValue=6;  //高斯滤波内核值
int g_nMedianBlurValue=10;  //中值滤波参数值
int g_nBilateralFilterValue=10;  //双边滤波参数值

static void on_BoxFilter(int, void *);
static void on_MeanBlur(int, void *);
static void on_GaussianBlur(int, void *);
static void on_MedianBlur(int, void *);
static void on_BilateralFilter(int, void*);

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

	namedWindow("GaussianFilter Image", 1);
	createTrackbar("Core Value", "GaussianFilter Image", &g_nGaussianBlurValue, 40, on_GaussianBlur);
	on_GaussianBlur(g_nGaussianBlurValue,0);

	namedWindow("MedianFilter Image", 1);
	createTrackbar("Core Value", "MedianFilter Image",&g_nMedianBlurValue, 50,on_MedianBlur );
	on_MedianBlur(g_nMedianBlurValue,0);

	namedWindow("BilateralFilter Image", 1);
	createTrackbar("Core value", "BilateralFilter Image",&g_nBilateralFilterValue, 50,on_BilateralFilter);
	on_BilateralFilter(g_nBilateralFilterValue,0);

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
	imshow("GaussianFilter Image", g_dstImage3);
}

static void on_MedianBlur(int, void *)
{
	medianBlur( g_srcImage, g_dstImage4, g_nMedianBlurValue*2+1 );
	imshow("MedianFilter Image", g_dstImage4);
}

static void on_BilateralFilter(int, void *)
{
	bilateralFilter( g_srcImage, g_dstImage5, g_nBilateralFilterValue, g_nBilateralFilterValue*2,g_nBilateralFilterValue/2 );
	imshow("BilateralFilter Image", g_dstImage5);
}

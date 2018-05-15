#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

static void ContrastAndBright(int, void *);

int g_nContrastValue;
int g_nBrightValue;
Mat g_srcImage, g_dstImage;

#define BG_IMAGE "../../img/2.jpg"

int main(  )
{
	g_srcImage = imread(BG_IMAGE);
	if (!g_srcImage.data ) {
		cout << "Read Image error" <<endl;
		return -1;
	}
	g_dstImage = Mat::zeros( g_srcImage.size(), g_srcImage.type());
	g_nContrastValue=80;
	g_nBrightValue=80;

	namedWindow("After", 1);
	createTrackbar("Contract", "After",&g_nContrastValue, 300, ContrastAndBright);
	createTrackbar("Bright","After",&g_nBrightValue, 200, ContrastAndBright);

	ContrastAndBright(g_nContrastValue,0);
	ContrastAndBright(g_nBrightValue,0);

	cout << "Please change the trackerbar, enter 'q' for exit\n" << endl;
	while(char(waitKey(1)) != 'q') {}
	return 0;
}

static void ContrastAndBright(int, void *)
{
	namedWindow("Original", 1);

	for(int y = 0; y < g_srcImage.rows; y++ ) {
		for(int x = 0; x < g_srcImage.cols; x++ ) {
			for(int c = 0; c < 3; c++ ) {
			g_dstImage.at<Vec3b>(y,x)[c]= saturate_cast<uchar>((g_nContrastValue*0.01)*
					(g_srcImage.at<Vec3b>(y,x)[c] ) + g_nBrightValue );
			}
		}
	}

	imshow("Original", g_srcImage);
	imshow("After", g_dstImage);
}

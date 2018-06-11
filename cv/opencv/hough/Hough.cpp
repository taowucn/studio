#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage, g_midImage;
vector<Vec4i> g_lines;

int g_nthreshold=100;

static void on_HoughLines(int, void*);
static void ShowHelpText();

#define IMAGE_NAME "../../img/5.jpg"

int main( )
{
	Mat g_srcImage = imread(IMAGE_NAME);

	imshow("Original Image", g_srcImage);

	namedWindow("PostProcess",1);
	createTrackbar("Value", "PostProcess", &g_nthreshold, 200, on_HoughLines);

	Canny(g_srcImage, g_midImage, 50, 200, 3);
	cvtColor(g_midImage,g_dstImage, CV_GRAY2BGR);

	on_HoughLines(g_nthreshold,0);
	HoughLinesP(g_midImage, g_lines, 1, CV_PI/180, 80, 50, 10 );

	imshow("PostProcess", g_dstImage);
	waitKey(0);
	return 0;
}

static void on_HoughLines(int, void*)
{
	Mat dstImage = g_dstImage.clone();
	Mat midImage = g_midImage.clone();

	vector<Vec4i> mylines;
	HoughLinesP(midImage, mylines, 1, CV_PI/180, g_nthreshold+1, 50, 10 );

	for(size_t i = 0; i < mylines.size(); i++ ) {
		Vec4i l = mylines[i];
		line( dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(23,180,55), 1, CV_AA);
	}

	imshow("PostProcess",dstImage);
}

static void ShowHelpText()
{
	cout << "Done" << endl;
}

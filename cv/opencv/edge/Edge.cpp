#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat g_srcImage, g_srcGrayImage,g_dstImage;

//Canny
Mat g_cannyDetectedEdges;
int g_cannyLowThreshold=1;

//Sobel
Mat g_sobelGradient_X, g_sobelGradient_Y;
Mat g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
int g_sobelKernelSize=1;

//Scharr
Mat g_scharrGradient_X, g_scharrGradient_Y;
Mat g_scharrAbsGradient_X, g_scharrAbsGradient_Y;

static void ShowHelpText( );
static void on_Canny(int, void*);
static void on_Sobel(int, void*);
void Scharr( );

#define LOGO_IMAGE "../../img/1.jpg"

int main( int argc, char** argv )
{
	ShowHelpText();

	g_srcImage = imread(LOGO_IMAGE);
	if (!g_srcImage.data) {
		cerr << "Load image error\n" << endl;
		return -1;
	}

	namedWindow("Orignal Image");
	imshow("Orignal Image", g_srcImage);

	g_dstImage.create( g_srcImage.size(), g_srcImage.type() );
	cvtColor( g_srcImage, g_srcGrayImage, CV_BGR2GRAY );

	namedWindow("Canny", CV_WINDOW_AUTOSIZE );
	namedWindow("Sobel", CV_WINDOW_AUTOSIZE );

	createTrackbar( "Value", "Canny", &g_cannyLowThreshold, 120, on_Canny );
	createTrackbar( "Value", "Sobel", &g_sobelKernelSize, 3, on_Sobel );

	on_Canny(0, 0);
	on_Sobel(0, 0);
	Scharr( );
	while((char(waitKey(1)) != 'q')) {}
	return 0;
}

static void ShowHelpText()
{
	cout << "Quit when type q" << endl;
}

void on_Canny(int, void*)
{
	//Denoise with 3x3 kernel
	blur( g_srcGrayImage, g_cannyDetectedEdges, Size(3,3) );

	//Run with Canny operator
	Canny( g_cannyDetectedEdges, g_cannyDetectedEdges, g_cannyLowThreshold, g_cannyLowThreshold*3, 3 );

	//set all element in g_dstImage in zero
	g_dstImage = Scalar::all(0);

	//Use g_cannyDetectedEdges mask，Copy g_srcImage to g_dstImage
	g_srcImage.copyTo( g_dstImage, g_cannyDetectedEdges);

	imshow( "Canny", g_dstImage );
}

void on_Sobel(int, void*)
{
	// X
	Sobel( g_srcImage, g_sobelGradient_X, CV_16S, 1, 0, (2*g_sobelKernelSize+1), 1, 1, BORDER_DEFAULT );
	convertScaleAbs( g_sobelGradient_X, g_sobelAbsGradient_X );

	//Y
	Sobel( g_srcImage, g_sobelGradient_Y, CV_16S, 0, 1, (2*g_sobelKernelSize+1), 1, 1, BORDER_DEFAULT );
	convertScaleAbs( g_sobelGradient_Y, g_sobelAbsGradient_Y );

	//Combine
	addWeighted( g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, g_dstImage );

	imshow("Sobel", g_dstImage);
}

void Scharr()
{
	//X
	Scharr( g_srcImage, g_scharrGradient_X, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT );
	convertScaleAbs( g_scharrGradient_X, g_scharrAbsGradient_X );
	//Y
	Scharr( g_srcImage, g_scharrGradient_Y, CV_16S, 0, 1, 1, 0, BORDER_DEFAULT );
	convertScaleAbs( g_scharrGradient_Y, g_scharrAbsGradient_Y );
	//Combine
	addWeighted( g_scharrAbsGradient_X, 0.5, g_scharrAbsGradient_Y, 0.5, 0, g_dstImage );

	imshow("Scharr", g_dstImage);
}


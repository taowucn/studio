#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

#define WINDOW_NAME "Pyramid Show"

using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage, g_tmpImage;

#define LOGO_IMAGE "../../img/1.jpg"

static void ShowHelpText();

static void ShowHelpText()
{
	cout << "Usage: Input 1, 2, 3, 4" << endl;
}

int main( )
{
	ShowHelpText();

	g_srcImage = imread(LOGO_IMAGE);
	if (!g_srcImage.data) {
		cout << "Load Image err" << endl;
		return -1;
	}

	namedWindow( WINDOW_NAME, CV_WINDOW_AUTOSIZE );
	imshow(WINDOW_NAME, g_srcImage);

	g_tmpImage = g_srcImage;
	g_dstImage = g_tmpImage;

	int key = 0;

	while (1) {
		key = waitKey(3000);
		//cout << "key: " << key << endl;
		switch ((char)key) {
		case 27://ESC
		case 'q'://q
			return 0;
			break;
		case 'a'://pyrUp
		case '3': //pyrUp
			pyrUp( g_tmpImage, g_dstImage, Size( g_tmpImage.cols*2, g_tmpImage.rows*2 ) );
			cout << ">a: pyrUp -> size ×2" << endl;
			break;
		case 'w'://resize
		case '1'://Resize
			resize(g_tmpImage,g_dstImage,Size( g_tmpImage.cols*2, g_tmpImage.rows*2 ));
			cout << "1: resize -> size×2" << endl;
			break;
		case 's' ://Resize
		case '2'://resize
			resize(g_tmpImage,g_dstImage,Size( g_tmpImage.cols/2, g_tmpImage.rows/2 ),(0,0),(0,0),2);
			cout << "2: resize -> size/2" << endl;
			break;
		case 'd':
		case '4': //pyrDown
			pyrDown( g_tmpImage, g_dstImage, Size( g_tmpImage.cols/2, g_tmpImage.rows/2 ) );
			cout << "4: pyrDown -> size/2" << endl;
			break;
		default:
			cout << "Input unknown key: " << (char )key << endl;
			break;
		}
		imshow(WINDOW_NAME, g_dstImage);
		g_tmpImage = g_dstImage;
	}

	return 0;
}

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat g_srcImage, g_dstImage, g_grayImage, g_maskImage;
int g_nFillMode = 1;
int g_nLowDifference = 20, g_nUpDifference = 20;
int g_nConnectivity = 4;
int g_bIsColor = true;
bool g_bUseMask = false;
int g_nNewMaskVal = 255;

#define IMAGE_NAME "../../img/5.jpg"

static void ShowHelpText()
{
	cout << "Usage: \n"
		"ESC- Quit\n"
		"1: - Switch Chroma/Color\n"
		"2: - Show/Hidden Mask window\n"
		"3: - Recovery original image\n"
		"4: - Enable FloodFill with empty scope\n"
		"5: - Enable FloodFill with gradual change/ fixed scope\n"
		"6: - Enable FloodFill with gradual change/ float scope\n"
		"7: - The Low 4bit of operator identifier use connectivity mode\n"
		"8: - The Low 4bit of operator identifier use connectivity mode\n"
		<< endl;
}

static void onMouse( int event, int x, int y, int, void* )
{
	if( event != CV_EVENT_LBUTTONDOWN )
		return;

	Point seed = Point(x,y);
	int LowDifference = g_nFillMode == 0 ? 0 : g_nLowDifference;
	int UpDifference = g_nFillMode == 0 ? 0 : g_nUpDifference;
	int flags = g_nConnectivity + (g_nNewMaskVal << 8) +
		(g_nFillMode == 1 ? CV_FLOODFILL_FIXED_RANGE : 0);
	//0~7 bit of g_nConnectivity，8~15 bit is g_nNewMaskVal left shift 8，
	//16~23 bit is CV_FLOODFILL_FIXED_RANGE or zero。

	//Get bgr randomly
	int b = (unsigned)theRNG() & 255;
	int g = (unsigned)theRNG() & 255;
	int r = (unsigned)theRNG() & 255;
	Rect ccomp; //Define the min edage rect range of repaint area

	Scalar newVal = g_bIsColor ? Scalar(b, g, r) : Scalar(r*0.299 + g*0.587 + b*0.114);
	Mat dst = g_bIsColor ? g_dstImage : g_grayImage;
	int area;

	if (g_bUseMask) {
		threshold(g_maskImage, g_maskImage, 1, 128, CV_THRESH_BINARY);
		area = floodFill(dst, g_maskImage, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference),
			Scalar(UpDifference, UpDifference, UpDifference), flags);
		imshow( "mask", g_maskImage );
	} else {
		area = floodFill(dst, seed, newVal, &ccomp, Scalar(LowDifference, LowDifference, LowDifference),
			Scalar(UpDifference, UpDifference, UpDifference), flags);
	}

	imshow("Processed", dst);
	cout << area << " Pixels\n";
}

int main(int argc, char *argv[])
{
	g_srcImage = imread(IMAGE_NAME, 1);
    if( !g_srcImage.data ) { cout << "Load Image err" << endl; return -1; }
    ShowHelpText();

	g_srcImage.copyTo(g_dstImage);
	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	g_maskImage.create(g_srcImage.rows+2, g_srcImage.cols+2, CV_8UC1);

	namedWindow("Processed" , CV_WINDOW_AUTOSIZE );

	createTrackbar("Max value of Negative", "Processed", &g_nLowDifference, 255, 0 );
	createTrackbar("Max value of Postive" ,"Processed", &g_nUpDifference, 255, 0 );

	setMouseCallback( "Processed", onMouse, 0 );

	while(1) {
		imshow("Processed", g_bIsColor ? g_dstImage : g_grayImage);

		int c = waitKey(0);
		if ( ((char)c & 255) == 27 ) {
			cout << "Quit Program" << endl;
			break;
		}

		switch((char)c) {

		case '1':
			if (g_bIsColor) {
				cout << "键盘“1”被按下，切换彩色/灰度模式，当前操作为将【彩色模式】切换为【灰度模式】\n";
				cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
				g_maskImage = Scalar::all(0); //将mask所有元素设置为0
				g_bIsColor = false;	//将标识符置为false，表示当前图像不为彩色，而是灰度
			} else {
				cout << "键盘“1”被按下，切换彩色/灰度模式，当前操作为将【彩色模式】切换为【灰度模式】\n";
				g_srcImage.copyTo(g_dstImage);
				g_maskImage = Scalar::all(0);
				g_bIsColor = true;
			}
			break;

		case '2':
			if (g_bUseMask) {
				destroyWindow( "mask" );
				g_bUseMask = false;
			} else {
				namedWindow( "mask", 0 );
				g_maskImage = Scalar::all(0);
				imshow("mask", g_maskImage);
				g_bUseMask = true;
			}
			break;

		case '3':
			cout << "按键“3”被按下，恢复原始图像\n";
			g_srcImage.copyTo(g_dstImage);
			cvtColor(g_dstImage, g_grayImage, COLOR_BGR2GRAY);
			g_maskImage = Scalar::all(0);
			break;

		case '4':
			cout << "按键“4”被按下，使用空范围的漫水填充\n";
			g_nFillMode = 0;
			break;

		case '5':
			cout << "按键“5”被按下，使用渐变、固定范围的漫水填充\n";
			g_nFillMode = 1;
			break;

		case '6':
			cout << "按键“6”被按下，使用渐变、浮动范围的漫水填充\n";
			g_nFillMode = 2;
			break;

		case '7':
			cout << "按键“7”被按下，操作标志符的低八位使用4位的连接模式\n";
			g_nConnectivity = 4;
			break;

		case '8':
			cout << "按键“8”被按下，操作标志符的低八位使用8位的连接模式\n";
			g_nConnectivity = 8;
			break;
		}
	}

	return 0;
}

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat g_srcImage, g_dstImage;
int g_nTrackbarNumer = 0;
int g_nStructElementSize = 3;

void Process();
void on_TrackbarNumChange(int, void *);
void on_ElementSizeChange(int, void *);

#define LOGO_IMAGE "../../img/1.jpg"

int main( )
{
	g_srcImage= imread(LOGO_IMAGE);
	if (!g_srcImage.data ) { cerr << "Read Image error" << endl; return false; }

	namedWindow("Original Image");
	imshow("Original Image", g_srcImage);

	namedWindow("Processed Image");
	Mat element = getStructuringElement(MORPH_RECT, Size(2*g_nStructElementSize+1,
			2*g_nStructElementSize+1),Point( g_nStructElementSize, g_nStructElementSize ));
	erode(g_srcImage,g_dstImage, element);
	imshow("Processed Image", g_dstImage);

	createTrackbar("Morph", "Processed Image", &g_nTrackbarNumer, 1, on_TrackbarNumChange);
	createTrackbar("Kerenl Size", "Processed Image",&g_nStructElementSize, 21, on_ElementSizeChange);

	cout << "Done" << endl;

	while(char(waitKey(1))!= 'q') {}

	return 0;
}

void Process()
{
	Mat element = getStructuringElement(MORPH_RECT, Size(2*g_nStructElementSize+1,2*g_nStructElementSize+1),Point( g_nStructElementSize, g_nStructElementSize ));

	if (g_nTrackbarNumer== 0) {
		erode(g_srcImage,g_dstImage, element);
	} else {
		dilate(g_srcImage,g_dstImage, element);
	}

	imshow("Processed Image", g_dstImage);
}

void on_TrackbarNumChange(int, void *)
{
	Process();
}

void on_ElementSizeChange(int, void *)
{
	Process();
}

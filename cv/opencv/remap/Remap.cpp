#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define IMAGE_NAME "../../img/1.jpg"

int main(  )
{
	Mat srcImage, dstImage;
	Mat map_x, map_y;

	srcImage = imread(IMAGE_NAME, 1 );
	if (!srcImage.data ) {
		cerr << "Load Image err" << endl;
		return -1;
	}
	imshow("Original Image", srcImage);

	dstImage.create( srcImage.size(), srcImage.type() );
	map_x.create( srcImage.size(), CV_32FC1 );
	map_y.create( srcImage.size(), CV_32FC1 );

	for( int j = 0; j < srcImage.rows;j++) {
		for( int i = 0; i < srcImage.cols;i++) {
			map_x.at<float>(j,i) = static_cast<float>(i);
			map_y.at<float>(j,i) = static_cast<float>(srcImage.rows - j);
		}
	}
	remap( srcImage, dstImage, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0) );

	imshow( "Processed", dstImage);
	waitKey(0);
	return 0;
}


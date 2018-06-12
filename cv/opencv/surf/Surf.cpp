#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>

using namespace std;
using namespace cv;
static void ShowHelpText( );

#define LOGO_IMAGE1 "../../img/1.jpg"
#define LOGO_IMAGE2 "../../img/3.jpg"

int main( int argc, char** argv )
{
	ShowHelpText( );

	Mat srcImage1 = imread(LOGO_IMAGE1, 1 );
	Mat srcImage2 = imread(LOGO_IMAGE2, 1 );
	if( !srcImage1.data || !srcImage2.data ) {
		cerr << "Load Image err" << endl;
		return -1;
	}
	imshow("Original Image 1", srcImage1);
	imshow("Original Image 2", srcImage2);

	int minHessian = 400; //define the operator of SURF's hessian
	SurfFeatureDetector detector( minHessian );
	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector.detect( srcImage1, keypoints_1 );
	detector.detect( srcImage2, keypoints_2 );

	Mat img_keypoints_1; Mat img_keypoints_2;
	drawKeypoints( srcImage1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
	drawKeypoints( srcImage2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

	imshow("Char Point 1", img_keypoints_1 );
	imshow("Char Point 2", img_keypoints_2 );

	waitKey(0);
	return 0;
}

void ShowHelpText()
{
	cout << "Done" << endl;
}

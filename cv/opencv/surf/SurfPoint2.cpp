#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include <iostream>

using namespace cv;
using namespace std;
static void ShowHelpText( );

#define LOGO_IMAGE1 "../../img/1.jpg"
#define LOGO_IMAGE2 "../../img/3.jpg"

int main(  )
{
	ShowHelpText( );

	Mat srcImage1 = imread(LOGO_IMAGE1, 1);
	Mat srcImage2 = imread(LOGO_IMAGE2, 1);
	if( !srcImage1.data || !srcImage2.data ) {
		cerr << "Load Image err" << endl;
		return -1;
	}

	//use SURF detect char point
	int minHessian = 700;
	SurfFeatureDetector detector( minHessian );
	std::vector<KeyPoint> keyPoint1, keyPoints2;

	detector.detect( srcImage1, keyPoint1 );
	detector.detect( srcImage2, keyPoints2 );

	SurfDescriptorExtractor extractor;
	Mat descriptors1, descriptors2;
	extractor.compute( srcImage1, keyPoint1, descriptors1 );
	extractor.compute( srcImage2, keyPoints2, descriptors2 );

	BruteForceMatcher< L2<float> > matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors1, descriptors2, matches );

	Mat imgMatches;
	drawMatches( srcImage1, keyPoint1, srcImage2, keyPoints2, matches, imgMatches );
	imshow("Matched Map", imgMatches );

	waitKey(0);
	return 0;
}

static void ShowHelpText()
{
	cout << "Done" << endl;
}

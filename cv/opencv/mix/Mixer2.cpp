#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

bool ROI_AddImage();
bool ROI_LinearBlending();

#define BG_IMAGE "../../img/2.jpg"
#define LOGO_IMAGE "../../img/1.jpg"

int main(  )
{
	if (ROI_AddImage() && ROI_LinearBlending()) {
		cout <<"Got mixed image" << endl;
	}
	waitKey(0);
	return 0;
}

bool ROI_AddImage()
{
	Mat srcImage1= imread(BG_IMAGE);
	Mat logoImage= imread(LOGO_IMAGE);
	if(!srcImage1.data ) { printf("Read BG Image error\n"); return false; }
	if(!logoImage.data ) { printf("Read Logo Image error\n"); return false; }

	Mat imageROI= srcImage1(Rect(10,10, logoImage.cols, logoImage.rows));
	Mat mask= imread(LOGO_IMAGE, 0);
	logoImage.copyTo(imageROI, mask);

	namedWindow("1. Mixer with ROI");
	imshow("1. Mixer with ROI",srcImage1);

	return true;
}

bool ROI_LinearBlending()
{
	Mat srcImage4= imread(BG_IMAGE,1);
	Mat logoImage= imread(LOGO_IMAGE);

	if(!srcImage4.data ) { printf("Read BG Image error\n"); return false; }
	if(!logoImage.data ) { printf("Read Logo Image error\n"); return false; }

	Mat imageROI;
	imageROI=srcImage4(Rect(10,10,logoImage.cols,logoImage.rows));
	addWeighted(imageROI,0.5, logoImage, 0.3, 0., imageROI);

	namedWindow("2. Mixer with Linear");
	imshow("2. Mixer with Linear", srcImage4);

	return true;
}

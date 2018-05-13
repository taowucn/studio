#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

#define BG_IMAGE "../../img/2.jpg"
#define LOGO_IMAGE "../../img/1.jpg"

bool MultiChannelBlending();

int main()
{
	if (MultiChannelBlending()) {
		cout<<"Done"<<endl;
	}
	waitKey(0);
	return 0;
}

bool MultiChannelBlending()
{
	Mat srcImage;
	Mat logoImage;
	vector<Mat>channels;
	Mat  imageBlueChannel;

	logoImage=imread(LOGO_IMAGE, 0);
	srcImage=imread(BG_IMAGE);
	if (!logoImage.data ) { printf("Read logo image error\n"); return false;}
	if (!srcImage.data ) { printf("Read BG image error\n"); return false;}

	split(srcImage, channels);
	imageBlueChannel = channels.at(0);
	addWeighted(imageBlueChannel(Rect(10,10,logoImage.cols,logoImage.rows)), 0.5,
		  logoImage, 0.5, 0, imageBlueChannel(Rect(10,10,logoImage.cols,logoImage.rows)));
	merge(channels, srcImage);
	namedWindow("1. Original + Logo with Blue channel");
	imshow("1. Original + Logo with Blue channel", srcImage);

	Mat  imageGreenChannel;
	logoImage=imread(LOGO_IMAGE,0);
	srcImage=imread(BG_IMAGE);
	if (!logoImage.data ) { printf("Read logo image error\n"); return false;}
	if (!srcImage.data ) { printf("Read BG image error\n"); return false;}
	split(srcImage,channels);
	imageGreenChannel=channels.at(1);
	addWeighted(imageGreenChannel(Rect(10,10,logoImage.cols,logoImage.rows)),0.5,
		  logoImage,0.5,0.,imageGreenChannel(Rect(10,10,logoImage.cols,logoImage.rows)));
	merge(channels,srcImage);
	namedWindow("2. Original + Logo with Green channel");
	imshow("2. Original + Logo with Green channel",srcImage);

	Mat  imageRedChannel;
	logoImage=imread(LOGO_IMAGE,0);
	srcImage=imread(BG_IMAGE);
	if (!logoImage.data ) { printf("Read logo image error\n"); return false;}
	if (!srcImage.data ) { printf("Read BG image error\n"); return false;}
	split(srcImage,channels);
	imageRedChannel=channels.at(2);
	addWeighted(imageRedChannel(Rect(10,10,logoImage.cols,logoImage.rows)),0.5,
		  logoImage,0.5,0.,imageRedChannel(Rect(10,10,logoImage.cols,logoImage.rows)));
	merge(channels,srcImage);
	namedWindow("3. Original + Logo with Red channel");
	imshow("3. Original + Logo with Red channel",srcImage);

	return true;
}

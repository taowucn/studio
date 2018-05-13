#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char **argv)
{
	Mat image = imread("../../img/2.jpg", 199);
	Mat logo = imread("../../img/1.jpg");
	Mat imageROI;

	imageROI = image(Rect(10, 10, logo.cols, logo.rows));
	//imageROI = image(Range(150,150+logo.rows),Range(200,200+logo.cols));
	addWeighted(imageROI, 0.5, logo, 0.3, 0., imageROI);
	//namedWindow("Mixer out image");
	imshow("Mixer out image",image);
	imwrite("mixer_output.jpg",image);
	waitKey(0);
	return 0;
}

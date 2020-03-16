#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s <input.jpg>\n", argv[0]);
		return 0;
	}
	printf("Input Image: %s\n", argv[0]);
	Mat src_img = imread(argv[0]);
	if (src_img.empty()) {
		printf("could not load the image...\n");
		return -1;
	}

	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	imshow("Original", src_img);

	Mat gray_img;
	cvtColor(src_img, gray_img, COLOR_BGR2GRAY);
	namedWindow("Gray", CV_WINDOW_AUTOSIZE);
	imshow("Gray", gray_img);

	Mat sum_img = Mat::zeros(gray_img.rows + 1, gray_img.cols + 1, CV_32FC1);
	Mat sqsum_img = Mat::zeros(gray_img.rows + 1, gray_img.cols + 1, CV_64FC1);
	integral(gray_img, sum_img, sqsum_img);

	Mat result_img;
	normalize(sum_img, result_img, 0, 255, NORM_MINMAX, CV_8UC1, Mat());
	imshow("Integral-Image", result_img);

	waitKey(0);
	return 0;
}

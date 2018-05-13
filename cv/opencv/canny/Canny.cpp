#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//#define IMG_FROM_CAMERA 1

int main(int argc, char**argv)
{

#if IMG_FROM_CAMERA
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
#else
    Mat frame;
    if (argc != 2) {
	cout << "Please Insert Image Path" << endl;
	return -1;
    }
    frame = imread(argv[1], CV_LOAD_IMAGE_COLOR );
#endif

    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {

#if IMG_FROM_CAMERA
        Mat frame;
        cap >> frame; // get a new frame from camera
#endif

        cvtColor(frame, edges, CV_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);

        imshow("edges", edges);
        waitKey(0);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

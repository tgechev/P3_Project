/* https://docs.opencv.org/3.2.0/d3/d05/tutorial_py_table_of_contents_contours.html
We should most probably read the stuff from this link because everything in this code is from there.

*/


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;


static void help()
{
	cout
		<< "\nThis program illustrates the use of findContours and drawContours\n"
		<< "The original image is put up along with the image of drawn contours\n"
		<< "Usage:\n"
		<< "./contours2\n"
		<< "\nA trackbar is put up which controls the contour level from -3 to 3\n"
		<< endl;
}

const int w = 500;


vector<vector<Point> > contours; //this should more or less store all detected blobs

vector<Vec4i> hierarchy; //cannot quite explain what this does but it is necessary


int main(int argc, char**)
{
	if (argc > 1)
	{
		help();
		return -1;
	}

	VideoCapture stream1(0); //0 is the id of video device.0 if you have only one
	//camera.
		if (!stream1.isOpened()) { //check if video device has been initialised
			cout << "cannot open camera";
		}

		//unconditional loop
		while (true) {

			Mat cameraFrame;					//matrix for the color frame
			Mat thresholdedFrame;				//matrix for the thresholded frame
			stream1.read(cameraFrame);			//read frame from camera

			cvtColor(cameraFrame, thresholdedFrame, CV_BGR2GRAY);		//convert color frame to grayscale

			threshold(thresholdedFrame, thresholdedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale image

			imshow("cam", cameraFrame);

			
			vector<vector<Point> > contours0;  //auxiliary object for resizing blobs -ish

			findContours(thresholdedFrame, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); //find contours/blobs

			contours.resize(contours0.size());
			for (size_t k = 0; k < contours0.size(); k++)				//simplify blobs -ish
				approxPolyDP(Mat(contours0[k]), contours[k], 3, true);
				
			namedWindow("contours", 1);

			Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);  //new matrix with drawn blobs in it

			int idx = 0;

			for (; idx >= 0; idx = hierarchy[idx][0])
			{
				Scalar color(128, 255, 255);
				drawContours(cnt_img, contours, idx, color, -1, 8, hierarchy);  //draw blobs in the cnt_img matrix
			}

			imshow("contours", cnt_img);							//show image with blobs
			cout << "Number of blobs: " << contours.size() << endl;		//display number of blobs
			if (waitKey(30) >= 0)
				break;
		}

	return 0;
}
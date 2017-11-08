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

			Mat cameraFrame; 

			Mat thresholdedFrame;
			
			stream1.read(cameraFrame);

			Rect roi(0, cameraFrame.rows / 3, cameraFrame.cols, cameraFrame.rows / 3);  //define region of interest (roi) rectangle

			cvtColor(cameraFrame, thresholdedFrame, CV_BGR2GRAY);  //convert frame to grayscale

			medianBlur(thresholdedFrame, thresholdedFrame, 5);     //blur the grayscale frame to remove noise as much as possible

			threshold(thresholdedFrame, thresholdedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

			Mat roiMatrix(thresholdedFrame, roi);  //define roi matrix for blob detection


			vector<vector<Point> > contours0;

			findContours(roiMatrix, contours0, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);  //find blobs in the roiMatrix

			contours.resize(contours0.size());
			for (size_t k = 0; k < contours0.size(); k++)
				//approximation of blob
				approxPolyDP(Mat(contours0[k]), contours[k], 2, true);
				
			namedWindow("contours", 1);

			Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection

			/*  Sorting blobs
			for (int i = 0; i < contours.size() - 1; i++) {
				int j = i + 1;
				while (j > 0) {
					if (contours[j - 1].size() < contours[j].size()) {
						vector<Point> temp = contours[j - 1];
						contours[j - 1] = contours[j];
						contours[j] = temp;
					}
					j--;
				}
			}*/

			//int idx = 0;

			//for (; idx >= 0; idx = hierarchy[idx][0])
			//{
				Scalar color(188, 255, 255);
				drawContours(cnt_img, contours, -1, color, -1, 8, hierarchy, 2, Point(0, cameraFrame.rows/3));          //draw found blobs in cnt_img matrix
				
				//draws a bounding box/rectangle around a blob - (contours[idx])
				
				//draw bounding box for smallest blob (contours[contours.size()-1]) | for biggest blob (contours[0])
				//rectangle(cameraFrame, roi /*boundingRect(contours[contours.size()-1])*/, Scalar(255, 0, 0), 2);
			//}

			rectangle(cameraFrame, roi /*boundingRect(contours[contours.size()-1])*/, Scalar(255, 0, 0), 2);   //draw rectangle in the color frame showing the roi
			
			imshow("cam", cameraFrame);           //show color frame

			//imshow("blur", thresholdedFrame);

			imshow("contours", cnt_img);        //show blob detection frame

			cout << "Number of blobs: " << contours.size() /*<< " contours0[0].size: " << contours[0].size()*/ << endl;


			//area of blob | perimeter of blob
			//cout<<"contourArea(contours[0]): "<<contourArea(contours[0])<<" arcLength(contours[0],true): "<< arcLength(contours[0], true)<<endl;
			if (waitKey(30) >= 0)
				break;
		}

	return 0;
}
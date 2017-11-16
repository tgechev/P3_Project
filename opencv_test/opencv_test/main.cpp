/* https://docs.opencv.org/3.2.0/d3/d05/tutorial_py_table_of_contents_contours.html
We should most probably read the stuff from this link because everything in this code is from there.
NOTE TO SELF: IF you get cvtColor() related error when trying to run the code check from which camera you are trying to read the frame!!!

TODO: TEST TEMPLATE MATCHING CODE WITH IMPLEMENTED THREAD.
*/

#define _USE_MATH_DEFINES

#include "Camera.h"
#include <math.h>
#include <iostream>
#include<thread>


using namespace cv;
using namespace std;

vector<vector<Point> > contours; //this should more or less store all detected blobs


void findMatch(Mat sourceImg, Mat templ)
{
	int match_method = 3;
	/// Source image to display
	Mat img_display, result;
	sourceImg.copyTo(img_display);

	/// Create the result matrix
	int result_cols = sourceImg.cols - templ.cols + 1;
	int result_rows = sourceImg.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_8UC1);

	/// Do the Matching
	matchTemplate(sourceImg, templ, result, match_method);
	//normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	//cout << "minVal: " << minVal << endl;
	//cout << "maxVal: " << maxVal << endl;

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	//cout << "matchLoc: " << matchLoc << endl;
	//cout << "maxLoc: " << maxLoc << endl;
	/// Show me what you got
	if (maxVal > 0.8) {
		rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(255), 2, 8, 0);
		//rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	}

	imshow("template_matching", img_display);

	//imshow(result_window, result);
	//return;
}

void detectTemplate(Camera* myCamera) {
	Mat camFrame, threshFrame, templ;
	//vector<vector<Point> > firstCardSlotBlobs;
	vector<string> templates;


	templ = imread("templates/card6_tmpl2.jpg", IMREAD_GRAYSCALE);
	int i = 999;
	while (i > 0) {

		camFrame = myCamera->getCameraFrame();

		//Rect firstCardROI(10, 10, camFrame.cols / 5, camFrame.rows / 4);

		Rect firstCardROI(0, 0/*camFrame.rows / 3*/, camFrame.cols/2, camFrame.rows / 2);

		cvtColor(camFrame, threshFrame, CV_BGR2GRAY);  //convert frame to grayscale

		medianBlur(threshFrame, threshFrame, 5);     //blur the grayscale frame to remove noise as much as possible

		threshold(threshFrame, threshFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

		Mat firstCardSlot(threshFrame, firstCardROI);

		findMatch(firstCardSlot, templ);

		rectangle(threshFrame, firstCardROI, Scalar(255, 0, 0), 2);

		imshow("threshFrame", threshFrame);



		//Mat firstCardSlot(threshFrame, firstCardROI);
		//findContours(firstCardSlot, firstCardSlotBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		/*
		for (int i = 0; i < firstCardSlotBlobs.size(); i++)
		{
			drawContours(threshFrame, firstCardSlotBlobs, i, Scalar(255), -1, 8, noArray(), 2, Point(10, 10));


			Rect boundingBox = boundingRect(firstCardSlotBlobs[i]);
			boundingBox += Point(10, 10);

			rectangle(threshFrame, boundingBox, Scalar(255), 2);

		}

		rectangle(threshFrame, firstCardROI, Scalar(255, 0, 0), 2);*/

		//imshow("threshFrame", threshFrame);

		if (waitKey(30) >= 0)
			break;
		i--;
	}
}

void detectCards(Camera* myCamera) {

	int roiStartX = 470, roiShiftX = 137, roiY = 172, roiW = 100, roiH = 100;

	Mat cameraFrame;
	Mat thresholdedFrame;

	vector<Rect> cardROI;
	vector<Mat> cardSlots;

	//vector<vector<Point> > firstSlotBlobs;
	vector<vector<Point> > secondSlotBlobs;
	vector<vector<Point> > thirdSlotBlobs;
	vector<vector<Point> > fourthSlotBlobs;

	for (int i = 0; i < 4; i++) {
		cardROI.push_back(Rect(roiStartX, roiY, roiW, roiH));
		
		roiStartX -= roiShiftX;
	}

	namedWindow("contours", 1);
	namedWindow("cam", 1);
	moveWindow("cam", 50, 50);

	//unconditional loop
	while (true) {

		cameraFrame = myCamera->getCameraFrame();

		

		cvtColor(cameraFrame, thresholdedFrame, CV_BGR2GRAY);  //convert frame to grayscale

		medianBlur(thresholdedFrame, thresholdedFrame, 3);     //blur the grayscale frame to remove noise as much as possible

		threshold(thresholdedFrame, thresholdedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

		//threshold(thresholdedFrame, thresholdedFrame, 80, 255, CV_THRESH_BINARY_INV);

		for (size_t i = 0; i < cardROI.size(); i++) {
			cardSlots.push_back(Mat(thresholdedFrame, cardROI[i]));
		}

		Mat roiMatrix(thresholdedFrame, cardROI[1]);  //define roi matrix for blob detection

		Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection

		//vector<vector<Point> > contours0;

		findContours(cardSlots[1], contours, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the roiMatrix

		//findContours(thresholdedFrame, contours0, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in frame

		vector<vector<Point> > firstSlotBlobs;

		
		if (contours.size() != 0) {
			/*
			contours.resize(contours0.size());
			for (size_t k = 0; k < contours0.size(); k++)
				//approximation of blob
				approxPolyDP(Mat(contours0[k]), contours[k], 2, true);
				*/
			//namedWindow("contours", 1);
			//namedWindow("cam", 1);

			for (size_t i = 0; i < contours.size() - 1; i++) {
				int j = i + 1;
				while (j > 0) {
					if (contours[j - 1].size() < contours[j].size()) {
						vector<Point> temp = contours[j - 1];
						contours[j - 1] = contours[j];
						contours[j] = temp;
					}
					j--;
				}
			}
			
			//discarding blobs with area bigger than 500 and smaller than 18 //table
			for (size_t i = 0; i < contours.size(); i++)
			{
				if (contourArea(contours[i]) >= 18 && contourArea(contours[i]) <= 1000) {
					firstSlotBlobs.push_back(contours[i]);
				}
			}


			//discarding blobs with area smaller than 60   //laptop cam
			/*for (int i = 0; i < contours.size(); i++)
			{
				if (contourArea(contours[i]) < 60 || contourArea(contours[i]) > 5000) {
					contours.erase(contours.begin() + i);
				}
			}*/
			
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < firstSlotBlobs.size(); i++)
			{
				// four times pi times the area divided by the perimeter squared
				//circularity = 4 * M_PI*contourArea(contours[i]) / pow(arcLength(contours[i], true), 2);

				drawContours(cnt_img, firstSlotBlobs, i, color, -1, 8, noArray(), 2, Point(cardROI[1].x, cardROI[1].y));

				Rect boundingBox = boundingRect(firstSlotBlobs[i]);
				boundingBox += Point(cardROI[1].x, cardROI[1].y);

				rectangle(cameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}
		}

		for (size_t i = 0; i < cardROI.size(); i++) {
			rectangle(cameraFrame, cardROI[i], Scalar(255, 0, 0), 2);
		}


		imshow("cam", cameraFrame);           //show color frame


		//imshow("thresholded frame", thresholdedFrame);

		imshow("contours", cnt_img);        //show blob detection frame

		if (firstSlotBlobs.size() != 0) {
			cout << "Number of blobs: " << firstSlotBlobs.size() << " area of biggest blob: " << contourArea(firstSlotBlobs[0]) << endl;
		}


//cout << "contours[0]'s bounding box aspect ratio (width/height): " << (float)boundingBox.width / boundingBox.height << endl;
//cout << "Number of blobs: " << contours.size() << " area of biggest blob: " << contourArea(contours[0]) << endl;

//area of blob | perimeter of blob
//cout<<"contourArea(contours[0]): "<<contourArea(contours[0])<<" arcLength(contours[0],true): "<< arcLength(contours[0], true)<<endl;
		if (waitKey(30) >= 0)
			break;
	}

}





//double circularity;
int main()
{
	Camera* myCam = new Camera();

	//VideoCapture stream1(0); //0 is the id of video device.0 if you have only one
	//camera.
	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}


	//thread templateDetectThread(detectTemplate, myCam);
	//cardDetectThread.detach();
	thread cardDetectThread(detectCards, myCam);

	//templateDetectThread.detach();
	cardDetectThread.detach();

	while (true) {


	}
		

	return 0;
}
#include "Cards.h"
#include "Sounds.h"

vector<int> curCards = { 1,2,3,4 };

vector<int> getCurCards() {
	return curCards;
}

void detectCards(Camera* myCamera, Rect cardROI) {

	bool isChordPlayed = false;
	vector<vector<Point> > foundBlobs;

	Mat cameraFrame;
	Mat thresholdedFrame;

	//Rect cardROI = Rect(roiStartX, roiY, roiW, roiH);

	Mat cardSlotMat;

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

		cardSlotMat = Mat(thresholdedFrame, cardROI);

		//Mat roiMatrix(thresholdedFrame, cardROI[1]);  //define roi matrix for blob detection

		Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection


		findContours(cardSlotMat, foundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the roiMatrix

		//findContours(thresholdedFrame, contours0, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in frame

		vector<vector<Point> > firstSlotBlobs;


		if (foundBlobs.size() != 0) {

			for (size_t i = 0; i < foundBlobs.size() - 1; i++) {
				int j = i + 1;
				while (j > 0) {
					if (foundBlobs[j - 1].size() < foundBlobs[j].size()) {
						vector<Point> temp = foundBlobs[j - 1];
						foundBlobs[j - 1] = foundBlobs[j];
						foundBlobs[j] = temp;
					}
					j--;
				}
			}

			//discarding blobs with area bigger than 500 and smaller than 18 //table
			for (size_t i = 0; i < foundBlobs.size(); i++)
			{
				double circularity = 4 * M_PI*contourArea(foundBlobs[i]) / pow(arcLength(foundBlobs[i], true), 2);

				if (contourArea(foundBlobs[i]) >= 30 && contourArea(foundBlobs[i]) <= 1000 && circularity > 0.8) {
					firstSlotBlobs.push_back(foundBlobs[i]);
				}
			}


			Scalar color(188, 255, 255);
			for (size_t i = 0; i < firstSlotBlobs.size(); i++)
			{

				drawContours(cnt_img, firstSlotBlobs, i, color, -1, 8, noArray(), 2, Point(cardROI.x, cardROI.y));

				Rect boundingBox = boundingRect(firstSlotBlobs[i]);
				boundingBox += Point(cardROI.x, cardROI.y);

				rectangle(cameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}
		}

		rectangle(cameraFrame, cardROI, Scalar(255, 0, 0), 2);


		imshow("cam", cameraFrame);           //show color frame


		//imshow("thresholded frame", thresholdedFrame);

		imshow("contours", cnt_img);        //show blob detection frame

		if (firstSlotBlobs.size() != 0 && !isChordPlayed) {

			PlayChord(firstSlotBlobs.size());
			//cout << "Number of blobs: " << firstSlotBlobs.size() << " area of biggest blob: " << contourArea(firstSlotBlobs[0]) << endl;
			isChordPlayed = true;
		}
		else if (firstSlotBlobs.size() == 0) {
			isChordPlayed = false;
		}

		if (waitKey(30) >= 0)
			break;
	}

}
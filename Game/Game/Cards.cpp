#include "Cards.h"
#include "Sounds.h"

vector<int> curCards = { 0,0,0,0 };
//vector<int> curCards;


vector<int> getCurCards() {
	return curCards;
}

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread) {

	bool isChordPlayed = false;
	vector<vector<Point> > foundBlobs;

	Mat cameraFrame;
	Mat thresholdedFrame;

	Mat cardSlotMat;

	namedWindow("contours" + to_string(slot.id), 1);
	namedWindow("cam" + to_string(slot.id), 1);
	moveWindow("cam" + to_string(slot.id), 50, 50);

	//unconditional loop
	while (true) {

		waitKey(500);

		cameraFrame = myCamera->getCameraFrame();

		cvtColor(cameraFrame, thresholdedFrame, CV_BGR2GRAY);  //convert frame to grayscale

		medianBlur(thresholdedFrame, thresholdedFrame, 3);     //blur the grayscale frame to remove noise as much as possible

		threshold(thresholdedFrame, thresholdedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame
		
		//threshold(thresholdedFrame, thresholdedFrame, 80, 255, CV_THRESH_BINARY_INV);

		cardSlotMat = Mat(thresholdedFrame, cardROI);


		Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection


		findContours(cardSlotMat, foundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the roiMatrix

		//findContours(thresholdedFrame, contours0, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in frame

		vector<vector<Point> > slotBlobs;

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

			//discarding blobs with area bigger than 500 and smaller than 30 //table
			for (size_t i = 0; i < foundBlobs.size(); i++)
			{
				double circularity = 4 * M_PI*contourArea(foundBlobs[i]) / pow(arcLength(foundBlobs[i], true), 2);

				if (contourArea(foundBlobs[i]) >= 30 && contourArea(foundBlobs[i]) <= 1000 && circularity > 0.8) {
					slotBlobs.push_back(foundBlobs[i]);
				}
			}


			Scalar color(188, 255, 255);
			for (size_t i = 0; i < slotBlobs.size(); i++)
			{

				drawContours(cnt_img, slotBlobs, i, color, -1, 8, noArray(), 2, Point(cardROI.x, cardROI.y));

				Rect boundingBox = boundingRect(slotBlobs[i]);
				boundingBox += Point(cardROI.x, cardROI.y);

				rectangle(cameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}
		}

		rectangle(cameraFrame, cardROI, Scalar(255, 0, 0), 2);


		imshow("cam" + to_string(slot.id), cameraFrame);           //show color frame


		//imshow("thresholded frame", thresholdedFrame);

		imshow("contours" + to_string(slot.id), cnt_img);        //show blob detection frame

		for (int i = 0; i < SLOTS_NUM; i++) {
			if (slot.id == i + 1) {
				if (slotBlobs.size() != 0 && !isChordPlayed) {

					slot.chord = slotBlobs.size();
					PlayChord(slot.chord);

					isChordPlayed = true;
					curCards.at(i) = slot.chord;

				}
				else if (slotBlobs.size() == 0 && isChordPlayed) {
					isChordPlayed = false;

					curCards.at(i) = 0;
				}
			}
			cout << "slot " << i+1 << ": " << curCards.at(i) << endl;
		}

		if (waitKey(30) >= 0)
			break;

	}//end while
	thread.detach();
}
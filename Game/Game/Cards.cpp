#include "Cards.h"
#include "Sounds.h"

vector<int> curCards = { 0,0,0,0 };


vector<int> getCurCards() {
	return curCards;
}

int cRoiStartX = 470; //, roiShiftX = 137, roiY = 172, roiW = 100, roiH = 100;
int mbRoiStartX = 20, mbRoiStartY = 150;



bool levelRunning = false;

thread cardSlotThread[SLOTS_NUM];
thread mbThread[MENU_BUT_NUM];

vector<cardSlot> slots(SLOTS_NUM);

Camera* myCam = new Camera();

vector<Rect> cardROIs;
vector<Rect> mbROIs;

Mat segmentFrame(Mat frame) {

	Mat segmentedFrame;

	cvtColor(frame, segmentedFrame, CV_BGR2GRAY);  //convert frame to grayscale

	medianBlur(segmentedFrame, segmentedFrame, 3);     //blur the grayscale frame to remove noise as much as possible

	threshold(segmentedFrame, segmentedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

	return segmentedFrame;
}

vector<vector<Point>> sortBlobs(vector<vector<Point>> blobs) {
	for (size_t i = 0; i < blobs.size() - 1; i++) {
		int j = i + 1;
		while (j > 0) {
			if (blobs[j - 1].size() < blobs[j].size()) {
				vector<Point> temp = blobs[j - 1];
				blobs[j - 1] = blobs[j];
				blobs[j] = temp;
			}
			j--;
		}
	}
	return blobs;
}

void mbBlobDetect(Rect mbROI, int buttonId, thread &thread) {

	vector<vector<Point> > mbFoundBlobs;
	Mat mbCameraFrame;
	Mat mbThresholdedFrame;
	Mat menuButtonMat;

	namedWindow("menuCam" + buttonId, 1);
	moveWindow("menuCam" + buttonId, 30, 30);

	while (true) {

		waitKey(200);

		mbCameraFrame = myCam->getCameraFrame();

		cvtColor(mbCameraFrame, mbThresholdedFrame, CV_BGR2GRAY);  //convert frame to grayscale

		medianBlur(mbThresholdedFrame, mbThresholdedFrame, 3);     //blur the grayscale frame to remove noise as much as possible

		threshold(mbThresholdedFrame, mbThresholdedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

		//threshold(thresholdedFrame, thresholdedFrame, 80, 255, CV_THRESH_BINARY_INV);

		menuButtonMat = Mat(mbThresholdedFrame, mbROI);

		findContours(menuButtonMat, mbFoundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		vector<vector<Point> > mbFilteredBlobs;

		if (mbFoundBlobs.size() != 0) {
			mbFoundBlobs = sortBlobs(mbFoundBlobs);

			for (size_t i = 0; i < mbFoundBlobs.size(); i++)
			{
				double circularity = 4 * M_PI*contourArea(mbFoundBlobs[i]) / pow(arcLength(mbFoundBlobs[i], true), 2);

				if (contourArea(mbFoundBlobs[i]) >= 30 && contourArea(mbFoundBlobs[i]) <= 40 && circularity > 0.8) {
					mbFilteredBlobs.push_back(mbFoundBlobs[i]);
				}
			}



		}



		rectangle(mbCameraFrame, mbROI, Scalar(255, 0, 0), 2);

		imshow("menuCam" + buttonId, mbCameraFrame);

		if (waitKey(30) >= 0)
			break;
	}
	thread.detach();
}

void runMenuThreads() {

	for (int i = 0; i < MENU_BUT_NUM; i++) {
		if (i % 2 == 0) {
			mbROIs.push_back(Rect(mbRoiStartX, mbRoiStartY, mbRoiW, mbRoiH));
		}
		else {
			mbROIs.push_back(Rect(mbRoiStartX+mbRoiShiftX, mbRoiStartY, mbRoiW, mbRoiH));
			mbRoiStartY += mbRoiShiftY;
		}

		mbThread[i] = thread(mbBlobDetect, mbROIs[i], i + 1, ref(mbThread[i]));
	}
}

void runCardThreads() {

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//make vector of card ROIs
	for (int i = 0; i < SLOTS_NUM; i++) {
		cardROIs.push_back(Rect(cRoiStartX, cRoiY, cRoiW, cRoiH));

		slots[i].id = i + 1;

		cardSlotThread[i] = thread(detectCards, myCam, cardROIs[i], ref(slots[i]), ref(cardSlotThread[i]));

		cRoiStartX -= cRoiShiftX;
	}
}

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread) {

	bool isChordPlayed = false;
	vector<vector<Point> > foundBlobs;

	Mat cameraFrame;
	Mat thresholdedFrame;
	Mat segmentedFrame;

	Mat cardSlotMat;

	namedWindow("contours" + to_string(slot.id), 1);
	namedWindow("cam" + to_string(slot.id), 1);
	moveWindow("cam" + to_string(slot.id), 50, 50);

	//unconditional loop
	while (levelRunning) {

		waitKey(500);

		cameraFrame = myCamera->getCameraFrame();

		/*
		cvtColor(cameraFrame, thresholdedFrame, CV_BGR2GRAY);  //convert frame to grayscale

		medianBlur(thresholdedFrame, thresholdedFrame, 3);     //blur the grayscale frame to remove noise as much as possible

		threshold(thresholdedFrame, thresholdedFrame, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame*/

		segmentedFrame = segmentFrame(cameraFrame);
		
		//threshold(thresholdedFrame, thresholdedFrame, 80, 255, CV_THRESH_BINARY_INV);

		//cardSlotMat = Mat(thresholdedFrame, cardROI);

		cardSlotMat = Mat(segmentedFrame, cardROI);


		Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection


		findContours(cardSlotMat, foundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the roiMatrix

		//findContours(thresholdedFrame, contours0, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in frame

		vector<vector<Point> > slotBlobs;

		if (foundBlobs.size() != 0) {


			foundBlobs = sortBlobs(foundBlobs);

			/*
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
			*/

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

		//draw ROI in camera frame
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
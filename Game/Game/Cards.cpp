#include "GUI.h"
#include "Cards.h"
#include "Sounds.h"
#include "Levels.h"


bool levelRunning = false;
bool isInMenu = true;

int cRoiStartX = 470;
int mbRoiStartX = 20, mbRoiStartY = 150, lbRoiStartX = 115;

thread cardSlotThread[SLOTS_NUM];
thread mbThread[MENU_BUT_NUM];
thread lbThread[3];

vector<int> curCards = { 0,0,0,0 };
vector<cardSlot> slots(SLOTS_NUM);
vector<Rect> cardROIs;
vector<Rect> mbROIs;
vector<Rect> lbROIs;

vector<int> getCurCards() {
	return curCards;
}

Mat segmentROI(Mat frame) {

	Mat segmentedROI;

	cvtColor(frame, segmentedROI, CV_BGR2GRAY);  //convert frame to grayscale

	medianBlur(segmentedROI, segmentedROI, 3);     //blur the grayscale frame to remove noise as much as possible

	//threshold(segmentedROI, segmentedROI, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

	threshold(segmentedROI, segmentedROI, 85, 255, CV_THRESH_BINARY_INV);  //threshold grayscale frame

	return segmentedROI;
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

void detectMenuBlobs(Camera* myCamera, Rect mbROI, int buttonId, thread &thread) {

	bool isButtonClicked = false;

	vector<vector<Point> > mbFoundBlobs;
	Mat mbCameraFrame;
	//Mat mbThresholdedFrame;
	Mat menuButtonMat;

	namedWindow("menuCam" + to_string(buttonId), 1);
	moveWindow("menuCam" + to_string(buttonId), 30, 30);

	//Run the while loop only if in Menu screen
	while (isInMenu) {

		waitKey(500);

		mbCameraFrame = myCamera->getCameraFrame();
		
		menuButtonMat = Mat(mbCameraFrame, mbROI);

		menuButtonMat = segmentROI(menuButtonMat);

		//Mat cnt_img = Mat::zeros(mbCameraFrame.rows, mbCameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection

		findContours(menuButtonMat, mbFoundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		vector<vector<Point> > mbFilteredBlobs;

		if (mbFoundBlobs.size() != 0) {

			mbFoundBlobs = sortBlobs(mbFoundBlobs);

			for (size_t i = 0; i < mbFoundBlobs.size(); i++)
			{
				double circularity = 4 * M_PI*contourArea(mbFoundBlobs[i]) / pow(arcLength(mbFoundBlobs[i], true), 2);

				if (contourArea(mbFoundBlobs[i]) >= 30 && contourArea(mbFoundBlobs[i]) <= 800 && circularity > 0.8) {
					mbFilteredBlobs.push_back(mbFoundBlobs[i]);
				}
			}

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < mbFilteredBlobs.size(); i++)
			{
				//drawContours(cnt_img, mbFilteredBlobs, i, color, -1, 8, noArray(), 2, Point(mbROI.x, mbROI.y));
				Rect boundingBox = boundingRect(mbFilteredBlobs[i]);
				boundingBox += Point(mbROI.x, mbROI.y);

				rectangle(mbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			rectangle(mbCameraFrame, mbROI, Scalar(255, 0, 0), 2);

			imshow("menuCam" + to_string(buttonId), mbCameraFrame);
			
			for (int i = 0; i < MENU_BUT_NUM; i++) {
				if (mbFilteredBlobs.size()==1 && !isButtonClicked) {
					
					checkMenuButton(buttonId);
					isButtonClicked = true;
				}
				else if (mbFilteredBlobs.size() != 1) {
					isButtonClicked = false;
				}
			}
		}

		if (waitKey(30) >= 0)
			break;
	}
	thread.detach();
}

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread) {

	bool isChordPlayed = false;
	vector<vector<Point> > foundBlobs;

	Mat cameraFrame;
	//Mat thresholdedFrame;
	Mat cardSlotMat;

	//namedWindow("contours" + to_string(slot.id), 1);
	//namedWindow("cam" + to_string(slot.id), 1);
	//moveWindow("cam" + to_string(slot.id), 50, 50);

	//Run the while loop only if in level screen
	while (levelRunning) {

		waitKey(500);

		cameraFrame = myCamera->getCameraFrame();

		cardSlotMat = Mat(cameraFrame, cardROI);

		cardSlotMat = segmentROI(cardSlotMat);

		//if (slot.id == 3) {
		//	imwrite("thirdslot.jpg", cardSlotMat);
		//}

		Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection


		findContours(cardSlotMat, foundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the roiMatrix

		vector<vector<Point> > slotBlobs;

		if (foundBlobs.size() != 0) {


			foundBlobs = sortBlobs(foundBlobs);

			//discarding blobs with area bigger than 1000 and smaller than 30 //table
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


		//imshow("cam" + to_string(slot.id), cameraFrame);           //show color frame


																   //imshow("thresholded frame", thresholdedFrame);

		//imshow("contours" + to_string(slot.id), cnt_img);        //show blob detection frame

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
			cout << "slot " << i + 1 << ": " << curCards.at(i) << endl;
		}

		if (waitKey(30) >= 0)
			break;

	}//end while
	thread.detach();
}

void detectLevelButtonBlobs(Camera* myCamera, Rect lbROI, int buttonId, thread &thread) {

	bool isLevelButtonClicked = false;
	vector<vector<Point> > lbFoundBlobs;

	Mat lbCameraFrame;
	//Mat thresholdedFrame;
	Mat lbMat;

	//namedWindow("contours" + to_string(buttonId), 1);
	namedWindow("lbCam" + to_string(buttonId), 1);
	moveWindow("lbCam" + to_string(buttonId), 50, 50);

	while (levelRunning) {

		//cout << "level button thread here" << endl;

		waitKey(500);
		lbCameraFrame = myCamera->getCameraFrame();

		lbMat = Mat(lbCameraFrame, lbROI);

		lbMat = segmentROI(lbMat);

		findContours(lbMat, lbFoundBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		vector<vector<Point> > lbFilteredBlobs;

		if (lbFoundBlobs.size() != 0) {

			lbFoundBlobs = sortBlobs(lbFoundBlobs);

			for (size_t i = 0; i < lbFoundBlobs.size(); i++)
			{
				double circularity = 4 * M_PI*contourArea(lbFoundBlobs[i]) / pow(arcLength(lbFoundBlobs[i], true), 2);

				if (contourArea(lbFoundBlobs[i]) >= 30 && contourArea(lbFoundBlobs[i]) <= 800 && circularity > 0.8) {
					lbFilteredBlobs.push_back(lbFoundBlobs[i]);
				}
			}

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < lbFilteredBlobs.size(); i++)
			{
				//drawContours(cnt_img, mbFilteredBlobs, i, color, -1, 8, noArray(), 2, Point(mbROI.x, mbROI.y));
				Rect boundingBox = boundingRect(lbFilteredBlobs[i]);
				boundingBox += Point(lbROI.x, lbROI.y);

				rectangle(lbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			rectangle(lbCameraFrame, lbROI, Scalar(255, 0, 0), 2);

			imshow("lbCam" + to_string(buttonId), lbCameraFrame);

			for (int i = 0; i < MENU_BUT_NUM; i++) {
				if (lbFilteredBlobs.size() == 1 && !isLevelButtonClicked) {
					
					checkLevelButton(buttonId);
					isLevelButtonClicked = true;
				}
				else if (lbFilteredBlobs.size() != 1) {
					isLevelButtonClicked = false;
				}
			}
		}


		if (waitKey(30) >= 0)
			break;

	}//end while
	thread.detach();
}

void runMenuThreads() {

	Camera* myCam = new Camera();

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	for (int i = 0; i < MENU_BUT_NUM; i++) {
		if (i % 2 == 0) {
			mbROIs.push_back(Rect(mbRoiStartX, mbRoiStartY, mbRoiW, mbRoiH));
		}
		else {
			mbROIs.push_back(Rect(mbRoiStartX+mbRoiShiftX, mbRoiStartY, mbRoiW, mbRoiH));
			mbRoiStartY += mbRoiShiftY;
		}

		mbThread[i] = thread(detectMenuBlobs, myCam, mbROIs[i], i + 1, ref(mbThread[i]));
	}
}

void runCardThreads() {

	Camera* myCam = new Camera();

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

void runLevelButtonThreads() {

	Camera* myCam = new Camera();

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//make vector of level button ROIs
	for (int i = 0; i < 3; i++) {
		lbROIs.push_back(Rect(lbRoiStartX, lbRoiY, lbRoiW, lbRoiH));

		//slots[i].id = i + 1;

		lbThread[i] = thread(detectLevelButtonBlobs, myCam, lbROIs[i], i+1, ref(lbThread[i]));

		lbRoiStartX += lbRoiShiftX;
	}


}

void checkMenuButton(int buttonId) {
	switch (buttonId) {
	case MENU_EXIT:
		isInMenu = false;

		destroyAllWindows();
		break;
	case MENU_TUTORIAL:
		setLvl(1);
		break;
	case MENU_LVL1:
		setLvl(2);
		break;
	case MENU_LVL2:
		setLvl(3);
		break;
	case MENU_LVL3:
		setLvl(4);
		break;
	case MENU_CREDITS:
		setLvl(5);
		break;
	}
	if (getLvl() != 0) {
		runLevel(getLvl());
	}
}

void checkLevelButton(int buttonId) {
	switch (buttonId) {
	case 1:
		setLvl(0);

		setTut(1);
		levelRunning = false;
		runLevel(getLvl());
		break;
	case 2:
		Confirm(getCurCards(), getLvl(), testTemp);
		break;
	case 3:
		RepeatSong();
		break;
	}
}
#include "GUI.h"
#include "Cards.h"
#include "Sounds.h"
#include "Levels.h"


bool levelRunning = false;
bool isInMenu = true;
bool isInCreditsOrTheory = false;

int cRoiStartX = 470;
int mbRoiStartX = 20, mbRoiStartY = 150, lbRoiStartX = 115, pbRoiStartX = 100;

thread cardSlotThread[SLOTS_NUM];
thread mbThread[MENU_BUT_NUM];
thread lbThread[3];
thread pbThread[4];
thread crThThread;

vector<int> curCards = { 0,0,0,0 };
vector<cardSlot> slots(SLOTS_NUM);
vector<Rect> cardROIs;
vector<Rect> mbROIs;
vector<Rect> lbROIs;
vector<Rect> pbROIs;
Rect crThROI;

vector<int> getCurCards() {
	return curCards;
}

Mat segmentROI(Mat frame) {

	Mat segmentedROI;

	cvtColor(frame, segmentedROI, CV_BGR2GRAY);  //convert frame to grayscale

	medianBlur(segmentedROI, segmentedROI, 5);     //blur the grayscale frame to remove noise as much as possible

	segmentedROI = histogramStretching(segmentedROI);

	//threshold(segmentedROI, segmentedROI, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

	threshold(segmentedROI, segmentedROI, 85, 255, CV_THRESH_BINARY_INV);  //threshold grayscale frame

	return segmentedROI;
}

Mat segmentROI(Mat frame, int buttonId) {

	Mat segmentedROI;

	if (levelRunning) {

		cvtColor(frame, segmentedROI, CV_BGR2GRAY);  //convert frame to grayscale

		//imwrite("grayButton.jpg", segmentedROI);

		medianBlur(segmentedROI, segmentedROI, 5);     //blur the grayscale frame to remove noise as much as possible

		imwrite("blurred_" + to_string(buttonId) + ".jpg", segmentedROI);

		segmentedROI = histogramStretching(segmentedROI);

		imwrite("stretched_" + to_string(buttonId) + ".jpg", segmentedROI);

		//threshold(segmentedROI, segmentedROI, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

		threshold(segmentedROI, segmentedROI, 85, 255, CV_THRESH_BINARY_INV);  //threshold grayscale frame

		imwrite("threshold_" + to_string(buttonId) + ".jpg", segmentedROI);
	}
	else {
		cvtColor(frame, segmentedROI, CV_BGR2GRAY);  //convert frame to grayscale

		medianBlur(segmentedROI, segmentedROI, 5);     //blur the grayscale frame to remove noise as much as possible

		segmentedROI = histogramStretching(segmentedROI);

		//threshold(segmentedROI, segmentedROI, 0, 255, CV_THRESH_BINARY_INV + CV_THRESH_OTSU);  //threshold grayscale frame

		threshold(segmentedROI, segmentedROI, 85, 255, CV_THRESH_BINARY_INV);  //threshold grayscale frame

	}

	return segmentedROI;
}

Mat histogramStretching(Mat frame) {

	int pixelOI;
	int lowestPixel = 255;
	int highestPixel = 0;

	for (int y = 0; y < frame.rows; y++) {		//row
		for (int x = 0; x < frame.cols; x++) {	//column
			pixelOI = frame.at<unsigned char>(y, x);	//Assign current pixel's value to pixelOI

														//histogram[pixelOI]++;	//Increases the amount/number of pixels (from the line above) in the corresponding value in the array index
														//E.g. pixelIO = 32, then the index coresponding to the pixel value 32 will increase by one 
														//(frequency in the histogram increases for the specific intencity/index/pixel value)


			if (pixelOI < lowestPixel) {		//If the pixel which is being looked at (pixelOI) has a lower value than the lowest recorded ...
				lowestPixel = pixelOI;			//...pixel value so far, this will be the new lowest pixel value 
			}
			if (pixelOI > highestPixel) {		//The same as above is done here, just with the highest number instead
				highestPixel = pixelOI;
			}
		} //end for
	} //end for

	Mat output = frame.clone();

	for (int y = 0; y < frame.rows; y++) {
		for (int x = 0; x < frame.cols; x++) {
			pixelOI = frame.at<unsigned char>(y, x);
			output.at<unsigned char>(y, x) = (255 / (highestPixel - lowestPixel)) * (pixelOI - lowestPixel); //stretching math applied here (see p.53 in IP book eq 4.11)
		} //end for
	} //end for

	return output;
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

vector<vector<Point>> filterBlobs(vector<vector<Point>> blobs, int minArea, int maxArea) {

	vector<vector<Point>> filteredBlobs;
	//discarding blobs with area bigger than 1000 and smaller than 30 //table
	for (size_t i = 0; i < blobs.size(); i++)
	{
		double circularity = 4 * M_PI*contourArea(blobs[i]) / pow(arcLength(blobs[i], true), 2);

		if (contourArea(blobs[i]) >= minArea && contourArea(blobs[i]) <= maxArea && circularity > 0.8) {
			filteredBlobs.push_back(blobs[i]);
		}
	}
	return filteredBlobs;
}

void detectMenuBlobs(Camera* myCamera, Rect mbROI, int buttonId, thread &thread) {

	bool isButtonClicked = false;

	vector<vector<Point> > menuBlobs;
	Mat mbCameraFrame;
	//Mat mbThresholdedFrame;
	Mat menuButtonMat;

	//namedWindow("menuCam" + to_string(buttonId), 1);
	//moveWindow("menuCam" + to_string(buttonId), 30, 30);

	//Run the while loop only if in Menu screen
	while (isInMenu) {

		waitKey(500);

		mbCameraFrame = myCamera->getCameraFrame();
		
		menuButtonMat = Mat(mbCameraFrame, mbROI);

		menuButtonMat = segmentROI(menuButtonMat);

		//menuButtonMat = segmentROI(menuButtonMat, buttonId);

		//Mat cnt_img = Mat::zeros(mbCameraFrame.rows, mbCameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection

		findContours(menuButtonMat, menuBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		//vector<vector<Point> > mbFilteredBlobs;

		if (menuBlobs.size() != 0) {

			menuBlobs = sortBlobs(menuBlobs);

			menuBlobs = filterBlobs(menuBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < menuBlobs.size(); i++)
			{
				//drawContours(cnt_img, mbFilteredBlobs, i, color, -1, 8, noArray(), 2, Point(mbROI.x, mbROI.y));
				Rect boundingBox = boundingRect(menuBlobs[i]);
				boundingBox += Point(mbROI.x, mbROI.y);

				rectangle(mbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(mbCameraFrame, mbROI, Scalar(255, 0, 0), 2);

			//imshow("menuCam" + to_string(buttonId), mbCameraFrame);
			
			for (int i = 0; i < MENU_BUT_NUM; i++) {
				if (menuBlobs.size()==1 && !isButtonClicked) {
					
					checkMenuButton(buttonId);
					isButtonClicked = true;
				}
				else if (menuBlobs.size() != 1) {
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
		vector<vector<Point> > slotBlobs;

		Mat cameraFrame;
		Mat cardSlotMat;

		//namedWindow("contours" + to_string(slot.id), 1);
		//namedWindow("cam" + to_string(slot.id), 1);
		//moveWindow("cam" + to_string(slot.id), 50, 50);

		//Run the while loop only if in level screen
		while (levelRunning) {

			waitKey(500);

			cameraFrame = myCamera->getCameraFrame();

			cardSlotMat = Mat(cameraFrame, cardROI);

			//cardSlotMat = segmentROI(cardSlotMat, slot.id);

			cardSlotMat = segmentROI(cardSlotMat);

			//if (slot.id == 3) {
			//	imwrite("thirdslot.jpg", cardSlotMat);
			//}

			Mat cnt_img = Mat::zeros(cameraFrame.rows, cameraFrame.cols, CV_8UC3);   //matrix for displaying frame with blob detection

			findContours(cardSlotMat, slotBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the roiMatrix

			if (slotBlobs.size() != 0) {


				slotBlobs = sortBlobs(slotBlobs);

				slotBlobs = filterBlobs(slotBlobs, 100, 500);

				Scalar color(188, 255, 255);
				for (size_t i = 0; i < slotBlobs.size(); i++)
				{

					//drawContours(cnt_img, slotBlobs, i, color, -1, 8, noArray(), 2, Point(cardROI.x, cardROI.y));

					Rect boundingBox = boundingRect(slotBlobs[i]);
					boundingBox += Point(cardROI.x, cardROI.y);

					rectangle(cameraFrame, boundingBox, Scalar(255, 0, 0), 2);
				}
			}

			//draw ROI in camera frame
			//rectangle(cameraFrame, cardROI, Scalar(255, 0, 0), 2);

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
	vector<vector<Point> > lbBlobs;

	Mat lbCameraFrame;
	//Mat thresholdedFrame;
	Mat lbMat;

	//namedWindow("contours" + to_string(buttonId), 1);
	//namedWindow("lbCam" + to_string(buttonId), 1);
	//moveWindow("lbCam" + to_string(buttonId), 50, 50);

	while (levelRunning) {

		waitKey(500);
		lbCameraFrame = myCamera->getCameraFrame();

		lbMat = Mat(lbCameraFrame, lbROI);

		lbMat = segmentROI(lbMat);

		//lbMat = segmentROI(lbMat, buttonId);

		findContours(lbMat, lbBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		if (lbBlobs.size() != 0) {

			lbBlobs = sortBlobs(lbBlobs);

			lbBlobs = filterBlobs(lbBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < lbBlobs.size(); i++)
			{
				//drawContours(cnt_img, mbFilteredBlobs, i, color, -1, 8, noArray(), 2, Point(mbROI.x, mbROI.y));
				Rect boundingBox = boundingRect(lbBlobs[i]);
				boundingBox += Point(lbROI.x, lbROI.y);

				rectangle(lbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(lbCameraFrame, lbROI, Scalar(255, 0, 0), 2);

			//imshow("lbCam" + to_string(buttonId), lbCameraFrame);

			for (int i = 0; i < MENU_BUT_NUM; i++) {
				if (lbBlobs.size() == 1 && !isLevelButtonClicked) {
					
					checkLevelButton(buttonId);
					isLevelButtonClicked = true;
				}
				else if (lbBlobs.size() != 1) {
					isLevelButtonClicked = false;
				}
			}
		}


		if (waitKey(30) >= 0)
			break;

	}//end while
	thread.detach();
}

void detectTimelineButtonBlobs(Camera* myCamera, Rect pbROI, int buttonId, thread &thread) {

	bool isTimelineButtonClicked = false;
	vector<vector<Point>> pbBlobs;
	Mat pbCameraFrame;
	Mat pbMat;

	//namedWindow("pbCam" + to_string(buttonId), 1);
	//moveWindow("pbCam" + to_string(buttonId), 50, 50);

	while (levelRunning) {

		waitKey(500);
		pbCameraFrame = myCamera->getCameraFrame();

		pbMat = Mat(pbCameraFrame, pbROI);

		//pbMat = segmentROI(pbMat);

		pbMat = segmentROI(pbMat, buttonId);

		findContours(pbMat, pbBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		if (pbBlobs.size() != 0) {

			pbBlobs = sortBlobs(pbBlobs);

			pbBlobs = filterBlobs(pbBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < pbBlobs.size(); i++)
			{
				//drawContours(cnt_img, mbFilteredBlobs, i, color, -1, 8, noArray(), 2, Point(mbROI.x, mbROI.y));
				Rect boundingBox = boundingRect(pbBlobs[i]);
				boundingBox += Point(pbROI.x, pbROI.y);

				rectangle(pbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(pbCameraFrame, pbROI, Scalar(255, 0, 0), 2);

			//imshow("pbCam" + to_string(buttonId), pbCameraFrame);

			for (int i = 0; i < MENU_BUT_NUM; i++) {
				if (pbBlobs.size() == 1 && !isTimelineButtonClicked) {
					PlaySnippet(buttonId);
					isTimelineButtonClicked = true;
				}
				else if (pbBlobs.size() != 1) {
					isTimelineButtonClicked = false;
				}
			}
		}

		if (waitKey(30) >= 0)
			break;
	}//end while
	thread.detach();
}

void detectCreditsOrTheoryBlobs(Camera* myCamera, Rect bROI, thread &thread, bool isInCredits) {

	vector<vector<Point>> crBlobs;
	Mat crCameraFrame;
	Mat crMat;
	int lastSize = 0;

	//namedWindow("crCam", 1);
	//moveWindow("crCam", 50, 50);

	while (isInCreditsOrTheory) {


		waitKey(500);

		crCameraFrame = myCamera->getCameraFrame();

		crMat = Mat(crCameraFrame, bROI);

		crMat = segmentROI(crMat);

		//crMat = segmentROI(crMat, buttonId);

		findContours(crMat, crBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		if (crBlobs.size() != 0) {

			crBlobs = sortBlobs(crBlobs);

			crBlobs = filterBlobs(crBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < crBlobs.size(); i++)
			{
				//drawContours(cnt_img, mbFilteredBlobs, i, color, -1, 8, noArray(), 2, Point(mbROI.x, mbROI.y));
				Rect boundingBox = boundingRect(crBlobs[i]);
				boundingBox += Point(bROI.x, bROI.y);

				rectangle(crCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(crCameraFrame, bROI, Scalar(255, 0, 0), 2);

			//imshow("crCam", crCameraFrame);

			if (crBlobs.size() == 1) {
				if (isInCredits) {
					isInCreditsOrTheory = false;
					cout << "Back!" << endl;
					setLvl(0);
					isInMenu = true;
					runLevel(getLvl());
				}
				else {
					if (getTut() <= 11 && lastSize == 0) {
						setTut(getTut() + 1);
						loadTutorial(getTut());
						lastSize = 1;
					}
					else if(getTut() > 11 && lastSize == 0){
						setTut(0);
						//cout << getTut() << endl;

						loadLevel(1);

						waitKey(1000);
						RepeatSong();
						isInCreditsOrTheory = false;
						levelRunning = true;
						runLevelThreads();
					}
				}
			}
			else if (crBlobs.size() == 0) {
				lastSize = 0;
			}


		}
		//imshow("crCam", crCameraFrame);

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

void runLevelThreads() {

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

	//make vector of level button ROIs
	for (int i = 0; i < 3; i++) {
		lbROIs.push_back(Rect(lbRoiStartX, lbRoiY, lbRoiW, lbRoiH));

		lbThread[i] = thread(detectLevelButtonBlobs, myCam, lbROIs[i], i + 1, ref(lbThread[i]));

		lbRoiStartX += lbRoiShiftX;
	}

	for (int i = 0; i < 4; i++) {
		pbROIs.push_back(Rect(pbRoiStartX, pbRoiY, pbRoiW, pbRoiH));

		pbThread[i] = thread(detectTimelineButtonBlobs, myCam, pbROIs[i], 4-i, ref(pbThread[i]));

		pbRoiStartX += pbRoiShiftX;
	}
}

void runCreditsOrTheoryThread(bool isInCredits) {

	Camera* myCam = new Camera();

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	crThROI = Rect(crRoiX, crRoiY, crRoiW, crRoiH);
	crThThread = thread(detectCreditsOrTheoryBlobs, myCam, crThROI, ref(crThThread), isInCredits);
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
		curCards = { 0,0,0,0 };
		break;
	case MENU_LVL2:
		setLvl(3);
		curCards = { 0,0,0,0 };
		break;
	case MENU_LVL3:
		setLvl(4);
		curCards = { 0,0,0,0 };
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
		curCards = { 0,0,0,0 };
		break;
	case 2:
		Confirm(getCurCards(), getLvl(), testTemp);
		break;
	case 3:
		RepeatSong();
		break;
	}
}
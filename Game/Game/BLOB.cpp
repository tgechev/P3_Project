/*BLOB.cpp file implementing everything from BLOB.h file*/
#include "GUI.h"
#include "BLOB.h"
#include "Sounds.h"
#include "Levels.h"

//Setting the boolean variables
//When the game is run the menu loads, thus isInMenu is set to true and the others are false
bool levelRunning = false;
bool isInMenu = true;
bool isInCreditsOrTheory = false;

//Initial values for the variables needed to create the ROIs
int cRoiStartX = 470;
int mbRoiStartX = 20, mbRoiStartY = 150, lbRoiStartX = 115, pbRoiStartX = 100;

//Initial value for the threshold
int segmentTh = 85;

//Defining the threads
thread cardSlotThread[SLOTS_NUM];
thread mbThread[MENU_BUT_NUM];
thread lbThread[3];
thread pbThread[4];
thread crThThread;

//Defining vectors for the current cards, card slots and different ROIs
vector<int> curCards = { 0,0,0,0 };
vector<cardSlot> slots(SLOTS_NUM);
vector<Rect> cardROIs;
vector<Rect> mbROIs;
vector<Rect> lbROIs;
vector<Rect> pbROIs;
Rect crThROI;

//Creating a new Camera object and setting the pointer
Camera* myCam = new Camera();

//Getter function for the current cards vector
vector<int> getCurCards() {
	return curCards;
}

//SegmentROI() function takes a ROI as an input parameter and returns the segmented ROI
Mat segmentROI(Mat ROI) {

	cvtColor(ROI, ROI, CV_BGR2GRAY);  //convert ROI to grayscale

	medianBlur(ROI, ROI, 5);     //blur the grayscale ROI to remove noise as much as possible

	ROI = histogramStretching(ROI); //Perform histogram stretching on the blurred ROI

	threshold(ROI, ROI, segmentTh, 255, CV_THRESH_BINARY_INV);  //threshold ROI

	return ROI; //return segmented ROI
}

Mat segmentROI(Mat ROI, int buttonId) {

//If a level is running save a picture after every segmentation step, if not do not save a picture. Finally, return segmented ROI

if (levelRunning) {

cvtColor(ROI, ROI, CV_BGR2GRAY);  //convert frame to grayscale

imwrite("grayButton.jpg", ROI);

medianBlur(ROI, ROI, 5);

imwrite("blurred_" + to_string(buttonId) + ".jpg", ROI);

ROI = histogramStretching(ROI);

imwrite("stretched_" + to_string(buttonId) + ".jpg", ROI);

threshold(ROI, ROI, segmentTh, 255, CV_THRESH_BINARY_INV);

imwrite("threshold_" + to_string(buttonId) + ".jpg", ROI);
}
else {
cvtColor(ROI, ROI, CV_BGR2GRAY);

medianBlur(ROI, ROI, 5);

ROI = histogramStretching(ROI);

threshold(ROI, ROI, 85, 255, CV_THRESH_BINARY_INV);

}

return ROI;
}

Mat histogramStretching(Mat ROI) {

	int pixelOI;
	int lowestPixel = 255;
	int highestPixel = 0;

	for (int y = 0; y < ROI.rows; y++) {		//row
		for (int x = 0; x < ROI.cols; x++) {	//column
			pixelOI = ROI.at<unsigned char>(y, x);	//Assign current pixel's value to pixelOI

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

	Mat output = ROI.clone();

	for (int y = 0; y < ROI.rows; y++) {
		for (int x = 0; x < ROI.cols; x++) {
			pixelOI = ROI.at<unsigned char>(y, x);
			output.at<unsigned char>(y, x) = (255 / (highestPixel - lowestPixel)) * (pixelOI - lowestPixel); //stretching math applied here (see p.53 in IP book eq 4.11)
		} //end for
	} //end for

	return output;
}

//Function that sorts the found BLOBs by size in decreasing order
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

//Function that filters BLOBs by area and circularity
vector<vector<Point>> filterBlobs(vector<vector<Point>> blobs, int minArea, int maxArea) {

	vector<vector<Point>> filteredBlobs; //vector containing the filtered BLOBs


	for (size_t i = 0; i < blobs.size(); i++)
	{
		//calculating circularity by multiplying 4*pi by the BLOBs area and deviding everything by the BLOB's perimeter
		double circularity = 4 * M_PI*contourArea(blobs[i]) / pow(arcLength(blobs[i], true), 2);

		//Saving BLOBs bigger than minArea, smaller than maxArea, and with circularity bigger than 0.8
		if (contourArea(blobs[i]) >= minArea && contourArea(blobs[i]) <= maxArea && circularity > 0.8) {
			filteredBlobs.push_back(blobs[i]);
		}
	}
	return filteredBlobs;
}

void detectMenuBlobs(Camera* myCamera, Rect mbROI, int buttonId, thread &thread) {

	//Getting the width and height of a frame
	int frame_width = myCamera->getStream().get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = myCamera->getStream().get(CV_CAP_PROP_FRAME_HEIGHT);

	//VideoWriter object which records video from the camera under the table
	//VideoWriter video("menu_button" + to_string(buttonId) + ".avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(frame_width, frame_height), true);

	//Boolean determining if a button has been clicked
	bool isButtonClicked = false;

	//Vector containing all the found BLOBs
	vector<vector<Point> > menuBlobs;

	//Mat object holding the current camera frame
	Mat mbCameraFrame;

	//Mat object holding the menu button ROI matrix
	Mat menuButtonMat;

	//windows displaying the camera feed, used for debugging purposes
	//namedWindow("menuCam" + to_string(buttonId), 1);
	//moveWindow("menuCam" + to_string(buttonId), 30, 30);

	//Run the while loop only if in Menu screen
	while (isInMenu) {

		//Wait 500 miliseconds
		waitKey(500);
		//Get frame from camera
		mbCameraFrame = myCamera->getCameraFrame();

		//Cut out ROI from thhe whole frame
		menuButtonMat = Mat(mbCameraFrame, mbROI);

		//Segment the ROI
		menuButtonMat = segmentROI(menuButtonMat);

		//menuButtonMat = segmentROI(menuButtonMat, buttonId);

		//Function for finding BLOBs in ROI
		findContours(menuButtonMat, menuBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		//If any BLOBs are found execute below code
		if (menuBlobs.size() != 0) {

			//Sorting the BLOBs
			menuBlobs = sortBlobs(menuBlobs);

			//Filtering the BLOBs
			menuBlobs = filterBlobs(menuBlobs, 100, 500);

			//drawing bounding box in camera frame
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < menuBlobs.size(); i++)
			{

				Rect boundingBox = boundingRect(menuBlobs[i]); //Find the bounding box of the current BLOB
				boundingBox += Point(mbROI.x, mbROI.y); //Adjust the coordinates for the bounding box

				rectangle(mbCameraFrame, boundingBox, Scalar(255, 0, 0), 2); //Draw the bounding box
			}

			//rectangle(mbCameraFrame, mbROI, Scalar(255, 0, 0), 2); //Draw the ROI in the camera frame

			//video.write(mbCameraFrame); //Write the current frame in the video 

			//imshow("menuCam" + to_string(buttonId), mbCameraFrame); //Show the feed from the camera

			for (int i = 0; i < MENU_BUT_NUM; i++) {
				//If a BLOB is found and a button has not been clicked
				if (menuBlobs.size() == 1 && !isButtonClicked) {

					checkMenuButton(buttonId); //Check which button has been clicked
					isButtonClicked = true; //Indicate that a button has been clicked
				}
				//Else if BLOB is not found indicate that a button has not been clicked
				else if (menuBlobs.size() != 1) {
					isButtonClicked = false;
				}
			}
		}

		if (waitKey(30) >= 0)
			break;
	}

	thread.detach(); //detach the thread
}

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread) {

	//Getting the width and height of a frame
	int frame_width = myCamera->getStream().get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = myCamera->getStream().get(CV_CAP_PROP_FRAME_HEIGHT);

	//VideoWriter object which records video from the camera under the table
	//VideoWriter video("level" + to_string(getLvl()-1) + "_card_slot" + to_string(slot.id) + ".avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(frame_width, frame_height), true);

	//Boolean determining if a chord is played
	bool isChordPlayed = false;

	//Vector containing all the found BLOBs
	vector<vector<Point> > slotBlobs;

	//Mat object holding the current camera frame
	Mat cameraFrame;

	//Mat object holding the card slot ROI matrix
	Mat cardSlotMat;

	//windows displaying the camera feed, used for debugging purposes
	//namedWindow("cam" + to_string(slot.id), 1);
	//moveWindow("cam" + to_string(slot.id), 50, 50);

	//Run the while loop only if in level screen
	while (levelRunning) {

		//Wait 500 miliseconds
		waitKey(500);

		//Get camera frame
		cameraFrame = myCamera->getCameraFrame();

		//cut out ROI from the whole frame
		cardSlotMat = Mat(cameraFrame, cardROI);

		//Segment the ROI
		cardSlotMat = segmentROI(cardSlotMat);

		findContours(cardSlotMat, slotBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);  //find blobs in the ROI

		if (slotBlobs.size() != 0) {

			//Sort blobs
			slotBlobs = sortBlobs(slotBlobs);

			//Filter blobs
			slotBlobs = filterBlobs(slotBlobs, 100, 500);

			//Draw bounding box in camera frame
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < slotBlobs.size(); i++)
			{
				Rect boundingBox = boundingRect(slotBlobs[i]);
				boundingBox += Point(cardROI.x, cardROI.y);

				rectangle(cameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}
		}


		//draw ROI in camera frame
		//rectangle(cameraFrame, cardROI, Scalar(255, 0, 0), 2);

		//video.write(cameraFrame);
		//imshow("cam" + to_string(slot.id), cameraFrame);           //show color frame

		//Go through the slots and play chords

		for (int i = 0; i < SLOTS_NUM; i++) {

			//If the slot is the current one from the iteration
			if (slot.id == i + 1) {
				//If blobs are found and chord is not played
				if (slotBlobs.size() != 0 && !isChordPlayed) {

					//save the chord
					slot.chord = slotBlobs.size();

					//Play the chord
					PlayChord(slot.chord);

					//Indicate that a chord has been played
					isChordPlayed = true;

					//Put the chord in the current cards vector
					curCards.at(i) = slot.chord;

				}
				//Else if no blobs are found indicate that a chord is not played
				else if (slotBlobs.size() == 0 && isChordPlayed) {
					isChordPlayed = false;

					//set the card to 0
					curCards.at(i) = 0;
				}
			}

			//cout << "slot " << i + 1 << ": " << curCards.at(i) << endl; //print the current value for every card slot
		}

		if (waitKey(30) >= 0)
			break;

	}//end while

	thread.detach(); //detach thread
}

void detectLevelButtonBlobs(Camera* myCamera, Rect lbROI, int buttonId, thread &thread) {

	int frame_width = myCamera->getStream().get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = myCamera->getStream().get(CV_CAP_PROP_FRAME_HEIGHT);

	//VideoWriter video("level" + to_string(getLvl()-1) + "_level_button" + to_string(buttonId) + ".avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(frame_width, frame_height), true);

	bool isLevelButtonClicked = false;
	vector<vector<Point> > lbBlobs;

	Mat lbCameraFrame;

	Mat lbMat;

	//namedWindow("lbCam" + to_string(buttonId), 1);
	//moveWindow("lbCam" + to_string(buttonId), 50, 50);

	while (levelRunning) {

		waitKey(500);
		lbCameraFrame = myCamera->getCameraFrame();

		lbMat = Mat(lbCameraFrame, lbROI);

		lbMat = segmentROI(lbMat);

		findContours(lbMat, lbBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		if (lbBlobs.size() != 0) {

			lbBlobs = sortBlobs(lbBlobs);

			lbBlobs = filterBlobs(lbBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < lbBlobs.size(); i++)
			{
				Rect boundingBox = boundingRect(lbBlobs[i]);
				boundingBox += Point(lbROI.x, lbROI.y);

				rectangle(lbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(lbCameraFrame, lbROI, Scalar(255, 0, 0), 2);

			//video.write(lbCameraFrame);

			//imshow("lbCam" + to_string(buttonId), lbCameraFrame);

			for (int i = 0; i < MENU_BUT_NUM; i++) {
				//If blobs are found
				if (lbBlobs.size() == 1 && !isLevelButtonClicked) {

					//Check which button is clicked
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

	int frame_width = myCamera->getStream().get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = myCamera->getStream().get(CV_CAP_PROP_FRAME_HEIGHT);

	//VideoWriter video("level" + to_string(getLvl()-1) + "_timeline_button" + to_string(buttonId) + ".avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(frame_width, frame_height), true);

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

		pbMat = segmentROI(pbMat);

		findContours(pbMat, pbBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		if (pbBlobs.size() != 0) {

			pbBlobs = sortBlobs(pbBlobs);

			pbBlobs = filterBlobs(pbBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < pbBlobs.size(); i++)
			{
				Rect boundingBox = boundingRect(pbBlobs[i]);
				boundingBox += Point(pbROI.x, pbROI.y);

				rectangle(pbCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(pbCameraFrame, pbROI, Scalar(255, 0, 0), 2);

			//video.write(pbCameraFrame);

			//imshow("pbCam" + to_string(buttonId), pbCameraFrame);

			for (int i = 0; i < MENU_BUT_NUM; i++) {

				//If a blob is found
				if (pbBlobs.size() == 1 && !isTimelineButtonClicked) {

					//Play the corresponding part of the song
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
	thread.detach(); //detach thread
}

void detectCreditsOrTheoryBlobs(Camera* myCamera, Rect bROI, thread &thread, bool isInCredits) {

	int frame_width = myCamera->getStream().get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = myCamera->getStream().get(CV_CAP_PROP_FRAME_HEIGHT);

	//VideoWriter video("crThNextButton.avi", CV_FOURCC('M', 'J', 'P', 'G'), 15, Size(frame_width, frame_height), true);

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

		findContours(crMat, crBlobs, noArray(), RETR_LIST, CHAIN_APPROX_NONE);

		if (crBlobs.size() != 0) {

			crBlobs = sortBlobs(crBlobs);

			crBlobs = filterBlobs(crBlobs, 100, 500);

			//drawing bounding box
			Scalar color(188, 255, 255);
			for (size_t i = 0; i < crBlobs.size(); i++)
			{
				Rect boundingBox = boundingRect(crBlobs[i]);
				boundingBox += Point(bROI.x, bROI.y);

				rectangle(crCameraFrame, boundingBox, Scalar(255, 0, 0), 2);
			}

			//rectangle(crCameraFrame, bROI, Scalar(255, 0, 0), 2);

			//video.write(crCameraFrame);

			//imshow("crCam", crCameraFrame);

			//if blob is found
			if (crBlobs.size() == 1) {
				//If player is in credits screen go back to menu screen
				if (isInCredits) {
					isInCreditsOrTheory = false;
					cout << "Back!" << endl;
					setLvl(0);
					isInMenu = true;
					runLevel(getLvl());
				}
				//Else, player is in tutorial sequence
				else {
					//If there are more tutorial slides, load the next one
					if (getTut() <= 11 && lastSize == 0) {
						setTut(getTut() + 1);
						loadTutorial(getTut());
						lastSize = 1;
					}
					//Else, run the tutorial level
					else if (getTut() > 11 && lastSize == 0) {
						setTut(6); //6 for skipping theory slides/1 for including theory slides

						loadLevel(1); //Loads the tutorial level

						waitKey(1000); //Waits a second
						RepeatSong(); //Plays the song for the level

						waitKey(1100); //Waits a second
						playAllChords(); //Plays the chords for the level

						isInCreditsOrTheory = false;
						levelRunning = true; //Indicate that player has entered a level
						runLevelThreads(); //Run level threads
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

	thread.detach(); //Detach thread
}

void runMenuThreads() {

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//Create the ROIs for the menu screen and push them in the vector
	for (int i = 0; i < MENU_BUT_NUM; i++) {
		if (i % 2 == 0) {
			mbROIs.push_back(Rect(mbRoiStartX, mbRoiStartY, mbRoiW, mbRoiH));
		}
		else {
			mbROIs.push_back(Rect(mbRoiStartX + mbRoiShiftX, mbRoiStartY, mbRoiW, mbRoiH));
			mbRoiStartY += mbRoiShiftY;
		}

		//Run the menu threads
		mbThread[i] = thread(detectMenuBlobs, myCam, mbROIs[i], i + 1, ref(mbThread[i]));
	}
}

void runLevelThreads() {

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//make vector of card ROIs
	for (int i = 0; i < SLOTS_NUM; i++) {
		cardROIs.push_back(Rect(cRoiStartX, cRoiY, cRoiW, cRoiH));

		slots[i].id = i + 1; //Set id for the current slot

							 //Run card slot threads
		cardSlotThread[i] = thread(detectCards, myCam, cardROIs[i], ref(slots[i]), ref(cardSlotThread[i]));

		//Calculate the start X coordinate for next slot ROI
		cRoiStartX -= cRoiShiftX;
	}

	//make vector of level button ROIs
	for (int i = 0; i < 3; i++) {
		lbROIs.push_back(Rect(lbRoiStartX, lbRoiY, lbRoiW, lbRoiH));

		//Run level button threads
		lbThread[i] = thread(detectLevelButtonBlobs, myCam, lbROIs[i], i + 1, ref(lbThread[i]));

		//Calculate the start X coordinate for next level button ROI
		lbRoiStartX += lbRoiShiftX;
	}

	//Make vector of timeline button ROIs
	for (int i = 0; i < 4; i++) {
		pbROIs.push_back(Rect(pbRoiStartX, pbRoiY, pbRoiW, pbRoiH));

		//Run timeline button threads
		pbThread[i] = thread(detectTimelineButtonBlobs, myCam, pbROIs[i], 4 - i, ref(pbThread[i]));

		//Calculate the start X coordinate for next timeline button ROI
		pbRoiStartX += pbRoiShiftX;
	}
}

void runCreditsOrTheoryThread(bool isInCredits) {

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}
	//Make ROI for the next button in tutorial sequence (or the back button in Credits screen)
	crThROI = Rect(crRoiX, crRoiY, crRoiW, crRoiH);

	//Run the thread
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

		setTut(6); //6 for skipping theory slides/1 for including theory slides
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


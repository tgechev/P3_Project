#include <iostream>
#include <opencv2\opencv.hpp> 
#include "Levels.h"
#include "Cards.h"

using namespace std;
using namespace cv;

//Global
int level = 0;
int roiStartX = 470, roiShiftX = 137, roiY = 172, roiW = 100, roiH = 100;

int getLvl() {
	return level;
}

void setLvl(int lvl) {

	level = lvl;
}

int main()
{
	Camera* myCam = new Camera();
	vector<Rect> cardROIs;

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//make vector of card ROIs
	for (int i = 0; i < 4; i++) {
	cardROIs.push_back(Rect(roiStartX, roiY, roiW, roiH));

	roiStartX -= roiShiftX;
	}

	thread cardDetectThread(detectCards, myCam, cardROIs[0]);

	

	checkLevel(getLvl());

	cardDetectThread.detach();

	while (true) {


	}


	

	return 0;
}
#include <iostream>
#include <opencv2\opencv.hpp> 
#include "Levels.h"
#include "Cards.h"

using namespace std;
using namespace cv;


//Global
int roiStartX = 470, roiShiftX = 137, roiY = 172, roiW = 100, roiH = 100;

thread cardSlotThread[SLOTS_NUM];

vector<cardSlot> slots(SLOTS_NUM);




int main()
{
	Camera* myCam = new Camera();
	vector<Rect> cardROIs;

	if (!myCam->getStream().isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//make vector of card ROIs
	for (int i = 0; i < SLOTS_NUM; i++) {
	cardROIs.push_back(Rect(roiStartX, roiY, roiW, roiH));

	slots[i].id = i+1;

	cardSlotThread[i] = thread(detectCards, myCam, cardROIs[i], ref(slots[i]), ref(cardSlotThread[i]));

	roiStartX -= roiShiftX;
	}

	//thread cardDetectThread(detectCards, myCam, cardROIs[0]);

	

	checkLevel(getLvl());

	//cardDetectThread.detach();

	while (true) {


	}


	

	return 0;
}
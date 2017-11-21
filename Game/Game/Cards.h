#pragma once

#define _USE_MATH_DEFINES

#include "Camera.h"
#include <math.h>
#include <iostream>
#include <thread>

using namespace cv;
using namespace std;

struct cardSlot {
	int chord;
	int id;
};

const int SLOTS_NUM = 4;

//ROI corrdinates
const int roiShiftX = 137, roiY = 172, roiW = 100, roiH = 100;
extern int roiStartX;

extern bool levelRunning;

extern thread cardSlotThread[];

extern vector<cardSlot> slots;

extern vector<Rect> cardROIs;

extern Camera* myCam;

vector<int> getCurCards();

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread);

void runCardThreads();

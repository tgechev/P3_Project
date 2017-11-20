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

//thread cardSlotThread[SLOTS_NUM];

vector<int> getCurCards();

//vector<cardSlot> slots(SLOTS_NUM);

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread);

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
const int MENU_BUT_NUM = 6;

//ROI corrdinates
const int cRoiShiftX = 137, cRoiY = 172, cRoiW = 100, cRoiH = 100;
const int mbRoiShiftX = 430, mbRoiShiftY = 67, mbRoiW = 150, mbRoiH = 50;
extern int cRoiStartX, mbRoiStartX, mbRoiStartY;

extern bool levelRunning;

extern thread cardSlotThread[];
extern thread mbThread[];

extern vector<cardSlot> slots;

extern vector<Rect> cardROIs;
extern vector<Rect> mbROIs;

extern Camera* myCam;

vector<int> getCurCards();

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread);

void runCardThreads();

void mbBlobDetect(Rect mbROI, int buttonId, thread &thread);

void runMenuThreads();

vector<vector<Point>> sortBlobs(vector<vector<Point>>);

Mat segmentFrame(Mat frame);

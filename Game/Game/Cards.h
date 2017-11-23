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
const int MENU_EXIT = 6, MENU_TUTORIAL = 2, MENU_LVL1 = 1, MENU_LVL2 = 3, MENU_LVL3 = 5, MENU_CREDITS = 4;

//ROI corrdinates
const int cRoiShiftX = 137, cRoiY = 172, cRoiW = 100, cRoiH = 100;
const int mbRoiShiftX = 430, mbRoiShiftY = 67, mbRoiW = 150, mbRoiH = 50;
const int lbRoiShiftX = 162, lbRoiY = 273, lbRoiW = 80, lbRoiH = 50;
extern int cRoiStartX, mbRoiStartX, mbRoiStartY, lbRoiStartX;

extern bool levelRunning;
extern bool isInMenu;

extern thread cardSlotThread[];
extern thread mbThread[];

extern vector<cardSlot> slots;

extern vector<Rect> cardROIs;
extern vector<Rect> mbROIs;

extern Camera* myCam;

vector<int> getCurCards();

vector<vector<Point>> sortBlobs(vector<vector<Point>>);

Mat segmentROI(Mat frame);

void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread);

void detectMenuBlobs(Camera* myCamera, Rect mbROI, int buttonId, thread &thread);

void detectLevelButtonBlobs(Camera* myCamera, Rect lbROI, int buttonId, thread &thread);

void runMenuThreads();

void runCardThreads();

void runLevelButtonThreads();

void checkMenuButton(int buttonId);

void checkLevelButton(int buttonId);

/*Header file containing the declarations of every function, constant and variable responsible for the Image Processing algorithm*/
#pragma once

#define _USE_MATH_DEFINES //Definition needed for using math constants like M_PI

#include "Camera.h"
#include <math.h>
#include <iostream>
#include <thread>

using namespace cv;
using namespace std;

//A struct representing a card slot
//'chord' - the currently placed chord/card in the slot
//'id' - the id of the slot /1st, 2nd, 3rd or 4th/
struct cardSlot {
	int chord;
	int id;
};

//Constants for number of slots, menu buttons and menu button IDs
const int SLOTS_NUM = 4;
const int MENU_BUT_NUM = 6;
const int MENU_EXIT = 6, MENU_TUTORIAL = 2, MENU_LVL1 = 1, MENU_LVL2 = 3, MENU_LVL3 = 5, MENU_CREDITS = 4;

//Constants defining the different ROI coordinates
//Prefixes:
// c - card ROI
// mb - menu button ROI
// lb - level button ROI /repeat, confirm and back buttons/
// pb - play button ROI /buttons under the timeline in the level screen/
// cr - credits button ROI /ROI for the back button in the credits screen as well as for the next button in the tutorial pages/

const int cRoiShiftX = 137, cRoiY = 172, cRoiW = 100, cRoiH = 100;
const int mbRoiShiftX = 430, mbRoiShiftY = 67, mbRoiW = 150, mbRoiH = 50;
const int lbRoiShiftX = 162, lbRoiY = 273, lbRoiW = 80, lbRoiH = 50;
const int pbRoiShiftX = 128, pbRoiY = 90, pbRoiW = 52, pbRoiH = 35;
const int crRoiX = 45, crRoiY = 375, crRoiW = 150, crRoiH = 55;

//Variables needed for different ROI coordinates, when ROIs are created the values of these variables is changing depending on which ROI is being created
extern int cRoiStartX, mbRoiStartX, mbRoiStartY, lbRoiStartX, pbRoiStartX;

//Variable for the threshold value in segmentROI() function
extern int segmentTh;

//Boolean variables used to check where in the game the player currently is and either run or stop required threads.
extern bool levelRunning;
extern bool isInMenu;
extern bool isInCreditsOrTheory;

//Arrays which contain the threads
extern thread cardSlotThread[];
extern thread mbThread[];
extern thread pbThread[];

//Vector containing the card slots
extern vector<cardSlot> slots;

//Vectors for the different ROIs
extern vector<Rect> cardROIs;
extern vector<Rect> mbROIs;
extern vector<Rect> pbROIs;

//Pointer to the camera
extern Camera* myCam;

//Function declarations

//Getter function which returns the currently placed cards by the player
vector<int> getCurCards();

//Function for sorting the found BLOBs
vector<vector<Point>> sortBlobs(vector<vector<Point>> blobs);

//Function which filters the BLOBs depending on their area and circularity
vector<vector<Point> > filterBlobs(vector<vector<Point> > blobs, int minArea, int maxArea);

//Function for segmenting the different ROIs
Mat segmentROI(Mat ROI);

//Function for convenience, which saves pictures after every segmentation step.
//Mat segmentROI(Mat ROI, int buttonId);

//Function implementing histogram stretching
Mat histogramStretching(Mat ROI);

//Function running on different threads, responsible for detecting the cards
void detectCards(Camera* myCamera, Rect cardROI, cardSlot &slot, thread &thread);

//Function running on different threads, responsible for detecting a press on a menu button
void detectMenuBlobs(Camera* myCamera, Rect mbROI, int buttonId, thread &thread);

//Function running on different threads, responsible for detecting a press on a level button
void detectLevelButtonBlobs(Camera* myCamera, Rect lbROI, int buttonId, thread &thread);

//Function running on different threads, responsible for detecting a press on a timeline button
void detectTimelineButtonBlobs(Camera* myCamera, Rect pbROI, int buttonId, thread &thread);

//Function running on different threads, responsible for detecting a press on the button in the credits screen or the next button in the tutorial sequence
void detectCreditsOrTheoryBlobs(Camera* myCamera, Rect bROI, thread &thread, bool isInCredits);

//Function which runs the threads for every menu button
void runMenuThreads();

//Function which runs the threads for every level button, timeline button and card slot
void runLevelThreads();

//Function which runs the threads for the back button in credits screen or next button in tutorial sequence
void runCreditsOrTheoryThread(bool isInCredits);

//Function checking which menu button has been pressed
void checkMenuButton(int buttonId);

//Function checking which level button has been pressed
void checkLevelButton(int buttonId);

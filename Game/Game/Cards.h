#ifndef Cards 
#define Cards

#define _USE_MATH_DEFINES

#include "Camera.h"
#include <math.h>
#include <iostream>
#include <thread>

using namespace cv;
using namespace std;

//vector<vector<Point> > foundBlobs;
vector<int> getCurCards();

void detectCards(Camera* myCamera, Rect cardROI);	

#endif
#ifndef Levels 
#define Levels

#include <opencv2\opencv.hpp> 
#include <iostream> 
#include <windows.h>
using namespace cv;
using namespace std;


void checkLevel(int lvl);
void mainMenu();
void loadLevel();
void callBackFuncMenu(int event, int x, int y, int flags, void* userdata);
void Confirm(vector<int> vectInput, int level);
void callBackFunc(int event, int x, int y, int flags, void* userdata);

#endif


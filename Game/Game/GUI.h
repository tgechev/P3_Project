#ifndef GUI_H
#define GUI_H

#include <opencv2\opencv.hpp> 
#include <iostream> 
using namespace cv;
using namespace std;

extern Mat testTemp;
//declare the functions so they can be reference to other files
void mainMenu();
void loadLevel(int lvl);
void loadCredits();
void callBackFuncCredits(int event, int x, int y, int flags, void* userdata);
void callBackFuncMenu(int event, int x, int y, int flags, void* userdata);
void Confirm(vector<int> vectInput, int level, Mat orgImg);
void callBackFunc(int event, int x, int y, int flags, void* userdata);
void loadTutorial(int ntut);
void callBackFuncTutorial(int event, int x, int y, int flags, void* userdata);
void setTextChords(int lvl, Mat image);
#endif
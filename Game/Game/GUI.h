#ifndef GUI 
#define GUI

#include <opencv2\opencv.hpp> 
#include <iostream> 
using namespace cv;
using namespace std;


void mainMenu();
void loadLevel();
void loadCredits();
void callBackFuncCredits(int event, int x, int y, int flags, void* userdata);
void callBackFuncMenu(int event, int x, int y, int flags, void* userdata);
void Confirm(vector<int> vectInput, int level);
void callBackFunc(int event, int x, int y, int flags, void* userdata);
void settext(Mat img, int lvl);
void loadTutorial(int ntut);
void callBackFuncTutorial(int event, int x, int y, int flags, void* userdata);
//Add settext for artist, song, chords(in the bottom) based on library in levels.
#endif
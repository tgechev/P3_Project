#ifndef Levels 
#define Levels

#include <opencv2\opencv.hpp> 
#include <iostream> 
//using namespace cv;
using namespace std;


void checkLevel(int lvl);
int getLvl();
void setLvl(int lvl);
vector<int> getCurCards();
vector<int> getCorrectCards();
//void checkTutorial(int ntut);
int getTut();
void setTut(int ntut);

//add a getArtist, getSong, and getChordsStr, which is passed in strings and vectors.

#endif


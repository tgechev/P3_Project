#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
#include <MMSystem.h>
#include "MP3Player.h"
#include "Levels.h"

using namespace cv;
using namespace std;

//Global
int level = 0;


int getLvl() {
	return level;
}

void setLvl(int lvl) {

	level = lvl;
}




int main()
{
	checkLevel(getLvl());
	return 0;
}
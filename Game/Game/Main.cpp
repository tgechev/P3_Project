#include <iostream>
#include <opencv2\opencv.hpp> 
#include "Levels.h"

using namespace std;
using namespace cv;

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
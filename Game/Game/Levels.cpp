#include "Levels.h"
//#include <opencv2\opencv.hpp>
//#include <iostream>
//#include <chrono>
//#include <thread>
//#include <functional>
#include "GUI.h"
//using namespace cv;
//using namespace std;

vector<int> correctCards = { 1,2,3,4 };

vector<int> getCorrectCards() {
	return correctCards;
}

int level = 0;
int tutlevel = 1;

//LEVEL STUFF
int getLvl() {
	return level;
}

void setLvl(int lvl) {
	level = lvl;
}

//TUTORIAL STUFF
int getTut() {
	return tutlevel;
}

void setTut(int tut) {

	tutlevel = tut;
}

void checkLevel(int lvl)
{
	switch (lvl) {
	case 0:
		mainMenu();
		break;
	case 1:
		cout << "Tutorial level!" << endl;
		loadTutorial(getTut());
		break;
	case 2:
		cout << "level 1" << endl;
		loadLevel();
		break;
	case 3:
		cout << "Level 2" << endl;
		loadLevel();
		break;
	case 4:
		cout << "Level 3" << endl;
		loadLevel();
		break;
	case 5:
		cout << "Credits" << endl;
		loadCredits();
		break;
	default:
		cout << "Invalid level" << endl;
	}

}







	





			
			
		
			
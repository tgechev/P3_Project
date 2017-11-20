#include "Levels.h"
//#include <opencv2\opencv.hpp>
//#include <iostream>
//#include <chrono>
//#include <thread>
//#include <functional>
#include "GUI.h"
//using namespace cv;
//using namespace std;


vector<int> getCorrectCards(int lvl) {
	vector<int> correctCards;

	switch (lvl)
	{
	case 1:
		correctCards = { 1, 4, 5, 0 };
		break;
	case 2:
		correctCards = { 1, 4, 7, 0 };
		break;
	case 3: 
		correctCards = { 1, 3, 4, 5 };
		break;
	case 4: 
		correctCards = { 1, 5, 6, 7 };
		break;
	default:
		break;
	}
	return correctCards;
}

void checkLevel(int lvl)
{
	switch (lvl) {
	case 0:
		mainMenu();
		break;
	case 1:
		cout << "Tutorial level!" << endl;
		loadLevel();
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






	





			
			
		
			
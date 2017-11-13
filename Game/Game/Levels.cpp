#include "Levels.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
#include "Main.h"
#include "Sound.h"
using namespace cv;
using namespace std;

//Global-ish
Rect btnExit;

//mainMenu
Rect btnTutorial;
Rect btnOptions;
Rect btnLevel1;
Rect btnLevel2;
Rect btnLevel3;

//level
Rect btnRepeat;
Rect btnConfirm;
Rect btnBack;
//Levels
Rect playButton1;
Rect playButton2;
Rect playButton3;
Rect playButton4;

//Credits
//Rect btnBack;


vector<int> curCards = { 1,2,3,4 };
vector<int> correctCards = { 1,5,3,4 };
vector<int> cardResults = { 0,0,0,0 };

Mat img;
Mat test;
Mat correct = imread("images/correct.png", CV_LOAD_IMAGE_COLOR);
Mat wrong = imread("images/wrong.png", CV_LOAD_IMAGE_COLOR);

//Window name
string winName = "EXCITING GAME";


void loadLevel() {

	Mat3b canvas;

	img = imread("images/bg.png", CV_LOAD_IMAGE_COLOR);
	test = imread("images/bg1.png", CV_LOAD_IMAGE_COLOR);
	//making the buttons
	btnRepeat = Rect(190, 457, 114, 62);
	btnConfirm = Rect(454, 457, 114, 62);
	btnBack = Rect(718, 457, 114, 62);
	playButton1 = Rect(168, 144, 70, 48);
	playButton2 = Rect(374, 144, 70, 48);
	playButton3 = Rect(577, 144, 70, 48);
	playButton4 = Rect(785, 144, 70, 48);


	canvas = Mat3b(img.rows + btnRepeat.height, img.cols, Vec3b(0, 0, 0));

	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);

	imshow(winName, img);
	waitKey(0);
}

void loadCredits() {

	Mat3b canvas;

	img = imread("images/bg_credits.jpg", CV_LOAD_IMAGE_COLOR);

	//making the buttons
	btnBack = Rect(751, 649, 215, 60);

	canvas = Mat3b(img.rows + btnRepeat.height, img.cols, Vec3b(0, 0, 0));

	namedWindow(winName);
	setMouseCallback(winName, callBackFuncCredits);

	imshow(winName, img);
	waitKey(0);
}



void checkLevel(int lvl)
{
	switch (lvl) {
	case 0:
		mainMenu();
		break;
	case 1:
		cout << "omggg level 1" << endl;
		loadLevel();
		break;
	case 2:
		cout << "level 2" << endl;
		break;
	case 3:
		cout << "Better try again" << endl;
		break;
	case 4:
		loadCredits();
		break;
	default:
		cout << "Invalid level" << endl;
	}
	waitKey(0);

}

void callBackFuncMenu(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnExit.contains(Point(x, y)))
		{
			cout << "Exitiiiitt" << endl;
			destroyAllWindows();
		}
		else if (btnTutorial.contains(Point(x, y)))
		{
			cout << "Tutorial" << endl;
		}
		else if (btnOptions.contains(Point(x, y)))
		{
			cout << "Credits" << endl;
			setLvl(4);
			checkLevel(getLvl());
		}
		else if (btnLevel1.contains(Point(x, y)))
		{
			cout << "Level 1" << endl;
			setLvl(1);
			checkLevel(getLvl());
		}
		else if (btnLevel2.contains(Point(x, y)))
		{
			cout << "Level 2" << endl;

		}
		else if (btnLevel3.contains(Point(x, y)))
		{
			cout << "Level 3" << endl;
		}

	}

	//imshow(winName, img);
	//waitKey(1);
}

void Confirm(vector<int> vectInput, int level) {


	for (int i = 0; i < 4; i++) {
		if (vectInput[i] == correctCards[i]) {
			//cout << "Input card: " << vectInput[i] << ". Correct card: " << correctCards[i] << ". They are matching." << endl;
			cardResults[i] = { 1 };

		}
		else {
			cardResults[i] = { 0 };
			//cout << "Input card: " << vectInput[i] << ". Correct card: " << correctCards[i] << ". They are not matching." << endl;

		}
	}
	//printing the results



	for (int i = 0; i<cardResults.size(); ++i) {
		cout << "The results are: " << cardResults[i] << endl;
		if (cardResults[i] == 1) {
			correct.copyTo(img(Rect(147 + (226 * i), 225, correct.cols, correct.rows)));

		}
		else {
			wrong.copyTo(img(Rect(147 + (226 * i), 225, correct.cols, correct.rows)));

		}

	}

	imshow(winName, img);
	cout << "Checked and confirmed!" << endl;
	waitKey(2000);
	imshow(winName, test);

}

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnRepeat.contains(Point(x, y)))
		{
			RepeatSong();
			cout << "Repeat!" << endl;
		}
		else if (btnConfirm.contains(Point(x, y)))
		{
			Confirm(curCards, getLvl());

			
		}
		else if (btnBack.contains(Point(x, y)))
		{
			cout << "Back!" << endl;
			setLvl(0);
			checkLevel(getLvl());
		}
		else if (playButton1.contains(Point(x, y)))
		{
			PlaySnippet(1);
		}
		else if (playButton2.contains(Point(x, y)))
		{
			cout << "Play 2" << endl;
			PlaySnippet(2);
		}
		else if (playButton3.contains(Point(x, y)))
		{
			PlaySnippet(3);
			cout << "Play 3" << endl;
		}
		else if (playButton4.contains(Point(x, y)))
		{
			cout << "Play 4" << endl;
			PlaySnippet(4);
		}

	}
	
}

void callBackFuncCredits(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnBack.contains(Point(x, y)))
		{
			cout << "Back!" << endl;
			setLvl(0);
			checkLevel(getLvl());
		}
	}
}

void mainMenu() {
	Mat3b canvas;
	//string winName = "MAIN MENU";
	//Mat img;

	img = imread("images/bg_menu.jpg", CV_LOAD_IMAGE_COLOR);
	//making the buttons
	btnTutorial = Rect(59, 240, 215, 60);
	btnOptions = Rect(59, 348, 215, 60);
	btnExit = Rect(59, 462, 215, 60);
	btnLevel1 = Rect(752, 240, 215, 60);
	btnLevel2 = Rect(752, 349, 215, 60);
	btnLevel3 = Rect(752, 462, 215, 60);

	// The canvas
	canvas = Mat3b(img.rows, img.cols, Vec3b(0, 0, 0));

	namedWindow(winName);
	setMouseCallback(winName, callBackFuncMenu);

	imshow(winName, img);

	// Wait until user press some key

}





	





			
			
		
			
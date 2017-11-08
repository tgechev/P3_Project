#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
using namespace cv;
using namespace std;
int level = 0;

int mainMenu();

int main()
{
	switch (level) {
	case 0:
		mainMenu();
		break;
	case 1:
		cout << "Well done" << endl;
		break;
	case 2:
		cout << "You passed" << endl;
		break;
	case 3:
		cout << "Better try again" << endl;
		break; 
	case 4:
		cout << "Better try again" << endl;
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
			cout << "Exit" << endl;
			destroyAllWindows();
		}
		else if (btnTutorial.contains(Point(x, y)))
		{
			cout << "Tutorial" << endl;
		}
		else if (btnOptions.contains(Point(x, y)))
		{
			cout << "Options" << endl;
		}
		else if (btnLevel1.contains(Point(x, y)))
		{
			cout << "Level 1" << endl;
			level = 1;
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

int mainMenu() {
	Mat3b canvas;
	string winName = "MAIN MENU";
	Mat img;
	Rect btnTutorial;
	Rect btnOptions;
	Rect btnExit;
	Rect btnLevel1;
	Rect btnLevel2;
	Rect btnLevel3;
	Rect btnRepeat;
	Rect btnConfirm;
	img = imread("images/bg_menu.jpg", CV_LOAD_IMAGE_COLOR);
	//making the buttons
	btnTutorial = Rect(54, 192, 169, 47);
	btnOptions = Rect(54, 277, 169, 47);
	btnExit = Rect(54, 366, 169, 47);
	btnLevel1 = Rect(598, 192, 169, 47);
	btnLevel2 = Rect(598, 277, 169, 47);
	btnLevel3 = Rect(598, 366, 169, 47);

	// The canvas
	canvas = Mat3b(img.rows, img.cols, Vec3b(0, 0, 0));

	// Draw buttons
	/*
	img(btnRepeat) = Vec3b(200, 200, 200);
	img(btnConfirm) = Vec3b(200, 200, 200);
	img(btnExit) = Vec3b(200, 200, 200);
	*/
	namedWindow(winName);
	setMouseCallback(winName, callBackFuncMenu);


	imshow(winName, img);


	// Wait until user press some key
	

	return 0;

}




#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
using namespace cv;
using namespace std;


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

	imshow(winName, img);
	//waitKey(1);
}

int main()
{
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
	waitKey(0);

	return 0;

}

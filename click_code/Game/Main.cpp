#include <opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;


Mat3b canvas;
string winName = "EXCITING GAME";

Mat img;

Rect btnRepeat;
Rect btnConfirm;
Rect btnExit;


void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnRepeat.contains(Point(x, y)))
		{
			cout << "Repeat!" << endl;
		}
		else if (btnConfirm.contains(Point(x, y)))
		{
			cout << "Confirm!" << endl;
		}
		else if (btnExit.contains(Point(x, y)))
		{
			cout << "Exit!" << endl;
			destroyAllWindows();
		}
	}

	//imshow(winName, img);
	//waitKey(1);
}

int main()
{
	img = imread("images/bg.png", 0);
	//making the buttons
	btnRepeat = Rect(80, 410, 120, 70);
	btnConfirm = Rect(340, 410, 120, 70);
	btnExit = Rect(600, 410, 120, 70);

	// The canvas
	canvas = Mat3b(img.rows + btnRepeat.height, img.cols, Vec3b(0, 0, 0));

	// Draw buttons
	/*
	img(btnRepeat) = Vec3b(200, 200, 200);
	img(btnConfirm) = Vec3b(200, 200, 200);
	img(btnExit) = Vec3b(200, 200, 200);
	*/
	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);

	
	imshow(winName, img);
	waitKey(0);

	return 0;
}
#include <opencv2\opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <functional>
using namespace cv;
using namespace std;
using namespace std::chrono_literals;


Mat3b canvas;
string winName = "EXCITING GAME";

Mat img;

Rect btnRepeat;
Rect btnConfirm;
Rect btnExit;

Rect timeline1

Rect timeline2;
Rect timeline3;
Rect timeline4;

Point position;


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
}



int main()
{

	img = imread("D:/bg.png", CV_LOAD_IMAGE_COLOR);
	//making the buttons
	btnRepeat = Rect(80, 410, 120, 70);
	btnConfirm = Rect(340, 410, 120, 70);
	btnExit = Rect(600, 410, 120, 70);

	//making the timeline rectangles
	timeline1 = Rect(106, 113, 203, 31);
	timeline2 = Rect(309, 113, 203, 31);
	timeline3 = Rect(512, 113, 203, 31);
	timeline4 = Rect(715, 113, 202, 31);

	
	// The canvas
	canvas = Mat3b(img.rows + timeline1.height, img.cols, Vec3b(0, 0, 0));

	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);


	int i = 0;

	while (true) {

		//cout << i;

		//imshow(winName, img);

		if (waitKey(30) >= 0)
			break;

		if (i < 9) {

			//this_thread::sleep_for(1s);
			position = Point(i, 10);
			cout << i;
			if (i == 0) {
				//img(timeline1) = Vec3b(200, 0, 0);
				rectangle(img, timeline1, Scalar(250, 0, 200), -1);
			}
			else if (i == 2) {
				img(timeline2) = Vec3b(250, 0, 200);
			}
			else if (i == 4) {
				img(timeline3) = Vec3b(250, 0, 200);
			}
			//else if (i == 10) {
				//img(timeline4) = Vec3b(250, 0, 200);
			//}

			this_thread::sleep_for(1s);
			i = i + 1;
		}


	}

return 0;
}
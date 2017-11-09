#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
using namespace cv;
using namespace std;

//Global
int level = 0;

//Level 1
string winName = "EXCITING GAME";

Mat bg;

Mat correct = imread("images/correct.png", 0);
Mat wrong = imread("images/wrong.png", 0);

Rect btnRepeat;
Rect btnConfirm;
Rect btnBack;

vector<int> curCards = { 1,2,3,4 };
vector<int> correctCards = { 1,5,3,4 };
vector<int> cardResults = { 0,0,0,0 };

//mainMenu
Rect btnTutorial;
Rect btnOptions;
Rect btnExit;
Rect btnLevel1;
Rect btnLevel2;
Rect btnLevel3;

void mainMenu();
void loadLevel();

void checkLevel() {

	switch (level) {
	case 0:
		mainMenu();
		break;
	case 1:
		cout << "omggg level 1" << endl;
		loadLevel();
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
			cout << "Exitiiiitt" << endl;
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
			checkLevel();
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

void mainMenu() {
	Mat3b canvas;
	string winName = "MAIN MENU";
	Mat img;
	
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

	namedWindow(winName);
	setMouseCallback(winName, callBackFuncMenu);

	imshow(winName, img);

	// Wait until user press some key
	
}

void Confirm(vector<int> vectInput, int level) {


	for (int i = 0; i <= 3; i++) {
		if (vectInput[i] == correctCards[i]) {
			cout << "Input card: " << vectInput[i] << ". Correct card: " << correctCards[i] << ". They are matching." << endl;
			cardResults[i] = { 1 };
		}
		else {
			cardResults[i] = { 0 };
			cout << "Input card: " << vectInput[i] << ". Correct card: " << correctCards[i] << ". They are not matching." << endl;
		}
	}
	//printing the results
	for (int i = 0; i<cardResults.size(); ++i) {
		cout << "The results are: " << cardResults[i] << endl;
		correct.copyTo(bg(Rect(100 * i, 100, correct.cols, correct.rows)));
		cout << "smid et fucking billede ind altsåååå" << endl;
	}
}

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
			Confirm(curCards, level);
			cout << "Checked and confirmed!" << endl;
		}
		else if (btnBack.contains(Point(x, y)))
		{
			cout << "Exit!" << endl;
			destroyAllWindows();
		}
	}

}

void loadLevel() {
	Mat3b canvas;
	//string winName = "EXCITING GAME";

	//Mat bg;

	bg = imread("images/bg.png", 0);

	//making the buttons
	btnRepeat = Rect(80, 410, 120, 70);
	btnConfirm = Rect(340, 410, 120, 70);
	btnBack = Rect(600, 410, 120, 70);

	canvas = Mat3b(bg.rows + btnRepeat.height, bg.cols, Vec3b(0, 0, 0));

	// Draw buttons
	
	bg(btnRepeat) = Vec3b(200, 200, 200);
	bg(btnConfirm) = Vec3b(200, 200, 200);
	bg(btnBack) = Vec3b(200, 200, 200);

	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);

	imshow(winName, bg);
	waitKey(0);
}



int main()
{
	checkLevel();
	return 0;
}
#include <iostream>
#include <opencv2\opencv.hpp> 
#include "Levels.h"
#include "Cards.h"

using namespace std;
using namespace cv;

int main()
{
	int misreadChord = 0;

	runLevel(getLvl());

	

	while (true) {
		int keycode = waitKey(30);
		if (keycode == 's') {
			Camera* myCam = new Camera();

			myCam->getStream().set(CV_CAP_PROP_SETTINGS, 0.0);
		}
		else if (keycode == '+') {
			misreadChord++;
			cout << "misread chord: " << misreadChord << endl;
		}
		else if (keycode == ' ') {
			cout << "misread chord: "<<misreadChord << endl;
		}
		else if (keycode == '-') {
			misreadChord = 0;
		}
	}

	return 0;
}
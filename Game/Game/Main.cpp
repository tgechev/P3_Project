/*Main.cpp file which initially runs the menu screen of the game and creates a trackbar for changing the threshold
value in the segmentROI() function in BLOB.cpp
It also has an infinite loop which includes an if statement which listens for a keypress and increments a variable called misreadChord.
When '+' is pressed on the keyboard the variable is incremented, when '-' is pressed the variable is set to 0 and when spacebar is pressed the variable
is printed out in the console.*/

#include <iostream>
#include <opencv2\opencv.hpp> 
#include "Levels.h"
#include "BLOB.h"

using namespace std;
using namespace cv;

int main()
{
	//Creates a window named Control which will hold the trackbar for changing the threshold value
	namedWindow("Control", CV_WINDOW_NORMAL);

	//Creates the trackbar with the name "threshold" in the window "Control"
	//it changes the value of the segmentTh variable, defined in BLOB.cpp, which is passed by reference here
	createTrackbar("Threshold", "Control", &segmentTh, 179);

	//counter variable for misread chords by the system
	int misreadChord = 0;

	//Running the menu screen for the first time
	runLevel(getLvl());


	//Infinite loop which listens for a key press
	//Also when 's' is pressed a window with the camera's settings is shown
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
			cout << "misread chord: " << misreadChord << endl;
		}
		else if (keycode == '-') {
			misreadChord = 0;
		}
	}

	return 0;
}
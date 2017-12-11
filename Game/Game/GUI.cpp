#include "GUI.h"
#include "Levels.h"
#include "Sounds.h"
#include "BLOB.h"

//Main-menu buttons
Rect btnTutorial;
Rect btnCredits;
Rect btnLevel1;
Rect btnLevel2;
Rect btnLevel3;
Rect btnExit;

//The next button for the tutorial
Rect btnNext;

//Level buttons, at the bottom of the screen
Rect btnRepeat;
Rect btnConfirm;
Rect btnBack;
//Timeline buttons in levels
Rect playButton1;
Rect playButton2;
Rect playButton3;
Rect playButton4;

//Vector that is declared to contain the information from the cards
vector<int> cardResults = { 0,0,0,0 };
//vector that determine the winning-state of the cards.
vector<int> cardResultsPositive = { 1,1,1,1 };
//two-dimentional vector that holds information about the chords, based on the level.
vector< vector<string> > chordsVec;
//two-dimentional vector that holds information about the song title and artist, based on the level.
vector< vector<string> > songVec;
//Text-vector that holds the text displayed at the top of the level.
vector<string> lvlVec;
//Text vectors that holds the roman numerals, that is displayed at the bottom of the screen.
vector<string> RNumVec;

//Window name
string winName = "EXCITING GAME";

//Matrix that will hold the background image.
Mat img;
//temporary matrix that will copy the original background matrix, so it doesn't get overridden.
Mat testTemp;
//Matrix that holds an image of a 'correct' symbol.
Mat correct = imread("images/Correct.png", IMREAD_COLOR);
//Matrix that holds an image of a 'wrong' symbol.
Mat wrong = imread("images/Wrong.png", IMREAD_COLOR);

//the function below, loads the level data into the different vectors, that are already declared above.
void getTextData() {
	vector<string> LvlT = { "G", "Am", "Bm", "C", "D", "Em", "F#dim" };
	vector<string> Lvl1 = { "E5", "F5", "G5", "A5", "B5", "C5", "D5" };
	vector<string> Lvl2 = { "Dm", "Edim", "F", "Gm", "Am", "Bb", "C" };
	vector<string> Lvl3 = { "Am", "Bdim", "C", "Dm", "E7", "F", "G" };
	vector<string> songT = { "Bob Dylan", "Knocking on heaven's door" };
	vector<string> song1 = { "AC/DC", "Back in Black" };
	vector<string> song2 = { "Madonna", "La Isla Bonita" };
	vector<string> song3 = {"Ray Charles", "Hit the road, Jack"};
	lvlVec = {"Tutorial", "Level 1", "Level 2", "Level 3"};

	RNumVec = { "I", "II", "III", "IV", "V", "VI", "VII" };
	//the information is loaded into another vector, causing them to be two-dimentional.
	chordsVec = {LvlT, Lvl1, Lvl2, Lvl3};
	songVec = { songT, song1, song2, song3 };
}

//the function below, sets the text for the song title and artist onto the matrix that is defined, based on the lvl.
void setTextSongInfo(int lvl, Mat image) {
	//gets the textdata
	getTextData();
	//settings font properties
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 2;
	int baseline = 0;
	//loads the data from
	string artist = songVec.at(lvl-1).at(0);
	string song = songVec.at(lvl-1).at(1);
	baseline += thickness;
	//setting the location of where to place the text
	Point textSong(50, 50);
	Point textArtist(50, 70);
	//Placing the text on the image, based on the variables above.
	putText(image, song, textSong, fontFace, fontScale+0.3, Scalar::all(255), thickness, 8);
	putText(image, artist, textArtist, fontFace, fontScale, Scalar::all(255), thickness, 8);

}
//Function to add the chord-text in the levels. It is slightly different from the other funtion above.
void setTextChords(int lvl, Mat image) {
	//Getting text data.
	getTextData();
	//Creating placeholder variables.
	string chord = "";
	string RNumText = "";
	//font parameters
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.9;
	double fontscaleRNum = 1.4;
	int thickness = 2;
	int baseline = 0;
	//The for-loop goes through the chords vectors based on the currect level
	for (size_t i = 0; i < chordsVec.at(lvl - 1).size(); i++) {
		//setting the values from the chords vector and roman numeral vector.
		chord = chordsVec.at(lvl - 1).at(i);
		RNumText = RNumVec.at(i);
		//Since some of the chords and numerals contain more characters than others, the textsize is calculated to later place the text in the center of the specific location.
		Size textSize = getTextSize(chord, fontFace, fontScale, thickness, &baseline);
		Size textSizeRnum = getTextSize(RNumText, fontFace, fontscaleRNum, thickness, &baseline);
		//As the different values needs to be placed a 7 different location at the bottom of the screen, x-axis is calculated with an eighth of the screen width and the multiplied with the currect number of iteration. Y-axis is manually set.
		Point textOrg((((image.cols / 8) * (i + 1)) - (textSize.width / 2)), 610);
		Point textRum((((image.cols / 8) * (i + 1)) - (textSizeRnum.width / 2)), 680);
		//the text is then placed onto the image.
		putText(image, chord, textOrg, fontFace, fontScale, Scalar::all(255), thickness, 8);
		putText(image, RNumText, textRum, fontFace, fontscaleRNum, Scalar::all(255), thickness, 8);
	}
}
//setting the level text, that is shown at he top of each level.
void setLvlText(int lvl, Mat image) {
	getTextData();
	
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 2;
	int baseline = 0;
	string lvltext = lvlVec.at(lvl-1);
	Size textSize = getTextSize(lvltext, fontFace, fontScale, thickness, &baseline);
	baseline += thickness;
	Point textOrg(image.cols/2-textSize.width, 50);
	putText(image, lvltext, textOrg, fontFace, fontScale + 0.3, Scalar::all(255), thickness, 8);
}

void loadTutorial(int ntut) {
	//instantiate matrix for the tutorial iamge
	Mat tutorial;
	//loads the tutorial image into the matrix.
	tutorial = imread("images/Tutorial/Tutorial" + to_string(ntut) +".jpg", CV_LOAD_IMAGE_COLOR);


	//making the button "next" that goes through the slides of the tutorial.
	btnNext = Rect(731, 615, 220, 78);
	//create a window
	namedWindow(winName);
	//Setting the window to respond to clicks
	setMouseCallback(winName, callBackFuncTutorial);
	//Show the image in the window.
	imshow(winName, tutorial);

	//Boolean to check if game currently is in a menu-state.
	isInMenu = false;
	
}
//Function to load level based on a integer called "lvl".
void loadLevel(int lvl) {
	//create temporary matrix to store background image.
	Mat temp;
	//The if-statement is to change between the 3 and 4 chords songs, as the 3 chord songs only displays 3 slots.
	if ((lvl == 1) || (lvl == 2)) {
		img = imread("images/bg3chords.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else {
		img = imread("images/bg.jpg", CV_LOAD_IMAGE_COLOR);
	}
	//copies the background image to temp matrix.
	temp = img.clone();

	//making the button sizes and locations.
	btnRepeat = Rect(190, 457, 114, 62);
	btnConfirm = Rect(454, 457, 114, 62);
	btnBack = Rect(718, 457, 114, 62);
	playButton1 = Rect(168, 144, 70, 48);
	playButton2 = Rect(374, 144, 70, 48);
	playButton3 = Rect(577, 144, 70, 48);
	playButton4 = Rect(785, 144, 70, 48);
	//create window
	namedWindow(winName);
	//setting the window to respond to mouseclicks.
	setMouseCallback(winName, callBackFunc);
	//Setting text and visuals.
	setTextChords(lvl, temp);
	setTextSongInfo(lvl, temp);
	setLvlText(lvl, temp);
	testTemp = temp.clone();
	imshow(winName, temp);

	isInMenu = false;
}

//loads credits
void loadCredits() {
	//loads credits image
	img = imread("images/bg_credits.jpg", CV_LOAD_IMAGE_COLOR);

	//making the back button
	btnBack = Rect(751, 649, 215, 60);
	//create window
	namedWindow(winName);
	//setting the window to respond to mouse clicks
	setMouseCallback(winName, callBackFuncCredits);
//shows the image
	imshow(winName, img);
	//boolean that determines if the game is in menu state.
	isInMenu = false;
}

//confirm function that runs when the confirm button is pressed in a level
void Confirm(vector<int> vectInput, int level, Mat orgImg) {
	//clone background image to temporary matrix
	Mat temp;
	temp = orgImg.clone();
	//goes through the card input and compares to the correct order of chords.
	for (int i = 0; i < 4; i++) {
		if (vectInput[i] == getCorrectCards(level)[i]) {
			cardResults[i] = { 1 };
		}
		else {
			cardResults[i] = { 0 };
		}
	}
	//printing the results
	if (cardResults == cardResultsPositive) {
		//if the results are matching
		cout << "The results are correct" << endl;
		CorrectCards();
	}
	else {
		//if they aren't matching
		cout << "The results are NOT correct" << endl;
		InCorrectCards();
	}
	//pastes the different wrong or correct symbols to the background image, based on the previous vector.
	for (size_t i = 0; i<cardResults.size(); ++i) {
		cout << "The results are: " << cardResults[i] << endl;
		if (cardResults[i] == 1) {
			correct.copyTo(temp(Rect(147 + (226 * i), 225, correct.cols, correct.rows)));
		}
		else {
			wrong.copyTo(temp(Rect(147 + (226 * i), 225, correct.cols, correct.rows)));
		}
	}
	//show the image with the symbols
	imshow(winName, temp);
	//make the game wait 2 seconds
	waitKey(2000);
	//show the original background image
	imshow(winName, orgImg);

}

//the callback function that occurs when clicking in a level.
void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		//these if-statement checks is there has been clicked inside any of the rectangle areas.
		if (btnRepeat.contains(Point(x, y)))
		{
			//the song will be played.
			RepeatSong();
		}
		else if (btnConfirm.contains(Point(x, y)))
		{
			//The confirm function will run, where it takes the current card values, current level and current backgroun matrix.
			Confirm(getCurCards(), getLvl(), testTemp);

		}
		else if (btnBack.contains(Point(x, y)))
		{
			//Setting the current lvl to 0.
			setLvl(0);
			//resetting the tutorial.
			setTut(6); //6 for skipping theory slides/1 for including theory slides
			//disable the level
			levelRunning = false;
			//check the lvl (which is current 0) and goes to the main menu.
			runLevel(getLvl());
		}
		else if (playButton1.contains(Point(x, y)))
		{
			//play song snippet 1
			PlaySnippet(1);
		}
		else if (playButton2.contains(Point(x, y)))
		{
			//play song snippet 2
			PlaySnippet(2);
		}
		else if (playButton3.contains(Point(x, y)))
		{
			//play song snippet 3
			PlaySnippet(3);
		}
		else if (playButton4.contains(Point(x, y)))
		{
			//play song snippet 4
			PlaySnippet(4);
		}

	}

}
//Function that is called when clicking on the back button in the credits screen.
void callBackFuncCredits(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnBack.contains(Point(x, y)))
		{
			//when clicking on the back on the credits screen, the boolean is set to false.
			isInCreditsOrTheory = false;
			//retting lvl to menu state
			setLvl(0);
			//setting boolean to be in menu
			isInMenu = true;
			//run the menu level
			runLevel(getLvl());
		}
	}
}
//When the next button is clicked in the tutorial, the function below is run.
void callBackFuncTutorial(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnNext.contains(Point(x, y)))
		{
			//Based on the tutorial int, it determines which image to show. If the tutorial int if under or equal to 11, it will load the next slide and add 1 to the int.
			if (getTut() <= 11) {
				//add 1 to the int
				setTut(getTut() + 1);
				//load next tutorial slide.
				loadTutorial(getTut());
			}

			else {
				//if the tutorial int is above 11, the tutorial will exit and go into the level.
				setTut(6); //6 for skipping theory slides/1 for including theory slides
				//loads the level
				loadLevel(1);
				//setting tutorial to false
				isInCreditsOrTheory = false;
				//makes the game wait for 1 second
				waitKey(1000);
				//runs the song
				RepeatSong();
				//set the level to run
				levelRunning = true;
				//running the level thread with the different regions of interest.
				runLevelThreads();
			}
		}
	}
}

//main menu function that display visuals and creates buttons
void mainMenu() {
	//load the background image for main menu
	img = imread("images/bg_menu.jpg", CV_LOAD_IMAGE_COLOR);
	//making the buttons
	btnTutorial = Rect(59, 240, 215, 60);
	btnCredits = Rect(59, 348, 215, 60);
	btnExit = Rect(59, 462, 215, 60);
	btnLevel1 = Rect(752, 240, 215, 60);
	btnLevel2 = Rect(752, 349, 215, 60);
	btnLevel3 = Rect(752, 462, 215, 60);
	//create window and making sure that it can be resized
	namedWindow(winName, CV_WINDOW_NORMAL);
	//setting the window to be fullscreen.
	setWindowProperty(winName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	//setting the window to respond to mouse clicks.
	setMouseCallback(winName, callBackFuncMenu);
	//show window
	imshow(winName, img);
	//setting the menu boolean to be true.
	isInMenu = true;

}
//This function is the callback for when you press any of the rectangles in the menu.
void callBackFuncMenu(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnExit.contains(Point(x, y)))
		{
			//setting booleans to false and closes down all the windows.
			isInMenu = false;
			isInCreditsOrTheory = false;
			destroyAllWindows();
		}
		else {
			if (btnTutorial.contains(Point(x, y)))
			{
				//set tutorial
				setLvl(1);
			}
			else if (btnCredits.contains(Point(x, y)))
			{
				//set credits screen
				setLvl(5);
			}
			else if (btnLevel1.contains(Point(x, y)))
			{
				//set level 1
				setLvl(2);
			}
			else if (btnLevel2.contains(Point(x, y)))
			{
				//set level 2
				setLvl(3);
			}
			else if (btnLevel3.contains(Point(x, y)))
			{
				//set level 3
				setLvl(4);
			}
			if (getLvl() != 0) {
				//run the level, based on the set value above
				runLevel(getLvl());
			}
		}
	}

}
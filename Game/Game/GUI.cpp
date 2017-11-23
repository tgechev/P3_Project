#include "GUI.h"
//#include "Cards.h"
#include "Levels.h"
#include "Sounds.h"
#include "Cards.h"

//Global-ish
Rect btnExit;

//mainMenu
Rect btnTutorial;
Rect btnCredits;
Rect btnLevel1;
Rect btnLevel2;
Rect btnLevel3;

//Tutorial
Rect btnNext;

//level
Rect btnRepeat;
Rect btnConfirm;
Rect btnBack;
//Levels
Rect playButton1;
Rect playButton2;
Rect playButton3;
Rect playButton4;

//timeline blocks
/*
Rect timeline1;
Rect timeline2;
Rect timeline3;
Rect timeline4;
bool timeline;
*/

//vectors
vector<int> cardResults = { 0,0,0,0 };
vector<int> cardResultsPositive = { 1,1,1,1 };
vector< vector<string> > chordsVec;
vector< vector<string> > songVec;
vector<string> lvlVec;
//Window name
string winName = "EXCITING GAME";

Mat img;
Mat testTemp;
Mat correct = imread("images/Correct.png", IMREAD_COLOR);
Mat wrong = imread("images/Wrong.png", IMREAD_COLOR);
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
	
	chordsVec = {LvlT, Lvl1, Lvl2, Lvl3};
	songVec = { songT, song1, song2, song3 };
}

void setTextSongInfo(int lvl, Mat image) {
	getTextData();
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.7;
	int thickness = 2;
	int baseline = 0;
	string artist = songVec.at(lvl-1).at(0);
	string song = songVec.at(lvl-1).at(1);
	//Size textSize = getTextSize(artist, fontFace, fontScale, thickness, &baseline);
	baseline += thickness;
	Point textSong(50, 50);
	Point textArtist(50, 70);
	putText(image, song, textSong, fontFace, fontScale+0.3, Scalar::all(255), thickness, 8);
	putText(image, artist, textArtist, fontFace, fontScale, Scalar::all(255), thickness, 8);

}

void setTextChords(int lvl, Mat image) {
	getTextData();
	string chord = "";
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 1.2;
	int thickness = 2;
	int baseline = 0;
	Size textSize = getTextSize(chord, fontFace, fontScale, thickness, &baseline);
	baseline += thickness;
	
	for (size_t i = 0; i < chordsVec.at(lvl-1).size(); i++) {
		chord = chordsVec.at(lvl-1).at(i);
		Point textOrg(((120*(i+1))-textSize.width/2), 650);
		putText(image, chord, textOrg, fontFace, fontScale,	Scalar::all(255), thickness, 8);	
	}
	
}

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

	Mat tutorial;

	tutorial = imread("images/Tutorial/Tutorial" + to_string(ntut) +".jpg", CV_LOAD_IMAGE_COLOR);


	//making the buttons
	btnNext = Rect(731, 615, 220, 78);

	namedWindow(winName);
	setMouseCallback(winName, callBackFuncTutorial);

	imshow(winName, tutorial);

	isInMenu = false;
	//waitKey(0);
}

void loadLevel(int lvl) {

	Mat temp;

	if ((lvl == 1) || (lvl == 2)) {
		img = imread("images/bg3chords.jpg", CV_LOAD_IMAGE_COLOR);
	}
	else {
		img = imread("images/bg.jpg", CV_LOAD_IMAGE_COLOR);
	}
	temp = img.clone();

	//making the buttons
	btnRepeat = Rect(190, 457, 114, 62);
	btnConfirm = Rect(454, 457, 114, 62);
	btnBack = Rect(718, 457, 114, 62);
	playButton1 = Rect(168, 144, 70, 48);
	playButton2 = Rect(374, 144, 70, 48);
	playButton3 = Rect(577, 144, 70, 48);
	playButton4 = Rect(785, 144, 70, 48);

	namedWindow(winName);
	setMouseCallback(winName, callBackFunc);
	setTextChords(lvl, temp);
	setTextSongInfo(lvl, temp);
	setLvlText(lvl, temp);
	testTemp = temp.clone();
	imshow(winName, temp);

	isInMenu = false;
}

void loadCredits() {
	
	img = imread("images/bg_credits.jpg", CV_LOAD_IMAGE_COLOR);

	//making the buttons
	btnBack = Rect(751, 649, 215, 60);
	
	namedWindow(winName);
	setMouseCallback(winName, callBackFuncCredits);

	imshow(winName, img);

	isInMenu = false;
}


void Confirm(vector<int> vectInput, int level, Mat orgImg) {
	Mat temp;
	temp = orgImg.clone();
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
		cout << "The results are correct" << endl;
		CorrectCards();
	}
	else {
		cout << "The results are NOT correct" << endl;
		InCorrectCards();
	}
	for (size_t i = 0; i<cardResults.size(); ++i) {
		cout << "The results are: " << cardResults[i] << endl;
		if (cardResults[i] == 1) {
			correct.copyTo(temp(Rect(147 + (226 * i), 225, correct.cols, correct.rows)));
		}
		else {
			wrong.copyTo(temp(Rect(147 + (226 * i), 225, correct.cols, correct.rows)));
		}
	}

	imshow(winName, temp);
	//cout << "Checked and confirmed!" << endl;
	waitKey(2000);
	imshow(winName, orgImg);

}

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnRepeat.contains(Point(x, y)))
		{
			//timelineLevel();
			RepeatSong();
			cout << "Repeat!" << endl;
		}
		else if (btnConfirm.contains(Point(x, y)))
		{
			Confirm(getCurCards(), getLvl(), testTemp);

		}
		else if (btnBack.contains(Point(x, y)))
		{
			cout << "Back!" << endl;
			setLvl(0);

			setTut(1);
			levelRunning = false;
			runLevel(getLvl());
		}
		else if (playButton1.contains(Point(x, y)))
		{
			cout << "Play 1" << endl;
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
			isInMenu = true;

			runLevel(getLvl());
		}
	}
}

void callBackFuncTutorial(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnNext.contains(Point(x, y)))
		{
			cout << "Next!" << endl;
			if (getTut() <= 11) {
				setTut(getTut() + 1);
				loadTutorial(getTut());
			}

			else {
				setTut(0);
				cout << getTut() << endl;
				//setLvl(2);
				
				loadLevel(1);

				levelRunning = true;
				runCardThreads();
				runLevelButtonThreads();
			}
		}
	}
}


void mainMenu() {

	img = imread("images/bg_menu.jpg", CV_LOAD_IMAGE_COLOR);
	//making the buttons
	btnTutorial = Rect(59, 240, 215, 60);
	btnCredits = Rect(59, 348, 215, 60);
	btnExit = Rect(59, 462, 215, 60);
	btnLevel1 = Rect(752, 240, 215, 60);
	btnLevel2 = Rect(752, 349, 215, 60);
	btnLevel3 = Rect(752, 462, 215, 60);
	
	namedWindow(winName, CV_WINDOW_NORMAL);
	setWindowProperty(winName, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	setMouseCallback(winName, callBackFuncMenu);

	imshow(winName, img);

	isInMenu = true;

}
void callBackFuncMenu(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (btnExit.contains(Point(x, y)))
		{
			cout << "Exit12" << endl;
			isInMenu = false;


			destroyAllWindows();
		}
		else {
			if (btnTutorial.contains(Point(x, y)))
			{
				cout << "Tutorial" << endl;
				setLvl(1);
			}
			else if (btnCredits.contains(Point(x, y)))
			{
				cout << "Credits" << endl;
				setLvl(5);
			}
			else if (btnLevel1.contains(Point(x, y)))
			{
				cout << "Level 1" << endl;
				setLvl(2);
			}
			else if (btnLevel2.contains(Point(x, y)))
			{
				cout << "Level 2" << endl;
				setLvl(3);
			}
			else if (btnLevel3.contains(Point(x, y)))
			{
				cout << "Level 3" << endl;
				setLvl(4);
			}
			if (getLvl() != 0) {
				runLevel(getLvl());
			}
		}
	}

}



/*void timelineLevel() {
Mat test1;
test1 = img.clone();
//making the timeline rectangles

timeline1 = Rect(106, 113, 203, 31);
timeline2 = Rect(309, 113, 203, 31);
timeline3 = Rect(512, 113, 203, 31);
timeline4 = Rect(715, 113, 202, 31);

//int i = 0;
int snippetcount = 0;


while (snippetcount <= 4) {
cout << "In WHILE" << endl;
//cout << music.getPlayingOffset().asSeconds() <<endl;


//cout << i;
test1(timeline1) = Vec3b(250, 0, 200);
imshow(winName, test1);

//teds if
//if(music.getPlayingOffset().asSeconds()==0)
//end teds if
//if (waitKey(5) >= 0)
//	break;


//if (i == 0) {
//img(timeline1) = Vec3b(250, 0, 200);
//rectangle(img, timeline1, Scalar(250, 0, 200), -1);
//}

if (snippetcount == 0 && snippetp2.getStatus() == snippetp2.Stopped && snippetp3.getStatus() == snippetp3.Stopped) {
cout << "snip1" << endl;
test1(timeline2) = Vec3b(250, 0, 200);
RepeatSong1();
}

if (snippetcount == 1 && snippetp1.getStatus() == snippetp1.Stopped && snippetp3.getStatus() == snippetp3.Stopped) {
cout << "snip2" << endl;
test1(timeline3) = Vec3b(250, 0, 200);
RepeatSong2();

}
//else if (i == 10) {
//img(timeline4) = Vec3b(250, 0, 200);
//}
//snippetp3.getStatus() == snippetp3.Playing

if (snippetcount == 2 && snippetp1.getStatus() == snippetp1.Stopped && snippetp2.getStatus() == snippetp2.Stopped) {
cout << "snip3" << endl;
test1(timeline1) = Vec3b(200, 200, 200);
test1(timeline2) = Vec3b(200, 200, 200);
test1(timeline3) = Vec3b(200, 200, 200);
RepeatSong3();

}
cout << "Before sleep!" << endl;
this_thread::sleep_for(1s);
//i = i + 1;


cout << "Between" << endl;
snippetcount++;
}
cout << "After sleep!" << endl;

}*/
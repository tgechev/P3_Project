#include "Levels.h"
#include "Cards.h"
#include "GUI.h"
#include "Sounds.h"

vector<int> getCorrectCards(int lvl) {
	vector<int> correctCards;

	switch (lvl)
	{
	case 1:
		correctCards = { 1, 5, 4, 0 };
		break;
	case 2:
		correctCards = { 1, 7, 4, 0 };
		break;
	case 3: 
		correctCards = { 1, 5, 4, 3 };
		break;
	case 4: 
		correctCards = { 1, 7, 6, 5 };
		break;
	default:
		break;
	}
	cout << "current level is: " << lvl << endl;
	return correctCards;
}

int level = 0;
int tutlevel = 6; //6 for skipping theory slides/1 for including theory slides

//LEVEL STUFF
int getLvl() {
	return level;
}

void setLvl(int lvl) {
	level = lvl;
}

//TUTORIAL STUFF
int getTut() {
	return tutlevel;
}

void setTut(int tut) {

	tutlevel = tut;
}

void runLevel(int lvl)
{
	switch (lvl) {
	case 0:
		mainMenu();

		runMenuThreads();
		break;
	case 1:
		cout << "Tutorial level!" << endl;
		loadTutorial(getTut());
		isInCreditsOrTheory = true;
		runCreditsOrTheoryThread(false);
		break;
	case 2:
		cout << "level 1" << endl;
		//isInMenu = false;
		loadLevel(lvl);

		levelRunning = true;

		waitKey(1000);
		RepeatSong();
		runLevelThreads();
		break;
	case 3:
		cout << "Level 2" << endl;
		loadLevel(lvl);

		waitKey(1000);
		RepeatSong();
		levelRunning = true;
		runLevelThreads();
		break;
	case 4:
		cout << "Level 3" << endl;
		loadLevel(lvl);

		waitKey(1000);
		RepeatSong();
		levelRunning = true;
		runLevelThreads();
		break;
	case 5:
		cout << "Credits" << endl;
		loadCredits();

		isInCreditsOrTheory = true;
		runCreditsOrTheoryThread(true);
		break;
	default:
		cout << "Invalid level" << endl;
	}

}







	





			
			
		
			
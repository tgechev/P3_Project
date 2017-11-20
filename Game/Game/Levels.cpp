#include "Levels.h"

#include "GUI.h"

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

void checkLevel(int lvl)
{
	switch (lvl) {
	case 0:
		mainMenu();
		break;
	case 1:
		cout << "Tutorial level!" << endl;
		loadLevel();
		break;
	case 2:
		cout << "level 1" << endl;
		loadLevel();
		break;
	case 3:
		cout << "Level 2" << endl;
		loadLevel();
		break;
	case 4:
		cout << "Level 3" << endl;
		loadLevel();
		break;
	case 5:
		cout << "Credits" << endl;
		loadCredits();
		break;
	default:
		cout << "Invalid level" << endl;
	}

}






	





			
			
		
			
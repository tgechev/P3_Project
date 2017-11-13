#include "Levels.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
#include "Main.h"
#include <SFML/Audio.hpp>
#include<SFML/System.hpp>

using namespace cv;
using namespace std;

sf::Music music;


void RepeatSong() {
	
	music.openFromFile("Audio/Jack/Song.ogg");
	cout << "repeat" << endl;
	music.play();
}

void PlayChord(int chord) {
	
	switch (chord)
	{
	case 1:
		music.openFromFile("Audio/Jack/1.ogg");
		cout << "1" << endl;
		music.play();
		break;
	case 2:
		music.openFromFile("Audio/Jack/2.ogg");
		cout << "2" << endl;
		music.play();
		break;
	case 3:
		music.openFromFile("Audio/Jack/3.ogg");
		cout << "3" << endl;
		music.play();
		break;
	case 4:
		music.openFromFile("Audio/Jack/4.ogg");
		cout << "4" << endl;
		music.play();
		break;
	case 5:
		music.openFromFile("Audio/Jack/5.ogg");
		cout << "5" << endl;
		music.play();
		break;
	case 6:
		music.openFromFile("Audio/Jack/6.ogg");
		cout << "6" << endl;
		music.play();
		break;
	case 7:
		music.openFromFile("Audio/Jack/7.ogg");
		cout << "7" << endl;
		music.play();
		break;
	default:
		cout << "error" << endl;
		break;
	}

}


void PlaySnippet(int snipNum) {
	
	switch (snipNum)
	{
	case 1:

		music.openFromFile("Audio/Jack/SnippetsJack1.ogg");
		cout << "snip1" << endl;
		//music.play();
		break;
	case 2:
		music.openFromFile("Audio/Jack/SnippetsJack2.ogg");
		cout << "lol 2" << endl;
		//music.play();
		break;
	case 3:
		music.openFromFile("Audio/Jack/SnippetsJack3.ogg");
		cout << "lol 3" << endl;
		//music.play();
		break;
	case 4:
		music.openFromFile("Audio/Jack/SnippetsJack4.ogg");
		cout << "lol 4" << endl;
		//music.play();
		break;
	default:
		cout << "error" << endl;
		break;
	}
	music.play();
	
}
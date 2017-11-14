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
	
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Song.ogg");
	music.play();
}

void PlayChord(int chord) {

	music.openFromFile("Audio/" + to_string(getLvl()) + "/" + to_string(chord) + ".ogg");
	music.play();

}


void PlaySnippet(int snipNum) {
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Snip" + to_string(snipNum) + ".ogg");
	music.play();
	
}
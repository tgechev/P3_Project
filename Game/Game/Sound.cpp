#include "Levels.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
//#include <MMSystem.h>
//#include <mciapi.h>
#include "Main.h"
#include <SFML/Audio.hpp>
#include<SFML/System.hpp>

using namespace cv;
using namespace std;

sf::Music music;


void RepeatSong() {
	/*
	//MP3Player player;

	player.OpenFromFile("Audio/HitTheRoadJack/JackSong.mp3");

	player.Play();
	*/
}

void PlayChord(int chord) {
	/*
	//MP3Player player;
	
	//const char test[] = { "Audio/HitTheRoadJack/1.mp3" };
	//player.OpenFromFile(test);
	//player.OpenFromFile("Audio/HitTheRoadJack/1.mp3");
	//player.Play();

	switch (chord)
	{
	case 1:
		player.OpenFromFile("Audio/HitTheRoadJack/1.mp3");
		break;
	case 2:
		player.OpenFromFile("Audio/HitTheRoadJack/2.mp3");
		break;
	case 3:
		player.OpenFromFile("Audio/HitTheRoadJack/3.mp3");
		break;
	case 4:
		player.OpenFromFile("Audio/HitTheRoadJack/4.mp3");
		break;
	case 5:
		player.OpenFromFile("Audio/HitTheRoadJack/5.mp3");
		break;
	case 6:
		player.OpenFromFile("Audio/HitTheRoadJack/6.mp3");
		break;
	case 7:
		player.OpenFromFile("Audio/HitTheRoadJack/7.mp3");
		break;
	default:
		player.OpenFromFile("Audio/error.mp3");
		break;
	}

	player.Play();
	*/
}


void PlaySnippet(int snipNum) {
	
	switch (snipNum)
	{
	case 1:

		music.openFromFile("Audio/Jack/SnippetsJack1.ogg");
		cout << "snip1" << endl;
		music.play();
		break;
	case 2:
		music.openFromFile("Audio/Jack/SnippetsJack2.ogg");
		cout << "lol 2" << endl;
		music.play();
		break;
	case 3:
		music.openFromFile("Audio/Jack/SnippetsJack3.ogg");
		cout << "lol 3" << endl;
		music.play();
		break;
	case 4:
		music.openFromFile("Audio/Jack/SnippetsJack4.ogg");
		cout << "lol 4" << endl;
		music.play();
		break;
	default:
		cout << "error" << endl;
		break;
	}
	
}
#ifndef SOUNDS_H
#define SOUNDS_H

#include <iostream> 
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

//using namespace cv;
using namespace std;

//sf::Music music;
/*
extern sf::Music snippetp1;
extern sf::Music snippetp2;
extern sf::Music snippetp3;
extern sf::Music snippetp4;

void RepeatSong1();
void RepeatSong2();
void RepeatSong3();
void RepeatSong4();*/
void RepeatSong();
void PlayChord(int chord);
void PlaySnippet(int snipNum);
void CorrectCards();
void InCorrectCards();

#endif

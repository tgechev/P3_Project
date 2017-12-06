#ifndef SOUNDS_H
#define SOUNDS_H

#include <iostream> 
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

//using namespace cv;
using namespace std;

void RepeatSong();
void PlayChord(int chord);
void PlaySnippet(int snipNum);
void CorrectCards();
void InCorrectCards();
void playAllChords();

#endif

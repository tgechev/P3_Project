#ifndef SOUNDS_H
#define SOUNDS_H

#include <iostream> 
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>

//set namespace, so "std::" doesn't need to be written all the time.
using namespace std;
//Declare the functions to be used later in other files.
void RepeatSong();
void PlayChord(int chord);
void PlaySnippet(int snipNum);
void CorrectCards();
void InCorrectCards();
void playAllChords();

#endif

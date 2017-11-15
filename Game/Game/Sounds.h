#ifndef Sounds
#define Sounds

#include <opencv2\opencv.hpp> 
#include <iostream> 
#include <windows.h>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "Sounds.h"
using namespace cv;
using namespace std;

extern sf::Music music;
extern sf::Music snippetp1;
extern sf::Music snippetp2;
extern sf::Music snippetp3;
extern sf::Music snippetp4;

void RepeatSong1();
void RepeatSong2();
void RepeatSong3();
void RepeatSong4();
void PlayChord(int chord);
void PlaySnippet(int snipNum);
void CorrectCards();
void InCorrectCards();

#endif

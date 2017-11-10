#include <opencv2\opencv.hpp>
#include <iostream>
#include <windows.h>
#include <MMSystem.h>
#include "MP3Player.h"
#include "Levels.h"

using namespace cv;
using namespace std;

//Global
int level = 0;


int getLvl() {
	return level;
}

void setLvl(int lvl) {

	level = lvl;
}
void RepeatSong() {
	MP3Player player;

	player.OpenFromFile("Audio/test.mp3");

	player.Play();

	//while (true) {
		//printf("Test music for 20s : %f elapsed\n",player.GetPosition());
		//Sleep(1000);
	//}
	//player.Close();

}


int main()
{
	checkLevel(getLvl());
	return 0;
}
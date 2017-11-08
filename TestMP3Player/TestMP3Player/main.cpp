#include "MP3Player.h"

int main(int argc, TCHAR* argv[])
{
	MP3Player player;

	player.OpenFromFile(L"your path to an mp3 file here");

	player.Play();

	while (true) {
		//printf("Test music for 20s : %f elapsed\n",player.GetPosition());
		Sleep(1000);
	}
	//player.Close();

	return 0;
}


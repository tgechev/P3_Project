#include "MP3Player.h"

int main(int argc, TCHAR* argv[])
{
	MP3Player player;

	player.OpenFromFile(L"test4.mp3");

	player.Play();

	while (true) {
		//printf("Test music for 20s : %f elapsed\n",player.GetPosition());
		Sleep(1000);
	}
	//player.Close();

	return 0;
}


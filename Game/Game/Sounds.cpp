//include the other files from the game, that has to work with the functions below
#include "BLOB.h"
#include "Levels.h"
#include "Sounds.h"

//include the class 'Music' from the SFML library
sf::Music music;

//function to repeat the song when pressing the repeat button in a level
void RepeatSong() {
	//load the right song-file based on the current level. The song-files are conviniently named "Audio/1/Song.ogg". The song files are of .OGG format, as it is a open source format.
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Song.ogg");
	//play the file loaded into music.
	music.play();
	//wait the milliseconds that the song is consisting of.
	waitKey(music.getDuration().asMilliseconds());
}
//the function "Playchord" plays a chord based on the current level and integer value.
void PlayChord(int chord) {
	//load the chord sound file. 
	music.openFromFile("Audio/" + to_string(getLvl()) + "/" + to_string(chord) + ".ogg");
	//Play the file
	music.play();
}

//this function is played when entering a level. It will play all the designated chord that belong to the current level.
void playAllChords() {
	//the for-loop count from 1-7.
	for (int i = 1; i <= 7; i++) {
		//load the sound-file based on the current level and the current integer in the for-loop.
		music.openFromFile("Audio/" + to_string(getLvl()) + "/" + to_string(i) + ".ogg");
		//play the sound file
		music.play();
		//waits 1.5 seconds before continuing the code.
		waitKey(1500);
	}
}
//plays a snippet based given integer
void PlaySnippet(int snipNum) {
	//loads snippet based on current level and given integeræ
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Snip" + to_string(snipNum) + ".ogg");
	//Plays the sound file
	music.play();
}
//Plays the victory sound when having all the cards correct.
void CorrectCards() {
	music.openFromFile("Audio/correctcards.ogg");
	music.play();
}
//Plays the "Try again" sound when the confirm button is pressed and the cards are incorrect.
void InCorrectCards() {
	music.openFromFile("Audio/incorrectcards.ogg");
	music.play();
}
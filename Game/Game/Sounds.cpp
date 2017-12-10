#include "BLOB.h"
#include "Levels.h"
#include "Sounds.h"



sf::Music music;

void RepeatSong() {
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Song.ogg");
	music.play();
	waitKey(music.getDuration().asMilliseconds());
}

void PlayChord(int chord) {
	music.openFromFile("Audio/" + to_string(getLvl()) + "/" + to_string(chord) + ".ogg");
	music.play();
}

void playAllChords() {
	for (int i = 1; i <= 7; i++) {
		music.openFromFile("Audio/" + to_string(getLvl()) + "/" + to_string(i) + ".ogg");
		music.play();
		waitKey(1500);
	}
}

void PlaySnippet(int snipNum) {
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Snip" + to_string(snipNum) + ".ogg");
	music.play();
}

void CorrectCards() {
	music.openFromFile("Audio/correctcards.ogg");
	music.play();
}

void InCorrectCards() {
	music.openFromFile("Audio/incorrectcards.ogg");
	music.play();
}
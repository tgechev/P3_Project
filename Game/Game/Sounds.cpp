#include "Levels.h"
#include "Sounds.h"


sf::Music music;

/* sf::Music snippetp1;
sf::Music snippetp2;
sf::Music snippetp3;
sf::Music snippetp4;

void RepeatSong1() {

	snippetp1.openFromFile("Audio/" + to_string(getLvl()) + "/Snip1.ogg");
	snippetp1.play();
}

void RepeatSong2() {
	snippetp2.openFromFile("Audio/" + to_string(getLvl()) + "/Snip2.ogg");
	snippetp2.play();
}

void RepeatSong3() {
	snippetp3.openFromFile("Audio/" + to_string(getLvl()) + "/Snip3.ogg");
	snippetp3.play();
}

void RepeatSong4() {
	snippetp4.openFromFile("Audio/" + to_string(getLvl()) + "/Snip4.ogg");
	snippetp4.play();
}*/

void RepeatSong() {
	music.openFromFile("Audio/" + to_string(getLvl()) + "/Song.ogg");
	music.play();
}

void PlayChord(int chord) {
	music.openFromFile("Audio/" + to_string(getLvl()) + "/" + to_string(chord) + ".ogg");
	music.play();
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
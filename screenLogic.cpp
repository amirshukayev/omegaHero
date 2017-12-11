
/**
 //    ____   _    _
 //   / __ \ | |  | |
 //  | |  | || |__| | ___ _ __ ___
 //  | |  | ||  __  |/ _ \ '__/ _ \
 //  _\ \/ /_| |  | |  __/ | | (_) |
 // (___||___)_|  |_|\___|_|  \___/
*/

//Created by Navras Kamal and Amir Shukayev for CMPUT 274 Final Project, December 2017
//Implements some code used in lectures and by TA's for the course

/*                     HOW THIS IS GONNA WORK
0. Menu: Base splash screen, main menu with difficulty selector, song selector and
				 high scores.  Difficulty can be toggled between two states (easy / hard),
				 there is currently 7 songs in the list, which can be sorted based on
				 multiple parameters, and highscores are recorded after each games and
				 are displayed on a sorted list in the highscores tab

1. Scanner: Every second scans array of notes. if there is a match, note is added
	 				  to the screen.

2. Advancement: Every loop the pixels are advanced once. Array holds all notes
								and goes through them and advanced them

3. Playing: If the button is pressed, the clicking segment is checked for
						available note. If it is then the user is given one point

4. RANDOM SONGS:  generates random music in C-minor (Amir's favourite key)

*/

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include "menus.h"
#include "sorting.h"

// pin constants
const int TFT_DC = 9;
const int TFT_CS = 10;

// screen constants
const int ScreenWidth = 320;
const int ScreenHeight = 240;

// currently unused
const int BUTTON_1 = 11;
const int BUTTON_2 = 12;
const int BUTTON_3 = 13;

// for square sin wave frequency outputs.
// sounds like more harsh than normal tones
const int SPEAKER_PIN = 47;

// macros because A2 instead of just an int
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin

// width/height of the display when rotated horizontally
#define TFT_WIDTH  320
#define TFT_HEIGHT 240

// 10 works well in terms of lag. shouldnt be more than 6 with current delays
const int MAX_RENDERED_NOTES = 10;

// Number of vertical Lines
int lines;
// game counter

// struct for a note moving across the screen
struct Note {
	int progression;
	uint8_t num;
};

// temporary song:
int song_1[10];
int song_2[] = {2, 6, 8, 12, 16, 19, 20, 22, 28, 30};
int song_3[] = {0, 4, 13, 14, 18, 21, 24, 26, 27, 29};

// these arrays holds all the notes on the screen to be rendered
// max of 30 notes to be rendered
Note screen_notes1[MAX_RENDERED_NOTES];
Note screen_notes2[MAX_RENDERED_NOTES];
Note screen_notes3[MAX_RENDERED_NOTES];

// goes through the first song for displaying note
int song_counter_1 = 0;
int song_counter_2 = 0;
int song_counter_3 = 0;

// these make sure that 1 tap doesnt give/take away 10+ points at once
boolean cooldown1 = false;
boolean cooldown2 = false;
boolean cooldown3 = false;

// creating the display screen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// creating the touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// gives us middle octave frequencies
const int note_c = 262;
const int note_d = 294;
const int note_e = 330;
const int note_f = 349;
const int note_g = 392;
const int note_a = 440;
const int note_b = 494;
const int node_C = 523;

// these two arrays hold the frequencies of notes.
// follows 440Hz A standard.
// C minor is 3 octaves for now
const int C_MAJ[] = {262,294,330,349,392,440,494,523};
const int C_MIN[] = {147, 156, 175, 196, 208, 233, 262 /*c*/, 130 /*C*/,
	 									147, 156, 175, 196, 208, 233, 262 /*c*/,
										294, 311, 349, 392, 415, 466, 523 /*C*/ };

// holds patterns for left hand (generally)
const int C_MIN_PATTERN_1[] = { C_MIN[7], C_MIN[11], C_MIN[9], C_MIN[11] };
const int C_MIN_PATTERN_2[] = { C_MIN[7], C_MIN[12], C_MIN[9], C_MIN[12] };
const int C_MIN_PATTERN_3[] = { C_MIN[7], C_MIN[8],  C_MIN[9], C_MIN[11] };
const int C_MIN_PATTERN_4[] = { C_MIN[11], C_MIN[8], C_MIN[10], C_MIN[9] };

// holds lower scale patterns
const int C_MIN_PATTERN_5[] = { C_MIN[0], C_MIN[1], C_MIN[2], C_MIN[3] };
const int C_MIN_PATTERN_6[] = { C_MIN[4], C_MIN[3], C_MIN[2], C_MIN[1] };
const int C_MIN_PATTERN_7[] = { C_MIN[6], C_MIN[5], C_MIN[4], C_MIN[3] };
const int C_MIN_PATTERN_8[] = { C_MIN[3], C_MIN[4], C_MIN[5], C_MIN[6] };

// holds higher scale patterns
const int C_MIN_PATTERN_9[] = { C_MIN[14], C_MIN[15], C_MIN[16], C_MIN[17] };
const int C_MIN_PATTERN_10[] = { C_MIN[15], C_MIN[14], C_MIN[13], C_MIN[12] };
const int C_MIN_PATTERN_11[] = { C_MIN[16], C_MIN[17], C_MIN[18], C_MIN[19] };
const int C_MIN_PATTERN_12[] = { C_MIN[18], C_MIN[17], C_MIN[18], C_MIN[16] };

// holds repeated note patterns
const int C_MIN_PATTERN_13[] = { C_MIN[18], C_MIN[18], C_MIN[17], C_MIN[18] };
const int C_MIN_PATTERN_14[] = { C_MIN[17], C_MIN[17], C_MIN[16], C_MIN[17] };
const int C_MIN_PATTERN_15[] = { C_MIN[16], C_MIN[16], C_MIN[19], C_MIN[16] };
const int C_MIN_PATTERN_16[] = { C_MIN[14], C_MIN[18], C_MIN[14], C_MIN[14] };

// for the random number generator.
const int RANDOM_PIN = 33;

// this will count frames from 1-60
int counter1 = 0;
// this will count seconds for the songs;
int counter2 = 0;
// player points
int points = 0;

// for continuing after the song finishes
bool infinteMode = true;

// hard mode subtracts points for lives lost
// not that big of a difficulty increase
bool hardMode = false;

// holds generated music
// size of 20 tones is a good middle ground between using a lot of memory, and
// not making too many calls to the random music generator
int music[20];
// holds the length of the song
int length_music = 20;

// for progressing the music tones
int toneCounter = 0;
int song_rand[4];

// counts remaining lives. Lives lost during a mispress.
int lives = 3;

int songToPlay = 0;

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000

int numsongs = 7; //must be hardcoded, currently 7
song songs[7]; //size of this must also be hardcoded and should be equal to numsongs

//global vars for song listings
int pg = 1; //current page of songs
int maxpages = ceil(numsongs/3); //number of full pages of three songs that can be made from the listed number of songs

int displayedSongs; //number of songs on screen (1-3)

int sortMode = 0; //0: name, 1: length, 2: difficulty, 3: artist

// function declarations
void drawNote(Note n);
void drawNote(int progression, int n);
Note addNote(int num);
void advance(Note &n);
void advanceAllRenderedNotes();
void addNotesFromSong();
int randomNumber(int bits);
void returnMusic(int len);
void restart();
void addLine(int num);
point checkTouch();
int lengthString(String s);
void printString(String s);
void drawLives();


void setup_screen_notes(){

	int screen_counter1 = 0; int screen_counter2 = 0; int screen_counter3 = 0;

	int additionalCounter = 0;

	for (int i = 0; i < 27; i++){
		int a = randomNumber(4);

		if (a % 3 == 0 && screen_counter1 != 10){
			song_1[screen_counter1] = additionalCounter;
			screen_counter1++;
			additionalCounter++;
		}
		else if (a % 3 == 1 && screen_counter2 != 10){
			song_2[screen_counter2] = additionalCounter;
			screen_counter2++;
			additionalCounter++;
		}
		else if (a % 3 == 2 && screen_counter3 != 10){
			song_3[screen_counter3] = additionalCounter;
			screen_counter3++;
			additionalCounter++;
		}

	}

	for (int i = 0; i < 10; i++){
		Serial.print("yeah: ");
		Serial.println(song_1[i]);
	}


}



// initializes program
void setup(){
	init();
	Serial.begin(9600);
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);

	// setting up structs for songs.  To modify / add / remove songs change these values.  Note that you must update the above numsongs and size of songs[]
	String title[] = {"Mushroom Kingdom Overworld", "Dream Land", "Hyrule Overworld", "Type A (Korobeiniki)", "Green Hills Zone", "Dr. Wily Stage Theme", "Mortal Kombat!"};
	String game[] = {"Super Mario Bros.", "Kirby's Dream Land", "The Legend of Zelda", "Tetris", "Sonic the Hedgehog", "Mega Man II", "Mortal Kombat"};
	int length[] = {70, 33, 64, 65, 79, 31, 81}; //in seconds
	int difficulty[] = {2, 1, 1, 2, 1, 3, 3}; //no current effect on game play
	String artist[] = {"Koji Kondo", "Jun Ishikawa", "Koji Kondo", "Hirakazu Tanaka", "Masato Nakamura", "Takashi Tateishi", "The Immortals"};
	//Names of songs, games and artists are not our property and belong to their original owners.  They are being used here for demonstation and educational purposes and their copyrighted music is not implemented

	for (int i = 0; i < numsongs; i++){
		songs[i] = {title[i], game[i], length[i], difficulty[i], artist[i], 0}; //builds structs
	}

	if(numsongs%3){maxpages++;} //adds an extra page in case number of songs is not a multiple of three

	Serial.println("");
	Serial.println("~presort~");
	for(int i = 0; i < numsongs; i++){
		Serial.println(songs[i].name);
		Serial.print("   L:");
		Serial.print(songs[i].length);
		Serial.print(" DIFF:");
		Serial.print(songs[i].difficulty);
		Serial.print(" GAME:");
		Serial.println(songs[i].game);
	}

	splashScreen();

	tft.setTextColor(ILI9341_WHITE);

	// setting up screen
	//tft.fillScreen(ILI9341_BLACK);

	// setting up point counter at top left
	tft.setCursor(0,0);
	tft.setTextSize(2);
	//tft.print(points);

	// setting up buttons (NOT USED AS OF YET)
	pinMode(BUTTON_1, INPUT_PULLUP);
	pinMode(BUTTON_2, INPUT_PULLUP);
	pinMode(BUTTON_3, INPUT_PULLUP);

	// setting up piezo speaker
	pinMode(SPEAKER_PIN, OUTPUT);

	// generates random music for the game
	returnMusic(length_music);

	/*
	Serial.print("pointer: ");
	int test = (int)&music;
	Serial.println(test);*/

	/*
	Serial.println("these are the musical frequencies of the random song");
	for (int i = 0; i < length_music; i++){
		Serial.println(music[i]);
	}
	*/

	/*
	tft.fillRect(0,0,  ScreenHeight,ScreenWidth,  ILI9341_BLUE);
	tft.fillRect(4,4,  ScreenHeight-8,ScreenWidth-8,   ILI9341_BLACK);
	*/

	// checks if num is -1, that means it shouldn't be rendered.
	for (int i = 0; i < MAX_RENDERED_NOTES; i++){
		screen_notes1[i].progression = 0;
		screen_notes1[i].num = -1;

		screen_notes2[i].progression = 0;
		screen_notes2[i].num = -1;

		screen_notes3[i].progression = 0;
		screen_notes3[i].num = -1;
	}
}

point checkTouch(){
	TSPoint touch = ts.getPoint();

	if ((touch.z < MINPRESSURE) || (touch.z > MAXPRESSURE)) {
		// no touch, just quit
		point val = {-1, -1};
		return val;
	}
	// get the y coordinate of where the display was touched
	// remember the x-coordinate of touch is really our y-coordinate
	// on the display
	int touchX = map(touch.x, TS_MINX, TS_MAXX, 0, 240);
	touchX = constrain(touchX, 0, 240);

	// need to invert the x-axis, so reverse the
	// range of the display coordinates
	int touchY = map(touch.y, TS_MINY, TS_MAXY, 0, 320);
	touchY = constrain(touchY, 0, 320);
	point val = {touchX, touchY};
	return val;
}

void printString(String s){
	int len = lengthString(s);
	if(len > 29){
		s[27] = 46;
		s[28] = 46;
		s[29] = 46;
		for(int i = 30; i <= len; i++){ //adds ellipses and hides remaining characters if name is too long to display as is
			s[i] = 0;
		}
		tft.print(s);
	}else{
		tft.print(s);
	}
}

int lengthString(String s){
	int count = 0;
	while (s[count]){count++;}
	// Serial.println(count);
	return count;
}

// restarting from death screen, moving to play state
void restart(){

	setup_screen_notes();

	tft.fillScreen(ILI9341_BLACK);
	addLine(3);
	tft.setTextSize(2);

	// restarting all the numbers and stuff
	// that make the game run

	points = 0;
	counter1 = 0;
	counter2 = 0;
	song_counter_1 = 0;
	song_counter_2 = 0;
	song_counter_3 = 0;
	lives = 3;

	for (int i = 0; i < MAX_RENDERED_NOTES; i++){
		screen_notes1[i].num = -1;
		screen_notes2[i].num = -1;
		screen_notes3[i].num = -1;
	}

	if (hardMode){
		lives = 1;
	} else {
		lives = 5;
	}


	// goes back to normal state
	drawLives();
	tft.fillRect(0,0, 11,11, ILI9341_BLACK);
	tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	tft.setCursor(0,0);
	tft.setTextSize(2);
	tft.print(points);
	while(true){
		loop();
	}
}

// uses piezo speaker to play a note
void playNote(){

	// 0.6 seconds for normal notes
	tone(SPEAKER_PIN, music[toneCounter], 600);

	// for debugging purposes
	Serial.println(music[toneCounter]);

	// so next tone in the music generator is played
	toneCounter++;

	// resets the music, and gets some more tunes.
	// doing this saves space, and reduces amount of calls to the random generator
	// making the game smoother in general
	if (toneCounter >= length_music){
		// resets
		toneCounter = 0;
		// makes some more music!
		returnMusic(length_music);
	}
}


// Draws the circle at the bottom which are kind of instruments
void drawInstruments(){
	tft.drawCircle((ScreenHeight/4)*1, 300, 5, ILI9341_RED);
	tft.drawCircle((ScreenHeight/4)*1, 300, 6, ILI9341_RED);
	tft.drawCircle((ScreenHeight/4)*1, 300, 7, ILI9341_YELLOW);

	tft.drawCircle((ScreenHeight/4)*2, 300, 5, ILI9341_RED);
	tft.drawCircle((ScreenHeight/4)*2, 300, 6, ILI9341_RED);
	tft.drawCircle((ScreenHeight/4)*2, 300, 7, ILI9341_YELLOW);

	tft.drawCircle((ScreenHeight/4)*3, 300, 5, ILI9341_RED);
	tft.drawCircle((ScreenHeight/4)*3, 300, 6, ILI9341_RED);
	tft.drawCircle((ScreenHeight/4)*3, 300, 7, ILI9341_YELLOW);
}

void drawLives(){
	tft.fillRect(190, 4, 50, 8, ILI9341_BLACK);//refreshes life counter visuals
	tft.setCursor(190, 5);
	tft.setTextSize(1);
	if(lives > 3){
		tft.setTextColor(ILI9341_GREEN);
	}
	if(lives == 2){
		tft.setTextColor(ILI9341_YELLOW);
	}
	if(lives == 1){
		tft.setTextColor(ILI9341_RED);
	}
	tft.print("LIVES: ");
	tft.print(lives);
}

// to proccess touches on the screen
void processTouch(){
	// gets point
	TSPoint touch = ts.getPoint();

	// if they barely press it or if the wind blows on it or something
	if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
		// no touch, just quit
		return;
	}

	// get the y coordinate of where the display was touched
	// remember the x-coordinate of touch is really our y-coordinate
	// on the display
	int touchY = map(touch.x, TS_MINX, TS_MAXX, 0, TFT_HEIGHT - 1);

	// need to invert the x-axis, so reverse the
	// range of the display coordinates
	int touchX = map(touch.y, TS_MINY, TS_MAXY, TFT_WIDTH - 1, 0);

	// if they touch the first intstument
	if (touchX < 100 && touchY < ScreenWidth/4){
		for (int i = 0; i < MAX_RENDERED_NOTES; i++){
			if (screen_notes1[i].progression > 275 && screen_notes1[i].num != -1 && screen_notes1[i].progression < 310){
				if (cooldown1){
					// increase points
					points++;

					// make this instrument cool down a bit
					cooldown1 = false;

					//update score stuff
					tft.fillRect(0,0, 20,20, ILI9341_BLACK);
					tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
					tft.setCursor(0,0);
					tft.setTextSize(2);
					tft.print(points);


					// call tone player
					playNote();

					// dont need to go through other notes
					break;
				}
			}
		}

		// this is if they mess up
		if (cooldown1) {
			if (hardMode){

				// reduce points if in hard mode
				points--;
			}

			// update score on display
		  tft.fillRect(0,0, 11,11, ILI9341_BLACK);
			tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			tft.setCursor(0,0);
			tft.print(points);

			// starts up cooldown
			cooldown1 = false;

			// reduce lives left
			lives--;
			tone(SPEAKER_PIN, 988 , 100);
			drawLives();
		}
	}

	// if they touch the third instrument
	else if (touchX < 100 && touchY > (ScreenWidth * 2)/4){

		// go through all notes being rendered on the screen
		for (int i = 0; i < MAX_RENDERED_NOTES; i++){

			// if the note is in the okay region
			if (screen_notes3[i].progression > 275 && screen_notes3[i].num != -1 && screen_notes3[i].progression < 310){

				// dont act if its cooling down so one press doesnt count as a bunch
				if (cooldown3){

					// increase points
					points++;

					// setup cooldown
					cooldown3 = false;

					// print updated points to top left
					tft.fillRect(0,0, 11,11, ILI9341_BLACK);
					tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
					tft.setCursor(0,0);
					tft.setTextSize(2);
					tft.print(points);

					// play the sound
					playNote();

					// dont need to go through other notes
					break;
				}
			}
		}

		// if the mess up
		if (cooldown3) {

			// subtract points if player is in hard mode
			if (hardMode){
				points--;
			}

			// update top left point counter
			tft.fillRect(0,0, 11,11, ILI9341_BLACK);
			tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			tft.setCursor(0,0);
			tft.setTextSize(2);
			tft.print(points);

			// setup cooldown
			cooldown3 = false;

			// reduce lives
			lives--;
			tone(SPEAKER_PIN, 988 , 100);
			drawLives();
		}
	}

	// this logic is the same as above except for the 2nd (middle instrument)
	else if (touchX < 100){
		for (int i = 0; i < MAX_RENDERED_NOTES; i++){
			if (screen_notes2[i].progression > 275 && screen_notes2[i].num != -1 && screen_notes2[i].progression < 310){
				if (cooldown2){
					points++;
					cooldown2 = false;
					tft.fillRect(0,0, 11,11, ILI9341_BLACK);
					tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
					tft.setCursor(0,0);
					tft.print(points);
					playNote();
					break;
				}
			}
		}

		if (cooldown2) {
			if (hardMode){
				points--;
			}
		  tft.fillRect(0,0, 11,11, ILI9341_BLACK);
			tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			tft.setCursor(0,0);
			tft.print(points);
			cooldown2 = false;

			lives--;
			tone(SPEAKER_PIN, 988 , 100);
			drawLives();
		}
	}

	// this goes to the death screen state if player loses all their lvies
	if (lives == 0){
		onDeath();
	}
}

// this gets reduced over time to increase difficulty marginally
// minimum value of 2
int delayVal = 8;

// game loop
void loop(){
	processTouch();

	// fast algorithm to move notes forward without wasting
	// time redrawing pixels the same colour
	advanceAllRenderedNotes();
	// delay of 17 gives us approximately 60 frames per second.
	delay(delayVal);

	// counter1 counts frames
	counter1++;

	// one logic tic a second (or every 60 frames if delay isnt 17)
	if (counter1 == 60){

		// checks song arrays to see if a note should be added
		addNotesFromSong();

		// counter 2 counts logic frames
		counter2++;

		// RESETS SONG;
		if (infinteMode){
			if (counter2 > 20){

				// resets counter so song replays every 20 logic frames
				counter2 = 0;

				// caps speed at delay of 2.
				if (delayVal > 2){
					delayVal--;
				}

			}
		}

		// resets to count until next logic frame
		counter1 = 0;
	}

	// redraw instruments every 0.25 seconds or 15 frames
	if (counter1 == 15){
		drawInstruments();
	}

	// resets cooldowns every 20 logic frames. This is so game still goes fairly fast
	// and gives enough time for player to react to multiple notes in a row
	if (counter1 == 20){
		cooldown1 = true;
		cooldown2 = true;
		cooldown3 = true;
	}

	/*
	if (digitalRead(BUTTON_1) == HIGH){
		Serial.println("button1");
	}
	if (digitalRead(BUTTON_2) == HIGH){
		Serial.println("button2");
	}
	if (digitalRead(BUTTON_2) == HIGH){
		Serial.println("button3");
	}
	*/

}


// this goes through the song and adds notes to the screen from it.
void addNotesFromSong(){
	if (counter2 == 0){
		song_counter_1 = 0;
		song_counter_2 = 0;
		song_counter_3 = 0;
	}

	// following 3 if statements check if the note should be added from each
	// song array
	if (counter2 == song_1[song_counter_1]){

		// add note if it is in the array
		screen_notes1[song_counter_1] = addNote(1);
		// increase counter
		song_counter_1++;
	}
	// same logic as above for 2nd instrument
	if (counter2 == song_2[song_counter_2]){
		screen_notes2[song_counter_2] = addNote(2);
		song_counter_2++;
	}
	// same logic as above for 3rd instrument
	if (counter2 == song_3[song_counter_3]){
		screen_notes3[song_counter_3] = addNote(3);
		song_counter_3++;
	}
}


// Advances all notes currently on the board.
void advanceAllRenderedNotes(){
	for (int i = 0; i < MAX_RENDERED_NOTES; i++){

		// moves forward all currently rendered notes.
		// unrendered notes have an insturment (num) value of -1
		if (screen_notes1[i].num > 0){
			advance(screen_notes1[i]);
		}
		if (screen_notes2[i].num > 0){
			advance(screen_notes2[i]);
		}
		if (screen_notes2[i].num > 0){
			advance(screen_notes3[i]);
		}
		/*
		if (screen_notes1[i].progression > 340 && screen_notes1[i].progression < 350){
			drawInstruments();
		}
		else if (screen_notes2[i].progression > 340 && screen_notes2[i].progression < 350){
			drawInstruments();
		}
		else if (screen_notes3[i].progression > 340 && screen_notes3[i].progression < 350){
			drawInstruments();
		}
		*/
	}
}


// adds note to the board
Note addNote(int num){

	// creates not structure and initializes it to given values
	Note n;
	n.progression = 0;
	n.num = num;
	return n;
}


// Adds vertical lines for the paths of the notes.
void addLine(int num) {
	lines = num;

	// n lines gives n+1 equal segments in between lines
	num++;

	//creates intervals
	int interval = ScreenHeight / num;

	// logic for adding to nice intervals
	for (int i = 0; i < lines; i++){
		tft.drawRect((interval + i*interval - 1),0,   4,ScreenWidth,  ILI9341_BLUE);
	}
}


// draws the note on the screen.
// uses basic OOP principles using structs.
void drawNote(int progression, int num){

	// draws note at given progression
	Note n;
	n.progression = progression;
	n.num = num;
	drawNote(n);
}


// progression is from 0 to 320
// num is from 1 to @param lines
void drawNote(Note n){

	// gets values from note passed to this function
	int num = n.num;
	int progression = n.progression;

	// these calculations pics where to draw it with the information from Note
	int interval = ScreenHeight / (lines);
	int centerX = (num-1)*interval + 1;

	// draws on the screen
	// drawing a note is fairly expensive when there are many of themlmao
	tft.fillRect(centerX-6, progression-6, 13,13, ILI9341_RED);
	tft.fillRect(centerX - 4, progression-4, 9,9, ILI9341_YELLOW);
}


// advanced note one pixel forward.
void advance(Note &n){

	// this information gets note value and calculates where to draw it and stuff
	int progression = n.progression;
	int num = n.num;

	if (num > 3){
		num -= 3;
	}

	num++;
	int interval = ScreenHeight / (lines+1);
	int centerX = (num-1)*interval + 1;

	// removes and changes only neccesary pixels.
	// without this accuracy moving the notes is far too slow
	tft.fillRect(centerX-6, progression-6, 13,1, ILI9341_BLACK);
	tft.fillRect(centerX-6, progression+7, 13,1, ILI9341_RED);
	tft.drawPixel(centerX-2, progression-6, ILI9341_BLUE);
	tft.drawPixel(centerX+1, progression-6, ILI9341_BLUE);
	tft.fillRect(centerX-4, progression-4, 9,1,  ILI9341_RED);
	tft.fillRect(centerX-4, progression+5, 9,1,  ILI9341_YELLOW);

	// updates progression to new, further position
	n.progression++;

	// kills notes when they fall off the screen
	if (progression > 330){
		n.num = -1;
	}
}


// main stuff
int main() {

	setup();
	// sets up 3 lines for the screen.

	modeSelect();

	addLine(3);
	Serial.flush();

	// three circles representing instruments
	drawInstruments();

	while (true) {
		loop();
	}
}


// generates random n-bit integer using voltage fluctuation from
// empty arduino pin
int randomNumber(int bits){
	// reads voltage from open input in. It randomly fluctuates
	uint32_t rand = analogRead(RANDOM_PIN) & 1;
	// loop takes last bit of random voltage and adds it to new random n-bit integer
	for (int i = 0; i < (bits-1); i++){
		// AND with ...0001 to get last bit
		int x = analogRead(RANDOM_PIN) & 1;
		// left shift by 1 to add new bit to end
		rand = rand << 1;
		rand = rand + x;
		// gives time for voltage to fluctuate sufficiently
		delay(20);
	}
	return rand;
}


// all the following code takes length of array for the song to be generated
// and generates it to match patterns and notes
// all in the c-minor key to create
// an okay sounding song
void returnMusic(int len){

	// goes through all the bars of the song (each bar is 5 notes so each note)
	// is length of a quarter note
	// this means bpm can be easily calculated using delay on loop from above in
	// the code
	for (int i = 0; i < len/4; i++){

		// 5 bits is sufficient
		int a = randomNumber(5);

		/*
		Serial.print("oooh: ");
		Serial.println(a);
		*/

		// random number corresponds to different patterns and notes.
		if (a % 16  == 0){

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_1[j];
			}

		}
		else if (a % 16 == 1){

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_2[j];
			}

		}
		else if (a % 16 == 2) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_3[j];
			}

		}
		else if (a % 16 == 3) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_4[j];
			}

		}
		else if (a % 16 == 4) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_5[j];
			}

		}
		else if (a % 16 == 5) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_6[j];
			}

		}
		else if (a % 16 == 6) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_7[j];
			}

		}
		else if (a % 16 == 7) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_8[j];
			}

		}
		else if (a % 16 == 8) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_9[j];
			}

		}
		else if (a % 16 == 9) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_10[j];
			}

		}
		else if (a % 16 == 10) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_11[j];
			}

		}
		else if (a % 16 == 11) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_12[j];
			}

		}
		else if (a % 16 == 12) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_13[j];
			}

		}
		else if (a % 16 == 13) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_14[j];
			}

		}
		else if (a % 16 == 14) {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_15[j];
			}

		}
		else {

			for (int j = 0; j < 4; j++){
				music[((i * 4)+j)] = C_MIN_PATTERN_16[j];
			}

		}

	}

  // for debugging purposes
	/*
	for (int i = 0; i < 20; i++){
		Serial.print("this number: ");
		Serial.println(music[i]);
	}
	*/

}

/*                     HOW THIS IS GONNA WORK

1. Scanner: Every second scans array of notes. if there is a match, note is added
	 				  to the screen.

2. Advancement: Every loop the pixels are advanced once. Array holds all notes
								and goes through them and advanced them

3. Playing: If the button is pressed, the clicking segment is checked for
						available note. If it is then the user is given one point

*/

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

const int TFT_DC = 9;
const int TFT_CS = 10;

const int ScreenWidth = 320;
const int ScreenHeight = 240;

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
/*
int song_1 = [1, 3, 5,  7,  9, 10, 11, 17, 25, 32];
int song_2 = [2, 6, 8, 12, 19, 20, 22, 28, 30, 31];
int song_2 = [0, 13, 36];
*/

// these arrays holds all the notes on the screen to be rendered
// max of 30 notes to be rendered
Note screen_notes1[MAX_RENDERED_NOTES];
Note screen_notes2[MAX_RENDERED_NOTES];
Note screen_notes3[MAX_RENDERED_NOTES];

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// function declarations
void drawNote(Note n);
void drawNote(int progression, int n);
Note addNote(int num);
void advance(Note &n);
void advanceAllRenderedNotes();


// initializes stuff
void setup(){
	init();
	Serial.begin(9600);
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);
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

	screen_notes1[0] = addNote(1);
	screen_notes2[0] = addNote(2);
}

// game loop
void loop(){

	advanceAllRenderedNotes();


	delay(17);
}


// Advances all notes currently on the board.
void advanceAllRenderedNotes(){
	for (int i = 0; i < MAX_RENDERED_NOTES; i++){
		if (screen_notes1[i].num > 0){
			advance(screen_notes1[i]);
		}
		if (screen_notes2[i].num > 0){
			advance(screen_notes2[i]);
		}
		if (screen_notes2[i].num > 0){
			advance(screen_notes3[i]);
		}
	}
}


// adds note to the board
Note addNote(int num){
	Note n;
	n.progression = 0;
	n.num = num;
	return n;
}


// Adds vertical lines for the paths of the notes.
void addLine(int num) {
	lines = num;
	num++;
	int interval = ScreenHeight / num;
	for (int i = 0; i < lines; i++){
		tft.drawRect((interval + i*interval - 1),0,   4,ScreenWidth,  ILI9341_BLUE);
	}
}

// draws the note on the screen
void drawNote(int progression, int num){
	Note n;
	n.progression = progression;
	n.num = num;
	drawNote(n);
}


// progression is from 0 to 320
// num is from 1 to @param lines
void drawNote(Note n){
	int num = n.num;
	int progression = n.progression;
	int interval = ScreenHeight / (lines);
	int centerX = (num-1)*interval + 1;
	tft.fillRect(centerX-6, progression-6, 13,13, ILI9341_RED);
	tft.fillRect(centerX - 4, progression-4, 9,9, ILI9341_YELLOW);
}


// advanced note one pixel forward.
void advance(Note &n){
	int progression = n.progression;
	int num = n.num;
	num++;
	int interval = ScreenHeight / (lines+1);
	int centerX = (num-1)*interval + 1;

	tft.fillRect(centerX-6, progression-6, 13,1, ILI9341_BLACK);
	tft.fillRect(centerX-6, progression+7, 13,1, ILI9341_RED);
	tft.drawPixel(centerX-2, progression-6, ILI9341_BLUE);
	tft.drawPixel(centerX+1, progression-6, ILI9341_BLUE);
	tft.fillRect(centerX-4, progression-4, 9,1,  ILI9341_RED);
	tft.fillRect(centerX-4, progression+5, 9,1,  ILI9341_YELLOW);
	n.progression++;
	if (progression > 350){
		n.progression = 0;
	}
}



int main() {

	setup();
	// sets up 3 lines for the screen.
	addLine(3);
	Serial.flush();

	while (true) {
		loop();
	}
}

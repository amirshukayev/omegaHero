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

const int BUTTON_1 = 11;
const int BUTTON_2 = 12;
const int BUTTON_3 = 13;

// macros because A2 instead of just an int
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin

// width/height of the display when rotated horizontally
#define TFT_WIDTH  320
#define TFT_HEIGHT 240

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
int song_1[] = {1, 3, 5,  7,  9, 10, 11, 17, 25, 32};
int song_2[] = {2, 6, 8, 12, 19, 20, 22, 28, 30, 31};
int song_3[] = {0, 13, 36};

// these arrays holds all the notes on the screen to be rendered
// max of 30 notes to be rendered
Note screen_notes1[MAX_RENDERED_NOTES];
Note screen_notes2[MAX_RENDERED_NOTES];
Note screen_notes3[MAX_RENDERED_NOTES];
int song_counter_1 = 0;
int song_counter_2 = 0;
int song_counter_3 = 0;

// creating the display screen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// creating the touchscreen
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// function declarations
void drawNote(Note n);
void drawNote(int progression, int n);
Note addNote(int num);
void advance(Note &n);
void advanceAllRenderedNotes();
void addNotesFromSong();

// this will count frames from 1-60
int counter1 = 0;
// this will count seconds for the songs;
int counter2 = 0;

// calibration data for the touch screen, obtained from documentation
// the minimum/maximum possible readings from the touch point
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// thresholds to determine if there was a touch
#define MINPRESSURE   10
#define MAXPRESSURE 1000


// initializes stuff
void setup(){
	init();
	Serial.begin(9600);
	tft.begin();
	tft.fillScreen(ILI9341_BLACK);

	pinMode(BUTTON_1, INPUT_PULLUP);
	pinMode(BUTTON_2, INPUT_PULLUP);
	pinMode(BUTTON_3, INPUT_PULLUP);

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



void processTouch(){
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

	if (touchY < 300 && touchX < ScreenHeight/3){
		Serial.println("First.");
	}
	else if (touchY < 300 && touchX > (ScreenHeight * 2)/3){
		Serial.println("Third.");
	}
	else if (touchY < 300){
		Serial.println("Second.");
	}


}



// game loop
void loop(){
	processTouch();
	advanceAllRenderedNotes();
	// delay of 17 gives us approximately 60 frames per second.
	delay(8);

	counter1++;
	if (counter1 == 60){
		addNotesFromSong();
		counter2++;
		counter1 = 0;
	}
	// redraw instruments every 0.25 seconds
	if (counter1 == 15){
		drawInstruments();
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
	if (counter2 == song_1[song_counter_1]){
		screen_notes1[song_counter_1] = addNote(1);
		song_counter_1++;;
	}
	if (counter2 == song_2[song_counter_2]){
		screen_notes2[song_counter_2] = addNote(2);
		song_counter_2++;
	}
	if (counter2 == song_3[song_counter_3]){
		screen_notes3[song_counter_3] = addNote(3);
		song_counter_3++;
	}
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
		n.num = -1;
	}
}


// main stuff
int main() {

	setup();
	// sets up 3 lines for the screen.
	addLine(3);
	Serial.flush();

	drawInstruments();

	while (true) {
		loop();
	}
}

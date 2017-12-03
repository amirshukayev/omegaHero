
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

const int SPEAKER_PIN = 47;

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
int song_2[] = {2, 6, 8, 12, 16, 19, 20, 22, 28, 30, 31};
int song_3[] = {0, 13, 14, 18, 21, 24, 26, 27, 29};

// these arrays holds all the notes on the screen to be rendered
// max of 30 notes to be rendered
Note screen_notes1[MAX_RENDERED_NOTES];
Note screen_notes2[MAX_RENDERED_NOTES];
Note screen_notes3[MAX_RENDERED_NOTES];
int song_counter_1 = 0;
int song_counter_2 = 0;
int song_counter_3 = 0;

boolean cooldown1 = false;
boolean cooldown2 = false;
boolean cooldown3 = false;

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
int randomNumber(int bits);
void returnMusic(int len);
void onDeath();
void restart();
void addLine(int num);

const int note_c = 262;
const int note_d = 294;
const int note_e = 330;
const int note_f = 349;
const int note_g = 392;
const int note_a = 440;
const int note_b = 494;
const int node_C = 523;

const int C_MAJ[] = {262,294,330,349,392,440,494,523};
const int C_MIN[] = {65/*C*/, 73, 78, 87, 98, 104, 117, 130 /*C*/,
	 									147, 156, 175, 196, 208, 233, 262 /*c*/,
										294, 311, 349, 392, 415, 466, 523 /*C*/ };

const int C_MIN_PATTERN_1[] = { C_MIN[7], C_MIN[11], C_MIN[9], C_MIN[11] };
const int C_MIN_PATTERN_2[] = { C_MIN[7], C_MIN[12], C_MIN[9], C_MIN[12] };
const int C_MIN_PATTERN_3[] = { C_MIN[7], C_MIN[8],  C_MIN[9], C_MIN[11] };
const int C_MIN_PATTERN_4[] = { C_MIN[11], C_MIN[8], C_MIN[10], C_MIN[9] };

const int C_MIN_PATTERN_5[] = { C_MIN[0], C_MIN[1], C_MIN[2], C_MIN[3] };
const int C_MIN_PATTERN_6[] = { C_MIN[4], C_MIN[3], C_MIN[2], C_MIN[1] };
const int C_MIN_PATTERN_7[] = { C_MIN[6], C_MIN[5], C_MIN[4], C_MIN[3] };
const int C_MIN_PATTERN_8[] = { C_MIN[3], C_MIN[4], C_MIN[5], C_MIN[6] };

const int C_MIN_PATTERN_9[] = { C_MIN[14], C_MIN[15], C_MIN[16], C_MIN[17] };
const int C_MIN_PATTERN_10[] = { C_MIN[15], C_MIN[14], C_MIN[13], C_MIN[12] };
const int C_MIN_PATTERN_11[] = { C_MIN[16], C_MIN[17], C_MIN[18], C_MIN[19] };
const int C_MIN_PATTERN_12[] = { C_MIN[18], C_MIN[17], C_MIN[18], C_MIN[16] };

const int C_MIN_PATTERN_13[] = { C_MIN[18], C_MIN[18], C_MIN[17], C_MIN[18] };
const int C_MIN_PATTERN_14[] = { C_MIN[17], C_MIN[17], C_MIN[16], C_MIN[17] };
const int C_MIN_PATTERN_15[] = { C_MIN[16], C_MIN[16], C_MIN[19], C_MIN[16] };
const int C_MIN_PATTERN_16[] = { C_MIN[14], C_MIN[18], C_MIN[14], C_MIN[14] };

const int RANDOM_PIN = 33;



// this will count frames from 1-60
int counter1 = 0;
// this will count seconds for the songs;
int counter2 = 0;
// player points
int points = 0;
bool infinteMode = true;
bool hardMode = false;

int music[20];
int length_music = 20;

int lives = 3;

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

	tft.setCursor(0,0);
	tft.print(points);

	pinMode(BUTTON_1, INPUT_PULLUP);
	pinMode(BUTTON_2, INPUT_PULLUP);
	pinMode(BUTTON_3, INPUT_PULLUP);

	pinMode(SPEAKER_PIN, OUTPUT);

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

int toneCounter = 0;
int song_rand[4];

void onDeath(){

	tft.fillScreen(ILI9341_BLACK);
	tft.setTextColor(ILI9341_RED);
	tft.setTextSize(4);
	tft.setCursor(0, 20);
	tft.println("YOU DIED:(");

	tft.setCursor(40, 60);
	tft.setTextColor(ILI9341_YELLOW);
	tft.setTextSize(3);
	tft.print("SCORE: ");
	tft.println(points);

	tft.setTextSize(2);
	tft.setCursor(10, 200);
	tft.print("touch to continue");

	tone(SPEAKER_PIN,988 ,400 );
	delay(400);

	tone(SPEAKER_PIN,1397 ,400 );
	delay(600);

	tone(SPEAKER_PIN,1397 ,200 );
	delay(200);

	tone(SPEAKER_PIN,1397 ,200 );
	delay(200);

	tone(SPEAKER_PIN,1319 ,200 );
	delay(200);

	tone(SPEAKER_PIN,1175 ,200 );
	delay(200);

	tone(SPEAKER_PIN,1047 ,300 );
	delay(300);

	tone(SPEAKER_PIN,659 ,200 );
	delay(210);

	tone(SPEAKER_PIN,659 ,200 );
	delay(210);


	while(true){
		TSPoint touch = ts.getPoint();

		// if they barely press it or if the wind blows on it or something
		if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
			// no touch, just quit
		} else {
			restart();
		}
	}

}

void restart(){

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


	while(true){
		loop();
	}
}

void playNote(){

	tone(SPEAKER_PIN, music[toneCounter], 600);
	Serial.println(music[toneCounter]);
	toneCounter++;
	if (toneCounter >= length_music){
		// resets
		toneCounter = 0;
		// makes some more music!
		returnMusic(length_music);
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

	if (touchX < 100 && touchY < ScreenWidth/4){
		for (int i = 0; i < MAX_RENDERED_NOTES; i++){
			if (screen_notes1[i].progression > 275 && screen_notes1[i].num != -1 && screen_notes1[i].progression < 310){
				if (cooldown1){
					points++;
					cooldown1 = false;
					tft.fillRect(0,0, 20,20, ILI9341_BLACK);
					tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
					tft.setCursor(0,0);
					tft.print(points);
					playNote();
					break;
				}
			}
		}

		if (cooldown1) {
			if (hardMode){
				points--;
			}


		  tft.fillRect(0,0, 11,11, ILI9341_BLACK);
			tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			tft.setCursor(0,0);
			tft.print(points);
			cooldown1 = false;

			lives--;
		}
	}

	else if (touchX < 100 && touchY > (ScreenWidth * 2)/4){
		for (int i = 0; i < MAX_RENDERED_NOTES; i++){
			if (screen_notes3[i].progression > 275 && screen_notes3[i].num != -1 && screen_notes3[i].progression < 310){
				if (cooldown3){
					points++;

					cooldown3 = false;
					tft.fillRect(0,0, 11,11, ILI9341_BLACK);
					tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
					tft.setCursor(0,0);
					tft.print(points);
					playNote();
					break;
				}
			}
		}

		if (cooldown3) {
			if (hardMode){
				points--;
			}
			tft.fillRect(0,0, 11,11, ILI9341_BLACK);
			tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			tft.setCursor(0,0);
			tft.print(points);
			cooldown3 = false;
			lives--;
		}
	}

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
		}
	}

	if (lives == 0){
		onDeath();
	}
}

int delayVal = 8;

// game loop
void loop(){
	processTouch();
	advanceAllRenderedNotes();
	// delay of 17 gives us approximately 60 frames per second.
	delay(delayVal);

	counter1++;
	if (counter1 == 60){
		addNotesFromSong();
		counter2++;

		// RESETS SONG;
		if (infinteMode){
			if (counter2 > 20){
				counter2 = 0;

				if (delayVal > 2){
					delayVal--;
				}

			}
		}

		counter1 = 0;
	}
	// redraw instruments every 0.25 seconds
	if (counter1 == 15){
		drawInstruments();
	}

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
	if (counter2 == song_1[song_counter_1]){
		screen_notes1[song_counter_1] = addNote(1);
		song_counter_1++;
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

	// kills notes when they fall off the screen
	if (progression > 330){
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


// generates random n-bit integer
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

void returnMusic(int len){


	for (int i = 0; i < len/4; i++){

		int a = randomNumber(5);

		/*
		Serial.print("oooh: ");
		Serial.println(a);
		*/

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

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include "menus.h"

#ifndef MINPRESSURE// thresholds to determine if there was a touch
#define MINPRESSURE   10
#endif
#ifndef MAXPRESSURE
#define MAXPRESSURE 1000
#endif

void drawBack(){
	// draws back button used to return to the previous screens
	tft.fillCircle(20, 20, 20, ILI9341_RED);
	tft.fillCircle(20, 20, 18, ILI9341_WHITE);
	tft.setCursor(10, 17);
	tft.setTextSize(1);
	tft.setTextColor(ILI9341_BLACK);
	tft.print("BACK");
}

void drawScroll(int type){ //0 = both active, 1 = top inactive, 2 = bottom inactive, 3 = both inactive
	// draws scroll up and down buttons used to go through highscores and song pages
	tft.fillRoundRect(204, 60, 28, 110, 2, ILI9341_RED);
	tft.fillRoundRect(205, 61, 26, 108, 2, ILI9341_WHITE);
	tft.fillRoundRect(204, 190, 28, 110, 2, ILI9341_RED);
	tft.fillRoundRect(205, 191, 26, 108, 2, ILI9341_WHITE);
	if(type == 0){
		tft.fillTriangle(218,91, 210,140, 226,140, ILI9341_BLACK);
		tft.fillTriangle(218,270, 210,230, 226,230, ILI9341_BLACK);
	}else if(type == 1){
		tft.fillTriangle(218,270, 210,230, 226,230, ILI9341_BLACK);
	}else if(type == 2){
		tft.fillTriangle(218,91, 210,140, 226,140, ILI9341_BLACK);
	}
}

void splashScreen(){
	// draws a fancy splash screen with an ascii art logo
	tft.fillScreen(0);
	tft.setCursor(15,30);
	tft.setTextColor(ILI9341_RED);
	tft.setTextSize(3);
	tft.setTextWrap(false);
	tft.println("~OMEGA HERO~");
	tft.setCursor(35, 90);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.println(" .d88888888b.");
	tft.setCursor(35, 108);
	tft.println("d88P\"    \"Y88b ");
	tft.setCursor(35, 126);
	tft.println("888        888 ");
	tft.setCursor(35, 144);
	tft.println("Y88b      d88P ");
	tft.setCursor(35, 162);
	tft.println(" \"88bo  od88\"");
	tft.setCursor(35, 180);
	tft.println("d88888  88888b ");
	tft.setCursor(40, 260);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_RED);
	tft.println("PRESS ANYWHERE");
	tft.setCursor(80,278);
	tft.println("TO PLAY");

	// processes touchscreen taps
	while(true){
		point touch = checkTouch();
		if (touch.x > 0){break;}
	}
}

void modeSelect() {
	//the main menu for the game
	tft.fillScreen(ILI9341_BLACK);
	drawBack();
	tft.setCursor(60, 10);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.print("~MODE SELECT~");
	tft.fillRoundRect(5, 70, 225, 60, 3, ILI9341_WHITE);
	tft.fillRoundRect(5, 140, 225, 60, 3, ILI9341_WHITE);
	tft.fillRoundRect(5, 210, 225, 60, 3, ILI9341_WHITE);
	tft.setTextColor(ILI9341_BLACK);
	tft.setCursor(25, 92);
	tft.print("DIFFICULTY");
	tft.setCursor(25, 162);
	tft.print("SONG SELECT");
	tft.setCursor(25, 232);
	tft.print("HIGHSCORES");

	while(true){
		point touch = checkTouch();
		if (touch.x < 0){continue;}
		if ((touch.x <= 40) & (touch.y <= 40)){splashScreen();}
		if ((touch.y >= 70) & (touch.y <= 130)){endlessDifficultySelect();}
		if ((touch.y >= 140) & (touch.y <= 200)){songSelect();}
		if ((touch.y >= 210) & (touch.y <= 270)){highscores();}
	}
}

void endlessDifficultySelect(){
	tft.fillScreen(0);
	drawBack();
	tft.setCursor(60, 10);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.print("~DIFFICULTY~");
	tft.fillRoundRect(5, 70, 225, 90, 3, ILI9341_WHITE);
	tft.fillRoundRect(5, 170, 225, 90, 3, ILI9341_WHITE);
	tft.setTextColor(ILI9341_BLACK);
	tft.setTextSize(3);
	tft.setCursor(25, 105);
	tft.print("EASY");
	tft.setCursor(25, 205);
	tft.print("HARD");

	bool easy = true;

	tft.fillRoundRect(5, 70, 225, 90, 3, ILI9341_GREEN);
	tft.setCursor(25, 105);
	tft.print("EASY");

	while(true){
		point touch = checkTouch();
		if (touch.x < 0){continue;}
		if ((touch.x <= 40) & (touch.y <= 40)){modeSelect();}
		if ((touch.y >= 70) & (touch.y <= 160)){

			if (easy){

			} else {

				tft.fillRoundRect(5, 70, 225, 90, 3, ILI9341_GREEN);
				tft.setCursor(25, 105);
				tft.print("EASY");

				tft.fillRoundRect(5, 170, 225, 90, 3, ILI9341_WHITE);
				tft.setCursor(25, 205);
				tft.print("HARD");

				lives = 5;

				easy = true;

			}
			/**TODO:easy endless mode*/
			hardMode = false;
			//Serial.println("EZMODE");
			delay(50);
		}
		if ((touch.y >= 170) & (touch.y <= 260)){
			/**TODO:hard endless mode*/
			if (easy){

				tft.fillRoundRect(5, 170, 225, 90, 3, ILI9341_RED);
				tft.setCursor(25, 205);
				tft.print("HARD");

				tft.fillRoundRect(5, 70, 225, 90, 3, ILI9341_WHITE);
				tft.setCursor(25, 105);
				tft.print("EASY");

				lives = 0;

				easy = false;

			} else {

			}
			Serial.println("HARDMODE");
			hardMode = true;
			delay(50);
		}
	}
}

void songSelect(){
	sortMode = 0;
	sortList();
	drawSongSelect();
	bool redrawScroll = true;
	while(true){
		//if the page has changed refresh the page up / down buttons to match
		if (redrawScroll){refreshList(); if((pg == 1) & (pg == maxpages)){drawScroll(3);}else if(pg == 1){drawScroll(1);}else if(pg == maxpages){drawScroll(2);}else{drawScroll(0);}; redrawScroll = false; delay(50);}

		point touch = checkTouch();
		if (touch.x < 0){continue;} //ignore
		if ((touch.x <= 40) & (touch.y <= 40)){modeSelect(); pg = 1; sortMode = 0;} //back
		if ((touch.x >= 204) & (touch.y >= 60) & (touch.y <= 180) & (pg != 1)){pg--; redrawScroll = true;} //page up
		if ((touch.x >= 204) & (touch.y >= 190) & (touch.y <= 300) & (pg != maxpages)){pg++; redrawScroll = true;} //page down

		if ((touch.x >= 10) & (touch.x <= 198) & (touch.y >= 55) & (touch.y <= 124) & (displayedSongs >= 1)){
			songToPlay = 0+3*(pg-1); //TODO this isn't pointing to the displayed song
			Serial.print("play song: ");
			Serial.println(songs[songToPlay].name);
			delay(50);
			pg = 1;
			songTitleScreen();
		} //Song 1

		if ((touch.x >= 10) & (touch.x <= 198) & (touch.y >= 128) & (touch.y <= 202) & (displayedSongs >= 2)){
			songToPlay = 1+3*(pg-1);
			Serial.print("play song: ");
			Serial.println(songs[songToPlay].name);
			delay(50);
			pg = 1;
			songTitleScreen();
		} //Song 2

		if ((touch.x >= 10) & (touch.x <= 198) & (touch.y >= 206) & (touch.y <= 286) & (displayedSongs == 3)){
			songToPlay = 2+3*(pg-1);
			Serial.print("play song ");
			Serial.println(songs[songToPlay].name);
			delay(50);
			pg = 1;
			songTitleScreen();
		} //Song 3

		//conditions for songs being selected
		if ((touch.x >= 10) & (touch.x <= 182) & (touch.y >= 285)){
			sortMenu();
			redrawScroll = true;
		} //sort submenu
	}
}

void drawSongSelect(){
	tft.fillScreen(0);
	drawBack();
	drawScroll(0);
	tft.setCursor(60, 10);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.print("~SONG SELECT~");
	drawSort();
}

void refreshList(){
	//Draws the songs for the current page, sorted appropriately
	int page = pg - 1;														//~FORMAT~
	tft.fillRect(0, 50, 240, 280, ILI9341_BLACK);		//NAME
	tft.setTextColor(ILI9341_WHITE);								//	BY: ARTIST
	tft.setTextSize(1);															//	FROM: GAME
	int x = 15;																			//	LENGTH: MM:SS
	int y = 55;																			//  DIFFICULTY: 0 0 0 (filled in bubbles out of three)
	displayedSongs = 0;
	for (int i = 0; i < constrain(numsongs-(3*page), 1, 3); i++){
		//print name
		tft.setCursor(x*1, y+(78*i)+14*0);
		printString(songs[i+(3*page)].name);

		//print artist
		tft.setCursor(x*2, y+(78*i)+14*1);
		tft.print("BY: ");
		printString(songs[i+(3*page)].artist);

		//print source
		tft.setCursor(x*2, y+(78*i)+14*2);
		tft.print("FROM: ");
		printString(songs[i+(3*page)].game);

		//print length
		tft.setCursor(x*2, y+(78*i)+14*3);
		tft.print("LENGTH: ");
		if((songs[i+(3*page)].length)/60 < 10){tft.print("0");}
		tft.print((songs[i+(3*page)].length)/60);
		tft.print(":");
		if((songs[i+(3*page)].length)%60 < 10){tft.print("0");}
		tft.print(((songs[i+(3*page)]).length)%60); //displays time as mm:ss

		//display difficulty level
		tft.setCursor(x*2, y+(78*i)+14*4);
		tft.print("DIFFICULTY: ");
		int diff = songs[i+(3*page)].difficulty;
		if(diff == 1){
			tft.setTextColor(ILI9341_GREEN, ILI9341_GREEN);
			tft.print("0 ");
			tft.setTextColor(ILI9341_WHITE, ILI9341_WHITE);
			tft.print("0 ");
			tft.print("0 ");
		}
		if(diff == 2){
			tft.setTextColor(ILI9341_YELLOW, ILI9341_YELLOW);
			tft.print("0 ");
			tft.print("0 ");
			tft.setTextColor(ILI9341_WHITE, ILI9341_WHITE);
			tft.print("0 ");
		}
		if(diff == 3){
			tft.setTextColor(ILI9341_RED, ILI9341_RED);
			tft.print("0 ");
			tft.print("0 ");
			tft.print("0 ");
		}
		tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		//draw a rectangle around the information
		tft.drawRoundRect(x-5, (y-5)+(78*i), 188, 74, 3, ILI9341_RED);

		displayedSongs++;
	}
	drawSort();
}

void sortMenu(){
	//bring up sort menu
	tft.setTextColor(ILI9341_WHITE);
	//big menu
	tft.fillRect(0, 56, 240, 318, ILI9341_BLACK);
	tft.drawRoundRect(5, 60, 225, 250, 3, ILI9341_RED);
	tft.setCursor(10, 65);
	tft.setTextSize(2);
	tft.print("SORT BY:");

	tft.fillRoundRect(8, 87, 218, 45, 2, ILI9341_RED);
	tft.fillRoundRect(9, 88, 216, 43, 2, ILI9341_WHITE);

	tft.fillRoundRect(8, 142, 218, 45, 2, ILI9341_RED);
	tft.fillRoundRect(9, 143, 216, 43, 2, ILI9341_WHITE);

	tft.fillRoundRect(8, 197, 218, 45, 2, ILI9341_RED);
	tft.fillRoundRect(9, 198, 216, 43, 2, ILI9341_WHITE);

	tft.fillRoundRect(8, 252, 218, 45, 2, ILI9341_RED);
	tft.fillRoundRect(9, 253, 216, 43, 2, ILI9341_WHITE);
	drawSortMenuText();

	while(true){
		point touch = checkTouch();
		if (touch.x < 0){continue;} //ignore
		if ((touch.x >= 8) & (touch.x <= 230) & (touch.y >= 80) & (touch.y <= 132)){if (sortMode != 0){sortMode = 0; pg = 1; drawSortMenuText(); delay(70); sortList(); pg = 1;}} //name
		if ((touch.x >= 8) & (touch.x <= 230) & (touch.y >= 140) & (touch.y <= 187)){if (sortMode != 1){sortMode = 1; pg = 1; drawSortMenuText(); delay(70); sortList(); pg = 1;}} //length
		if ((touch.x >= 8) & (touch.x <= 230) & (touch.y >= 195) & (touch.y <= 242)){if (sortMode != 2){sortMode = 2; pg = 1; drawSortMenuText(); delay(70); sortList(); pg = 1;}} //difficulty
		if ((touch.x >= 8) & (touch.x <= 230) & (touch.y >= 250) & (touch.y <= 297)){if (sortMode != 3){sortMode = 3; pg = 1; drawSortMenuText(); delay(70); sortList(); pg = 1;}} //artist
		drawSongSelect();
		break;
	}
}

void drawSort(){
	tft.fillRoundRect(10, 285, 172, 32, 2, ILI9341_RED);
	tft.fillRoundRect(11, 286, 170, 30, 2, ILI9341_WHITE);
	tft.setCursor(70, 294);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_BLACK);
	tft.print("SORT");
}


void drawSortMenuText(){
	tft.setTextSize(1);
	tft.setTextColor(ILI9341_BLACK);

	tft.setCursor(15, 107);
	if(sortMode == 0){tft.setTextColor(ILI9341_GREEN);}
	tft.print("TITLE (ALPHABETICAL)");
	if(sortMode == 0){tft.setTextColor(ILI9341_BLACK);}

	tft.setCursor(15, 162);
	if(sortMode == 1){tft.setTextColor(ILI9341_GREEN);}
	tft.print("LENGTH (INCREASING)");
	if(sortMode == 1){tft.setTextColor(ILI9341_BLACK);}

	tft.setCursor(15, 217);
	if(sortMode == 2){tft.setTextColor(ILI9341_GREEN);}
	tft.print("DIFFICULTY (INCREASING)");
	if(sortMode == 2){tft.setTextColor(ILI9341_BLACK);}

	tft.setCursor(15, 272);
	if(sortMode == 3){tft.setTextColor(ILI9341_GREEN);}
	tft.print("GAME (ALPHABETICAL)");
	if(sortMode == 3){tft.setTextColor(ILI9341_BLACK);}
}

void highscores(){
	int page = 1;
	int pagemax = ceil(numsongs/5);
	if(numsongs%5){pagemax++;}
	tft.fillScreen(0);

	int maxScore = 0;
	for (int i = 0; i < numsongs; i++){
		if (songs[i].hiscore > maxScore){maxScore = songs[i].hiscore;}
	}
	if (maxScore > 0){
		sortMode = 4;
	}else{
		sortMode = 0;
	}
	sortList();
	sortMode = 0;

	drawBack();
	drawScroll(0);
	refreshScores(page);
	bool redrawScroll = true;
	tft.setCursor(60, 10);
	tft.setTextSize(2);
	tft.setTextColor(ILI9341_WHITE);
	tft.print("~HIGH SCORES~");
	while(true){
		point touch = checkTouch();
		if (redrawScroll){if((page == 1) & (page == pagemax)){drawScroll(3);}else if(page == 1){drawScroll(1);}else if(page == pagemax){drawScroll(2);}else{drawScroll(0);}; redrawScroll = false;}
		if (touch.x < 0){continue;}
		if ((touch.x <= 40) & (touch.y <= 40)){modeSelect();}
		if ((touch.x >= 204) & (touch.y >= 60) & (touch.y <= 180) & (page != 1)){page--; refreshScores(page); redrawScroll = true;} //page up
		if ((touch.x >= 204) & (touch.y >= 190) & (touch.y <= 300) & (page != pagemax)){page++; refreshScores(page); redrawScroll = true;} //page down
	}
}

void refreshScores(int page){
	//Draws the highscores for all songs
	page--;
	tft.fillRect(0, 54, 240, 280, ILI9341_BLACK);
	tft.setTextColor(ILI9341_WHITE);
	tft.setTextSize(1);
	int x = 15;
	int y = 65;
	for (int i = 0; i < constrain(numsongs-(5*page), 1, 5); i++){
		tft.setCursor(x,y+(50*i));
		String temp = songs[i+(5*page)].name;
		printString(temp);
		tft.setCursor(x*2,y+(50*i)+18);
		tft.print("SCORE: ");
		tft.print(songs[i+(5*page)].hiscore);
	}
}



void songTitleScreen(){
	//Displays a countdown before starting a song

	tft.fillScreen(ILI9341_BLACK);
	tft.setTextSize(3);
	tft.setTextColor(ILI9341_WHITE);
	tft.setCursor(0,0);
	tft.print("NOW PLAYING:");
	tft.setCursor(0,25);
	tft.setTextSize(1);
	printString(songs[songToPlay].name);
	tft.setTextSize(6);


	tft.setCursor(100,100);
	tft.print("3");
	tone(SPEAKER_PIN,440,500);
	delay(1000);
	tft.setCursor(100,100);
	tft.fillRect(80,100,50,50,ILI9341_BLACK);

	tft.print("2");
	tone(SPEAKER_PIN,440,500);
	delay(1000);
	tft.setCursor(100,100);
	tft.fillRect(80,100,50,50,ILI9341_BLACK);

	tft.print("1");
	tone(SPEAKER_PIN,440,500);
	delay(1000);

	tone(SPEAKER_PIN,880,500);

	restart();

}

// played when lives is reduced to 0
void onDeath(){
	Serial.println("");
	Serial.print("Current Song: ");
	Serial.println(songs[songToPlay].name);
	Serial.print("Current hiscore: ");
	Serial.println(songs[songToPlay].hiscore);
	Serial.print("Score: ");
	Serial.println(points);
	if(points > songs[songToPlay].hiscore){
		songs[songToPlay].hiscore = points;
		Serial.print("New Highscore: ");
		Serial.println(songs[songToPlay].hiscore);
	}else{
		Serial.println("No new high score :(");
	}

	// fils screen and adds text
	tft.fillScreen(ILI9341_BLACK);
	tft.setTextColor(ILI9341_RED);
	tft.setTextSize(4);
	tft.setCursor(0, 20);
	tft.println("YOU DIED:(");

	// adds text displaying score upon death
	tft.setCursor(40, 60);
	tft.setTextColor(ILI9341_YELLOW);
	tft.setTextSize(3);
	tft.print("SCORE: ");
	tft.println(points);

	// displays instructions for continuing the game
	tft.setTextSize(2);
	tft.setCursor(10, 200);
	tft.print("touch to continue");


	// this plays the tune from Mario
	// it is the death tune and is played as followed:

	//BF FFEDC E
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


	// waiting for player to tap to continue
	while(true){
		TSPoint touch = ts.getPoint();

		// if they barely press it or if the wind blows on it or something
		if (touch.z < MINPRESSURE || touch.z > MAXPRESSURE) {
			// no touch, just quit
		} else {
			// go to next state
			modeSelect();
		}
	}
}

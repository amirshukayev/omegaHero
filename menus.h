//Handles drawing the menu system for the navigation and configuration of the game by the player

#ifndef _MENUS_H
#define _MENUS_H

#include "song.h"

//Global Variables
//structs used in this program
struct point{int x; int y;}; //used to interpret touch

extern int numsongs;
extern song songs[];

//global vars for song listings
extern int pg;
extern int maxpages;

extern int displayedSongs;

// external variables
extern Adafruit_ILI9341 tft;
extern TouchScreen ts;
extern int lives;
extern bool hardMode;
extern int sortMode;
extern int songToPlay;
extern const int SPEAKER_PIN;
extern int points;

//Function Declarations
void drawBack();
void drawScroll(int type);

void splashScreen();

void modeSelect();

void endlessDifficultySelect();

void songSelect();
void drawSongSelect();
void refreshList();
void sortMenu();
void drawSort();
void drawSortMenuText();

void highscores();
void refreshScores(int page);

void songTitleScreen();

void onDeath();

// external functions
point checkTouch();
void sortList();
void printString(String s);
void restart();

#endif

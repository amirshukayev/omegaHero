// Sorts values

#ifndef _SORTING_H
#define _SORTING_H

#include "song.h"

//Global Variables
extern int sortMode;

//external Variables
extern int numsongs;
//extern struct song{String name; String game; int length; int difficulty; String artist; int hiscore;};
extern song songs[];
extern int tempArr[];

//Function Declarations
void sortList();
void sortSongs(int lower, int upper);

#endif

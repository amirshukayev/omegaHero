#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>
#include "sorting.h"

void sortList(){
	sortSongs(0, numsongs);
	Serial.println("");
	Serial.println("~postsort~");
	for(int i = 0; i < numsongs; i++){
		Serial.println(songs[i].name);
		Serial.print("   L:");
		Serial.print(songs[i].length);
		Serial.print(" DIFF:");
		Serial.print(songs[i].difficulty);
		Serial.print(" GAME:");
		Serial.println(songs[i].game);
	}
	Serial.println("");
}

//Sorts songs based on current sorting method
	//0: name (alphabetical)
	//1: length (ascending)
	//2: difficulty (ascending)
	//3; game (alphabetical)
	//4: highscore (descending)
void sortSongs(int lower, int upper){ //Quicksort: O(nlogn) (usually), O(n^2) worst case
	int tempArr[numsongs];
	for(int i = 0; i < numsongs; i++){
		if (sortMode == 0){tempArr[i] = (int)songs[i].name[0];}
		if (sortMode == 1){tempArr[i] = songs[i].length;}
		if (sortMode == 2){tempArr[i] = songs[i].difficulty;}
		if (sortMode == 3){tempArr[i] = (int)songs[i].game[0];}
		if (sortMode == 4){tempArr[i] = -songs[i].hiscore;}
	}
	int l = lower;
	int u = upper;
	song temp;
	int piv = tempArr[(l+u)/2];
	while(l <= u){
		while(tempArr[l] < piv){++l;}
		while(tempArr[u] > piv){--u;}
		if(!(l > u)){temp = songs[l]; songs[l] = songs[u]; songs[u] = temp; ++l; --u;}
	}
	if (lower < u){sortSongs(lower, u);}
	if (l < upper){sortSongs(l, upper);}
}

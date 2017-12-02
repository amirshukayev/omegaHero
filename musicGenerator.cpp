#include "musicGenerator.h"
#include <Arduino.h>

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

int* returnMusic(int len){

	int a[] = {1, 2, 3, 4, 5, 6, 7};

	return a;

}

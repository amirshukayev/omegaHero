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

void returnMusic(int *music, int len){

	for (int i = 0; i < len/4; i++){

		int a = randomNumber(2);

		/*
		Serial.print("oooh: ");
		Serial.println(a);
		*/

		if (a == 0){

			for (int j = 0; j < 4; j++){

				music[((i * 4)+j)] = C_MIN_PATTERN_1[j];

			}

		}
		else if (a == 1){

			for (int j = 0; j < 4; j++){

				music[((i * 4)+j)] = C_MIN_PATTERN_2[j];

			}

		}
		else if (a == 2) {

			for (int j = 0; j < 4; j++){

				music[((i * 4)+j)] = C_MIN_PATTERN_3[j];

			}

		}
		else {

			for (int j = 0; j < 4; j++){

				music[((i * 4)+j)] = C_MIN_PATTERN_4[j];

			}

		}

	}

	for (int i = 0; i < 20; i++){
		Serial.print("this number: ");
		Serial.println(music[i]);
	}

}

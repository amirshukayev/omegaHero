#ifndef _MUSIC_GENERATOR_H
#define _MUSIC_GENERATOR_H

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

const int RANDOM_PIN = 33;

void returnMusic(int *music, int len);
int randomNumber(int bits);


#endif

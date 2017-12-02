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
const int C_MIN[] = {};

const int RANDOM_PIN = 33;

int* returnMusic(int len);
int randomNumber(int bits);


#endif

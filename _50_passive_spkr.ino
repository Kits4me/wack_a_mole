/*
 * this file uses pitches.h which has constants for all notes
 */
#include "pitches.h";
int count = 0;
int count2 = 0;
int count3 = 0;
int MAX_COUNT = 24;

void setup_pSpeaker() {
 pinMode(pinBzr, OUTPUT);
 playFinish();
}


void playGood(){
      tone( pinBzr, NOTE_G5, 200);
      delay(200);
}
void playWrong() {
      tone( pinBzr, NOTE_D5, 200);
      delay(200);
}
int tn[]={NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_A5,NOTE_B5};
int wrong_tone[]={NOTE_D5,NOTE_E5,NOTE_CS5};
int wrong_times[]={400,200,250};
void playFinish() {
  for (int k=0;k<3;k++){
      tone( pinBzr, wrong_tone[k], wrong_times[k]);
      delay(wrong_times[k]);
  }
}


int good_tone[]={NOTE_G5,NOTE_B5};
int good_times[]={200,400};
void playStart(){
  for (int k=0;k<2;k++){
      tone( pinBzr, good_tone[k], good_times[k]);
      delay(good_times[k]);
  }
}


#define pinLed_R_1 12
#define pinLed_G_1 14
#define pinLed_B_1 27
#define pinLed_Y_1 26
#define pinLed_R_2 25
#define pinLed_G_2 33
#define pinLed_B_2 32
#define pinLed_Y_2 17

#define pinBtn_R_1 23
#define pinBtn_G_1 22
#define pinBtn_B_1 15
#define pinBtn_Y_1 21
#define pinBtn_R_2 19
#define pinBtn_G_2 2
#define pinBtn_B_2 0
#define pinBtn_Y_2 4

#define pinBzr 13
#define pinLvlChooser 36 // A0
#define pin_in_air 39

// #define CLK_1637  18    // clk
// #define DIO_1637  5     // dio

#define LED_NUM 8
int Leds[LED_NUM] = {pinLed_R_1,pinLed_G_1,pinLed_B_1,pinLed_Y_1,pinLed_R_2,pinLed_G_2,pinLed_B_2,pinLed_Y_2};
int Btns[LED_NUM] = {pinBtn_R_1,pinBtn_G_1,pinBtn_B_1,pinBtn_Y_1,pinBtn_R_2,pinBtn_G_2,pinBtn_B_2,pinBtn_Y_2};
int Lvl=1;
int score=0;
int min_off_time=400;
short number_of_ON_leds=0;
bool LedIsOn[LED_NUM];
bool BtnIsPressed[LED_NUM];
unsigned long LedOnStartTime[LED_NUM];
int LedOn_LengthTime[LED_NUM];
unsigned long lastToggleTime;
unsigned long gameStartedTime;
short CurrentLedIdxForWaitPhase;
int game_length = 60000;
short curr_mode;
//--- modes
#define WAIT_FOR_GAME   51
#define START_NEW_GAME  52
#define GAME_ON         53
#define GAME_FINISHED   54

void setup() {
  setup_pSpeaker();

  setup_disp();
  for(int k=0;k<LED_NUM;k++){
    pinMode(Leds[k],OUTPUT);
    pinMode(Btns[k],INPUT_PULLUP);
    TurnLedOff(k);
  }
  start_wait_for_new_game();
  randomSeed(analogRead(pin_in_air));
}
void start_wait_for_new_game(){
  curr_mode=WAIT_FOR_GAME;
  lastToggleTime=millis();
  CurrentLedIdxForWaitPhase=0;
  TurnLedOn(CurrentLedIdxForWaitPhase);
}
void TurnLedOff(short chnl){
  digitalWrite(Leds[chnl],LOW);
  LedIsOn[chnl]=false;
  LedOnStartTime[chnl]=millis();
  number_of_ON_leds--;
}
void TurnLedOn(short chnl){
  digitalWrite(Leds[chnl],HIGH);
  LedIsOn[chnl]=true;
  LedOnStartTime[chnl]=millis();
  number_of_ON_leds++;
}
void loop() {
  switch(curr_mode){
    case WAIT_FOR_GAME : wait_for_game();   break;
    case START_NEW_GAME: start_new_game();  break;
    case GAME_ON       : GameHandler();     break;
    case GAME_FINISHED : finish_game();     break;
  }
  showScore();
}
void wait_for_game(){
  //any btn starts a game
  for(int k=0;k<LED_NUM;k++){
    if(digitalRead(Btns[k])==LOW){
      while(digitalRead(Btns[k])==LOW){delay(50);}
      TurnLedOff(CurrentLedIdxForWaitPhase);
      curr_mode=START_NEW_GAME;
      return;
    }
  }
  // leds are blinking
  if((millis()-lastToggleTime > 400)||(millis()<lastToggleTime)){
    lastToggleTime=millis();
    TurnLedOff(CurrentLedIdxForWaitPhase);
    CurrentLedIdxForWaitPhase=(CurrentLedIdxForWaitPhase+1)%LED_NUM;
    TurnLedOn(CurrentLedIdxForWaitPhase);
  }
}
void start_new_game(){
  Lvl = map(analogRead(pinLvlChooser),0,4096,1,5);
  playStart();
  score=0;
  number_of_ON_leds=0;
  gameStartedTime=millis();
  curr_mode=GAME_ON;
}
void finish_game(){
  for(int k=0;k<LED_NUM;k++){
    TurnLedOff(k);
  }
  playFinish();
  delay(1000);
  start_wait_for_new_game();
}
void GameHandler(){
  for(int k=0;k<LED_NUM;k++){
    if(digitalRead(Btns[k])==HIGH){
      BtnIsPressed[k]=false;
    }
    else { //btn pressed
      if(!BtnIsPressed[k]) { // just started
        if(LedIsOn[k]){
          CorrectAnswer();
          TurnLedOff(k);
        }
        else {
          WrongAnswer();
        }
      }
      BtnIsPressed[k]=true;
    }
  }
  if(Should_I_Light_A_Led()){
    short LedNum=ChooseRandomLed();
    set_on_time_length(LedNum);
    TurnLedOn(LedNum);
  }
  TurnOffOldLeds();
  if(millis()-gameStartedTime > game_length){
    curr_mode=GAME_FINISHED;
  }
}
bool Should_I_Light_A_Led(){
  if(number_of_ON_leds < Lvl){
    return true;
  }
  return false;
}
short ChooseRandomLed(){
  short LedNum;
  do{
    LedNum=random(LED_NUM);
  } while((LedIsOn[LedNum])||(millis()-LedOnStartTime[LedNum] < min_off_time));
  return LedNum;
}
void set_on_time_length(short LedNum){
  int prd_min_val = 400+50*(4-Lvl);
  int prd_max_val = 800+120*(4-Lvl);
  int prd=random(prd_min_val,prd_max_val);
  LedOn_LengthTime[LedNum] = prd;
}
void TurnOffOldLeds(){
  for(int k=0;k<LED_NUM;k++){
    if(LedIsOn[k]){
      if(millis()-LedOnStartTime[k] > LedOn_LengthTime[k]){
        TurnLedOff(k);
      }
    }
  }
}
void CorrectAnswer(){
  playGood();
  score++;
}
void WrongAnswer(){
  playWrong();
  if(score>0)
    score--;
}

